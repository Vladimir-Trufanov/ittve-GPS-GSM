/** Arduino C/C++ ******************************************* Kvizzy900.ino ***
 *
 * Выводить данные GPS на сайт                  
 * (ориентировочно - от окна гостинной:  по гармину = 61.80193,  34.32983   
 *                                       по яндекс  = 61.802082, 34.329586)  
 * По материалам сайтов:
 * https://github.com/arduino/ArduinoCore-primo/blob/master/libraries/SoftwareSerial/examples/TwoPortReceive/TwoPortReceive.ino
 * https://docs.arduino.cc/tutorials/communication/TwoPortReceive/
 * 
 * В скетче принимает данные с двух программных последовательных портов и
 * отправляет их на аппаратный последовательный порт.
 * 
 * Чтобы прослушать программный порт, вызывается метод port.listen().
 * При использовании двух программных последовательных портов необходимо переключать порты,
 * прослушивая каждый из них по очереди. Следует выбирать логичное время для переключения
 * портов, например, в конце ожидаемой передачи или когда буфер пуст. 
 *
 * Вместо устаревшей TinyGPS используется TinyGPSPlus.
 * 
 * v2.0.4, 01.11.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 16.10.2025
 *
**/

#include <SoftwareSerial.h>
SoftwareSerial VKEL_TTL(12,13); // синий на 12 - будет RX; зеленый на 13 - будет TX
SoftwareSerial   SIM900( 7,8 ); // SIM900 

#include <MemoryFree.h>

// Определяем интервал подачи координат в мс на сайт
uint32_t deltaGPS=1000;             

// Подключаем список 16-символьных сообщений приложения Kvizzy900
// и функцию вывода сообщений
#include "s16_Kvizzy900.h"
// Резервируем буфер нефиксированных сообщений
char charMess[34];    

// Обеспечиваем взаимодействие и выборку данных из приёмника GPS VKEL_TTL 
#include "VKEL_TTL.h"     
// Обеспечиваем взаимодействие с SIM900 и передачу данных на сайт  
#include "SIM900.h"     

bool isSIM900=false;                    // "Не работает SIM900" = SIM900 does not work
unsigned long ncikl=0;                  // счетчик циклов
bool isFullCikl=true;                   // true - "Выполняем прослушивание";     false - "Отрабатываем пустой цикл"
bool isMemTrass=false;                  // true - "Показываем свободную память"; false - "Отменяем трассирование памяти"

void setup()
{
  Serial.begin(115200);
  VKEL_TTL.begin(9600); 
  SIM900.begin(9600);
  // Выводим сводку по памяти в начале программы
  Serial.println(" ");
  saymest(FreeMemoryToChar());
  // Включаем SIM900
  //saymess(m1_TurnOnSIM900);
  //SIM900powerUp();

  // Очищаем буфер SIM900
  while (SIM900.available()) SIM900.read();
  saymess(m1_CliBSIM900);
  // saymess(m1_Fill1);
  Serial.println("Ожидаем разговора с V.KEL-TTL ...");
}

void loop()
{
  // Отрабатываем управляющие команды из последовательного порта
  if (Serial.available())
  {
    int ccom = Serial.read();
    // Выполняем команду на пустое зацикливание
    // (например для того, чтобы посмотреть предыдущие сообщения)
    // или отменяем её
    if (ccom == '9') 
    {
      if (isFullCikl) {isFullCikl=false; saymess(m1_EmptyLoop);}
      else            {isFullCikl=true;  saymess(m1_anAudition);}
    }
    // Выполняем команду по трассировке утечек памяти
    // (показывать оставшуюся свободную память)
    // или отменяем её
    if (ccom == '8') 
    {
      if (isMemTrass) 
      {
        isMemTrass=false; 
        saymess(m1_NoMemoryTrace);
        if (!isFullCikl) {isFullCikl=true;  saymess(m1_anAudition);}
      }
      else            {isMemTrass=true;  saymess(m1_FreeMemory);}
    }

    /*
    int c = Serial.read();
    // Выполняем команду на пустое зацикливание
    // (например для того, чтобы посмотреть предыдущие сообщения)
    if (c == 'a') 
    {
      isFullCikl=false;
      saymess(m1_EmptyLoop);
    }
    // Отменяем команду на пустое зацикливание
    else if (c == 's') 
    {
      isFullCikl=true;
      saymess(m1_anAudition);
    }
    */
  }
  // При необходимости трассируем память
  if (isMemTrass) saymest(FreeMemoryToChar());
  // Начинаем прослушивать устройства, так как разрешено
  if (isFullCikl)
  {
    ncikl++;
    // Прослушиваем приемник GPS V.KEL-TTL
    // (по умолчанию прослушивается последний инициализированный порт,
    // если требуется прослушивать другой, следует его явно указать)
    VKEL_TTL.listen();
    // Очищаем буфер последовательного порта V.KEL-TTL и делаем 
    // задержку чуть более секунды для того, чтобы он заполнился данными с координатами
    while (VKEL_TTL.available()) VKEL_TTL.read();
    delay(1100);
    // Выбираем данные навигации из приёмника GPS V.KEL TTL 
    isVKEL_TTL=Talk_VKEL_TTL(ncikl);
    // Если данные от приемника GPS есть, то
    // начинаем прослушивать и работать с портом SIM900
    if (isVKEL_TTL)
    {
      // Начинаем отсчет интервал в мс до следующего опроса GPS 
      BdelayGPS=millis();            
      // Работаем с SIM900
      SIM900.listen();
      delay(500);
      Talk_SIM900(ncikl);
    }
    // Пересчитываем и указываем интервал отсутствия сигнала GPS
    else
    {
      delayGPS=millis()-BdelayGPS; 
      uint32_t deltaSec=delayGPS/1000;
      if (deltaSec<100) saymest(SecToChar(deltaSec));
      else 
      {
        uint32_t deltaMin=deltaSec/60;
        if (deltaMin<100) saymest(SecToChar(deltaMin,false));
        else saymess(m1_Delay99);
      }  
    }  
  }
  // Делаем заглушку 2 сек, чтобы не по-человечески реагировать на другие команды 
  else delay(2000);
}
// ****************************************************************************
// *                       Сформировать сообщение по памяти                   *
// ****************************************************************************
char* FreeMemoryToChar() 
{
  String stringOne;
  // "1234567890123456"
  // "Память 1017 байт"
  memset(charMess,'\0',34); 
  stringOne="Память "+String(getFreeMemory())+" байт";
  stringOne.toCharArray(charMess,33);
  return charMess;  
}  

// Arduino C/C++ ******************************************** Kvizzy900.ino ***
                                                                                                                                                                                                  
