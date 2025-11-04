/** Arduino UNO, SIM900 **************************************** VKEL_TTL.h ***
 * 
 * Обеспечить взаимодействие с SIM900 и передачу данных на сайт 
 * 
 * v1.1.2, 02.11.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 16.10.2025
**/

#ifndef SIM900_h
#define SIM900_h
// Указываем, что данный файл нужно подключить только один раз
#pragma once  

// ****************************************************************************
// *                 Сформировать сообщения по ошибке AT-команды              *
// ****************************************************************************
void ATerrorMess(char* ATcommand, uint8_t answer) 
{
  // Выводим текст AT команды
  if (answer!=0) saymest(ATcommand);
  // Выводим сообщение об ошибке
  if (answer==1) saymess(m1_ResponsExceed);
  else if (answer==2) saymess(m1_NoReception);
  else if (answer==3) saymess(m1_NoConfirmed);
  else if (answer==4) saymess(m1_NotCompleted);
} 
// ****************************************************************************
// *               Отправить AT-командe на SIM900 и выбрать ответ             *
// ****************************************************************************
/*
Функция отправляет AT-команду в SIM900 и выбирает ответ за заданное время тайм-аута.
Возвращается ответ answer: 
  0 - "передана команда SIM900, получен подтверждающий ответ";
  1 - "ответ SIM900 превышает 150 символов";
  2 - "за время тайм-аута не начат приём ответа";
  3 - "ответ на команду не подтвержден"
  4 - "за время тайм-аута не завершён ответ"
*/
uint8_t ATcom(char* ATcommand, char* expected_answer, unsigned int timeout)
{
  // Резервируем переменную ответа
  static uint8_t answer;                  
  // Инициируем переменные
  uint8_t i;                               // позиция в заполняемом буфере ответа GPRS(SIM900)
  answer=2;                                // "за время тайм-аута не начат приём ответа"
  unsigned long previous;                  // время начала приема ответа от GPRS
  // Готовим буфер приёма ответа от GPRS
  char response[150];                      // объявили буфер ответа GPRS
  memset(response, '\0', 150);             // очистили буфер 
  i = 0;                                   // установили начальную позицию заполнения буфера
  delay(100);                              // сделали начальную задержку перед подачей команды
  previous = millis();                     // зафиксировали начальное время для отсчета таймаута
  // Очищаем возможно оставшийся прежний ответ от GPRS
  while (SIM900.available()>0) SIM900.read(); 
  // Отправляем AT-команду   
  SIM900.println(ATcommand);    
  // Циклимся, пока не выберем весь ответ за время тайм-аута
  do
  {
    // Если во входном буфере UART есть данные, считывает их 
    if (SIM900.available() != 0)
    {    
      response[i] = SIM900.read();
      i++;
      // Проверяем, не вышли ли за границу буфера
      if (i>149) {answer=1; break; }
    }
  }
  while((answer == 2) && ((millis() - previous) < timeout));
  // При необходимости трассируем ответ на AT-команду
  if (isATTrass) saymest(response);
  if (isATTrass) saymest(ATcommand);
  // Если вышли ли за границу буфера, то возвращаем ошибку
  // "ответ SIM900 превышает 150 символов"  
  if (answer==1) goto by; 
  // Если остались в начальной позиции, то возвращаем ошибку
  // "за время тайм-аута не начат приём ответа"  
  else if (i==0) goto by;  
  // Если еще есть символы в буфере SIM900, то возвращаем ошибку
  // "за время тайм-аута не завершён ответ"
  else if (SIM900.available()!=0) {answer=4; goto by;} 
  // Проверяем, есть ли желаемый ответ в ответе модуля
  if (strstr(response, expected_answer) != NULL)    
  /* 
    Функция strstr в C++ используется для поиска первого вхождения подстроки в строке. 
    Она определена в стандартной библиотеке C, поэтому доступна и в C++.
    Параметры: response — строка, в которой выполняется поиск; expected_answer — подстрока для поиска в строке response.
    Возвращаемое значение: указатель на первое вхождение подстроки в строке; нулевой указатель (nullptr), если подстрока не найдена.
    Важно: функция чувствительна к регистру — например, поиск по «hello» не соответствует «Hello». 
    Синтаксис: char* strstr(const char* str1, const char* str2).
    Чтобы использовать strstr, в начале кода нужно включить заголовочный файл <cstring> или <string.h> — 
    это гарантирует, что компилятор распознаёт функцию.
  */ 
  {
    // "передана команда SIM900, получен подтверждающий ответ"
    answer = 0;
  }
  else 
  {
    // "ответ SIM900 на команду не подтвержден"
    answer = 3;
  }
  // Завершаем работу функции и возвращаем ответ
  by:
  ATerrorMess(ATcommand,answer);
  // Serial.print("answer="); Serial.println(answer); 
  return answer;
}
// ****************************************************************************
// *  Включить/выключить (программный триггер) SIM900 через вывод D9 Arduino  *
// *     (В качестве синхронизации включения/выключения требуется импульс     *
// *    длительностью > 1 сек,  а для стабилизации синхронизации требуется    *
// *                          задержка > 3,2 сек)                             *
// ****************************************************************************
void SIM900powerUpOrDown()
{
  pinMode(9, OUTPUT);
  digitalWrite(9,LOW);
  delay(1000);
  digitalWrite(9,HIGH);
  delay(2000);
  digitalWrite(9,LOW);
  delay(3000);
}
// ****************************************************************************
// *                   Отправить набор AT-команд на SIM900                    *
// ****************************************************************************
void Talk_SIM900(unsigned long ncikl)
{
  int divisor=8;
  Serial.print(ncikl); Serial.print(": Talk_SIM900="); Serial.println(ncikl % divisor); 

  if (ncikl % divisor == 1)
  {
    Serial.println(" ");
    Serial.println("Разговор с SIM900 ...");
    delay(1000);
  }
  else if (ncikl % divisor == 2)
  {
    // AT - Рукопожатие с SIM900. "AT" – Это самая простая команда AT. Она также инициализирует передачу данных в бодах
    // Если это сработает, далее вы будете видеть эхо-сигнал символов AT, а затем OK, сообщающий вам, что все в порядке и SIM900 правильно вас понимает!
    ATcom("AT","OK",500);
  }
  else if (ncikl % divisor == 3)
  {
    // AT+CSQ – Проверить "уровень сигнала" – первое значение это уровень сигнала в дБ, он должен быть выше 5. Чем выше, тем лучше, до 31.
    ATcom("AT+CSQ","OK",500);
  }
  else if (ncikl % divisor == 4)
  {
    // AT+CCID – Получить номер SIM–карты, это проверка того, что SIM-карта найдена нормально, и вы можете проверить, какой номер присвоен карте
    ATcom("AT+CCID","OK",500);
  }
  else if (ncikl % divisor == 5)
  {
    // AT+CREG? Проверить, зарегистрированы ли вы в сети. Второе число должно быть 1 или 5. 1 означает, что вы зарегистрированы в домашней сети,
    // а 5 - в роуминговой сети. Кроме этих двух цифр, вы не зарегистрированы ни в какой сети
    ATcom("AT+CREG?","OK",500);
  }
  else if (ncikl % divisor == 6)
  {
    // ATI – Получить название платы и её редакцию
    ATcom("ATI","OK",500);
  }
  else if (ncikl % divisor == 7)
  {
    // AT+COPS? – Убедится, что вы подключены к сети и получить информацию об операторе
    ATcom("AT+COPS?","OK",500);
  }
  else if (ncikl % divisor == 0)
  {
    // AT+CBC – Получить состояние батареи lipo. Вторая цифра - это % заполнения, а третья цифра - фактическое напряжение в мВ
    ATcom("AT+CBC","OK",500);
  }
}

// 2025-11-04 вид запроса, введенный вручную 
// http://probatv.ru/State/?cycle=7&num=5&ctrl=204&sjson={"trkpt":{"lat":52518611,"lon":13376111,"color":"yellow"}}

// 2025-11-04 вид запроса, считанный из URL сайта в Edge и Google Chrome
// http://probatv.ru/State/?cycle=7&num=5&ctrl=204&sjson={%22trkpt%22:{%22lat%22:52518611,%22lon%22:13376111,%22color%22:%22yellow%22}}

// 2025-11-04 вид запроса, считанный из URL сайта в Yandex
// http://probatv.ru/State/?cycle=7&num=5&ctrl=204&sjson=%7B%22trkpt%22:%7B%22lat%22:52518611,%22lon%22:13376111,%22color%22:%22yellow%22%7D%7D

// 2 репозитария, которые могут пригодиться в будущем
// https://github.com/lbussy/LCBUrl
// https://github.com/plageoj/urlencode

bool send_coords_at(long lat, long lng)
{
  // (!SendAT("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"", "OK"))
  if   (ATcom("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"","OK",2000)!=2) return false;
    
  // (!SendAT("AT+SAPBR=3,1,\"APN\",\"internet.mts.ru\"", "OK"))
  if   (ATcom("AT+SAPBR=3,1,\"APN\",\"internet.mts.ru\"","OK",2000)!=2) return false;
  
  // (!SendAT("AT+SAPBR=1,1", "OK"))
  if   (ATcom("AT+SAPBR=1,1","OK",2000)!=2) 
  {
    // SendAT("AT+SAPBR=0,1", "OK"); //close bearer
        ATcom("AT+SAPBR=0,1","OK",2000);
    if (ATcom("AT+SAPBR=1,1","OK",2000)!=2) return false;
  }

  // (!SendAT("AT+HTTPINIT", "OK"))
  if   (ATcom("AT+HTTPINIT","OK",2000)!=2) return false;

  // (!SendAT("AT+HTTPPARA=\"CID\",1", "OK"))
  if   (ATcom("AT+HTTPPARA=\"CID\",1","OK",2000)!=2) return false;

  char url[1024];
  //sprintf(url,"AT+HTTPPARA=\"URL\",\"http://gurux13.net84.net/GpsTracking/record.php?Lat=%ld&Lng=%ld\"", lat, lng);
  sprintf(url,"AT+HTTPPARA=\"URL\",\"https://probatv.ru?Lat=%ld&Lng=%ld\"",lat,lng);

  // (!SendAT(url, "OK"))
  if   (ATcom(url,"OK",5000)!=2) return false;

  // (!SendAT("AT+HTTPACTION=0", "OK"))
  if   (ATcom("AT+HTTPACTION=0","OK",2000)!=2) return false;
  return true;
}


#endif

// *************************************************************** SIM900.h ***

