/** Arduino C/C++ ******************************************* Kvizzy900.ino ***
 *
 * Выводить данные GPS на сайт - ознакомительный вариант,                  
 * ориентировочно - от окна гостинной:  по гармину = 61.80193,  34.32983   
 *                                      по яндекс  = 61.802082, 34.329586  
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
 * v2.0.1, 24.10.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 16.10.2025
 *
**/

#include <SoftwareSerial.h>
SoftwareSerial VKEL_TTL(2,3); // синий на 2 - будет RX; зеленый на 3 - будет TX
SoftwareSerial   SIM900(7,8); // SIM900 
unsigned long ncikl=0;

// Настраиваем переменные и модули для работы с V.KEL TTL
#include <TinyGPSPlus.h>
TinyGPSPlus gps;

// Подключаем список 16-символьных сообщений приложения Kvizzy900
// и функцию вывода сообщений
#include "s16_Kvizzy900.h"

double lat,lng; 
double lat0=61.801900, lng0=34.329700;  // координаты выбранной точки
double DistanceBetween;

void setup()
{
  Serial.begin(115200);
  VKEL_TTL.begin(9600); 
  SIM900.begin(9600);
  
  delay(500);
  Serial.println(" ");
  Serial.println("Очищаем буфер SIM900");
  while (SIM900.available()) SIM900.read();
  Serial.println("Ожидаем разговора с V.KEL-TTL ...");
}
 
void loop()
{
  ncikl++;
  if (ncikl>8) delay(1000);
  else
  {
    //Serial.println(_FS(m1_CliBSIM900)); 
    saymess(m1_CliBSIM900);
    saymess(m1_Fill2);
    // По умолчанию прослушивается последний инициализированный порт,
    // если требуется прослушивать другой, следует его явно указать
    VKEL_TTL.listen();
    //saymess(m1_Fill1);
    //saymess(m1_Fill);
    // Делаем задержку в 1 секунду для того, чтобы буфер последовательного
    // порта V.KEL-TTL заполнился данными с координатами
    delay(1000);
    //saymess(m1_Fill2);
    //saymess(m1_Fill);
    // Выбираем данные навигации из приёмника GPS V.KEL TTL 
    Talk_VKEL_TTL();

    // Начинаем прослушивать и работать с портом SIM900
    SIM900.listen();
    delay(500);
    Talk_SIM900();
  }
}


// ****************************************************************************
// *            Выбрать данные навигации из приёмника GPS V.KEL TTL           *
// ****************************************************************************
bool Talk_VKEL_TTL()
{
  bool newdata = false;
  // Если данные есть, то считываем их и публикуем
  newdata = readgps();
  if (newdata)
  {
    Serial.print(ncikl); Serial.print(". "); 
    // Определяем координаты
    if (gps.location.isValid())
    {
      lat=gps.location.lat();
      lng=gps.location.lng();
      Serial.print(F("Координаты: ")); Serial.print(lat,6); Serial.print(F(",")); Serial.print(lng,6);
      DistanceBetween = gps.distanceBetween(lat,lng,lat0,lng0);
      Serial.print(F(". Расстояние от предыдущей точки: ")); Serial.print(DistanceBetween,2); Serial.print(F(" м."));
      // Меняем прежнее положение для определения будущего расстояния между точками
      lat0=lat; lng0=lng;  
    }
    else
    {
      Serial.print(F("Неопределяется локация"));
    }
    Serial.println();
    // Определяем дату
    if (gps.date.isValid())
    {
      Serial.print(gps.date.day());
      Serial.print(F("."));
      Serial.print(gps.date.month());
      Serial.print(F("."));
      Serial.print(gps.date.year());
    }
    else
    {
      Serial.print(F("Неопределяется дата "));
    }
    Serial.print(F(" "));
    // Определяем время
    if (gps.time.isValid())
    {
      if (gps.time.hour() < 10) Serial.print(F("0"));
      Serial.print(gps.time.hour());
      Serial.print(F(":"));
      if (gps.time.minute() < 10) Serial.print(F("0"));
      Serial.print(gps.time.minute());
      Serial.print(F(":"));
      if (gps.time.second() < 10) Serial.print(F("0"));
      Serial.print(gps.time.second());
      Serial.print(F("."));
      if (gps.time.centisecond() < 10) Serial.print(F("0"));
      Serial.print(gps.time.centisecond());
    }
    else
    {
      Serial.print(F("Неопределяется время "));
    }
    Serial.println();
  }
  else
  {
    if (millis() > 5000 && gps.charsProcessed() < 10)
    {
      Serial.println(F("GPS не обнаружен: проверьте соединения и оборудование"));
      while(true);
    }
  }
  return newdata;
}
// ****************************************************************************
// *          Проверить наличие данных в буфере приёмника GPS V.KEL TTL       *
// ****************************************************************************
bool readgps()
{
  while (VKEL_TTL.available())
  {
    int b = VKEL_TTL.read();
    // !!! Windows обратно совместима с MS-DOS (даже в агрессивной форме), а в MS-DOS использовалась комбинация CR-LF, 
    // потому что MS-DOS была совместима с CP/M-80 (в некоторой степени случайно), в которой использовалась комбинация CR-LF, 
    // потому что так работал принтер (ведь изначально принтеры были пишущими машинками с компьютерным управлением).
    // В принтерах есть отдельная команда для перемещения бумаги на одну строку вверх и отдельная команда для возврата 
    // каретки (на которой закреплена бумага) к левому краю.
    // В современных устройствах по-прежнему есть эти команды, потому что они тоже обратно совместимы с более ранними принтерами
    // и другими устройствами. (В частности, HP хорошо справляется с этим).
    // В пишущих машинках тоже, сначала бумага поднимается ("LF" = "\n"), 
    // а затем каретка возвращается в исходное положение   ("CR" = "\r"), 
    // даже если это происходит одним движением. Звук «динг» сообщал, что конец строки близок и нужно подготовиться.

    // Отлавливаем конец строки с \r и \n
    if ('\r' != b)
    {
      if (gps.encode(b)) return true;
    }
  }
  return false;
}

// ****************************************************************************
// *               Выбрать ответное сообщение из буфера SIM900                *
// ****************************************************************************
void update_SIM900()
{
  delay(500);
  while (Serial.available())
  {
    SIM900.write(Serial.read()); // Forward what Serial received to Software Serial Port
  }
  while (SIM900.available())
  {
    Serial.write(SIM900.read()); // Forward what Software Serial received to Serial Port
  }
}
// ****************************************************************************
// *                   Отправить набор AT-команд на SIM900                    *
// ****************************************************************************
void Talk_SIM900()
{
  int divisor=8;
  Serial.print(ncikl); Serial.print(": "); 
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
    SIM900.println("AT"); // Handshaking with SIM900
    update_SIM900();
  }
  else if (ncikl % divisor == 3)
  {
    // AT+CSQ – Проверить "уровень сигнала" – первое значение это уровень сигнала в дБ, он должен быть выше 5. Чем выше, тем лучше, до 31.
    SIM900.println("AT+CSQ"); // Signal quality test, value range is 0-31 , 31 is the best
    update_SIM900();
  }
  else if (ncikl % divisor == 4)
  {
    // AT+CCID – Получить номер SIM–карты, это проверка того, что SIM-карта найдена нормально, и вы можете проверить, какой номер присвоен карте
    SIM900.println("AT+CCID"); // Read SIM information to confirm whether the SIM is plugged
    update_SIM900();
  }
  else if (ncikl % divisor == 5)
  {
    // AT+CREG? Проверить, зарегистрированы ли вы в сети. Второе число должно быть 1 или 5. 1 означает, что вы зарегистрированы в домашней сети,
    // а 5 - в роуминговой сети. Кроме этих двух цифр, вы не зарегистрированы ни в какой сети
    SIM900.println("AT+CREG?"); // Check whether it has registered in the network
    update_SIM900();
  }
  else if (ncikl % divisor == 6)
  {
    // ATI – Получить название платы и её редакцию
    SIM900.println("ATI"); 
    update_SIM900();
  }
  else if (ncikl % divisor == 7)
  {
    // AT+COPS? – Убедится, что вы подключены к сети и получить информацию об операторе
    SIM900.println("AT+COPS?"); 
    update_SIM900();
  }
  else if (ncikl % divisor == 0)
  {
    // AT+CBC – Получить состояние батареи lipo. Вторая цифра - это % заполнения, а третья цифра - фактическое напряжение в мВ
    SIM900.println("AT+CBC"); 
    update_SIM900();
  }
}

// Arduino C/C++ ******************************************** Kvizzy900.ino ***
                                                                                                                                                                                                  
