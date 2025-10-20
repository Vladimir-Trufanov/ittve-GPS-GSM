/** Arduino C/C++ ********************************** Ex4_TwoPortReceive.ino ***
 *
 * 5 пример из набора примеров "Несколько SoftwareSerial на Ардуино UNO"
 * 
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
 * v1.0.1, 20.10.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 16.10.2025
 *
**/

#include <SoftwareSerial.h>
SoftwareSerial VKEL_TTL(2,3); // синий на 2 - будет RX; зеленый на 3 - будет TX
SoftwareSerial   SIM900(7,8); // SIM900 
unsigned long ncikl=0;

// Настраиваем переменные и модули для работы с V.KEL TTL
// #include "TinyGPS.h"
#include <TinyGPSPlus.h>

// TinyGPS gps;
TinyGPSPlus gps;

double lat,lng; 
double lat0=61.801900, lng0=34.329700;  // координаты выбранной точки
double DistanceBetween;
unsigned long time, date;

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
    // По умолчанию прослушивается последний инициализированный порт,
    // если требуется прослушивать другой, следует его явно указать
    VKEL_TTL.listen();
    // Делаем задержку в 1 секунду для того, чтобы буфер последовательного
    // порта V.KEL-TTL заполнился данными с координатами
    delay(1000);
    // Выбираем данные навигации из приёмника GPS V.KEL TTL 
    // Talk_VKEL_TTL();
    Talk_VKEL_TTL1();
    
    /*
    // This sketch displays information every time a new sentence is correctly encoded.
    while (VKEL_TTL.available() > 0) if (gps.encode(VKEL_TTL.read())) displayInfo();
    if (millis() > 5000 && gps.charsProcessed() < 10)
    {
      Serial.println(F("No GPS detected: check wiring."));
      while(true);
    }
    */

    // Начинаем прослушивать и работать с портом SIM900
    SIM900.listen();
    delay(500);
    Talk_SIM900();
  }
}

void displayInfo()
{
  Serial.print(ncikl); Serial.print(":"); 
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
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
    Serial.print(F("INVALID"));
  }

  Serial.println();
}

// ****************************************************************************
// *            Выбрать данные навигации из приёмника GPS V.KEL TTL           *
// ****************************************************************************
bool Talk_VKEL_TTL()
{
  /*
  bool newdata = false;
  // Если данные есть, то считываем их и публикуем
  newdata = readgps();
  if (newdata)
  {
    gps.get_position(&lat, &lon);
    gps.get_datetime(&date, &time);
    DistanceBetween=gps.distance_between (lat0,lon0,lat,lon)/10000000;

    Serial.print(ncikl);     Serial.print(":"); 
    Serial.print(" Lat: ");  Serial.print(lat);
    Serial.print(" Long: "); Serial.print(lon);
    Serial.print(" Date: "); Serial.print(date);
    Serial.print(" Time: "); Serial.print(time);
    Serial.print(" Dist: "); Serial.println(DistanceBetween);
  }
  return newdata;
  */
}

// ****************************************************************************
// *            Выбрать данные навигации из приёмника GPS V.KEL TTL           *
// ****************************************************************************
bool Talk_VKEL_TTL1()
{
  bool newdata = false;
  // Если данные есть, то считываем их и публикуем
  newdata = readgps();
  if (newdata)
  {
    Serial.print(ncikl); Serial.print(":"); 
    if (gps.location.isValid())
    {
      lat=gps.location.lat();
      lng=gps.location.lng();
      //Serial.print("lat=");  Serial.println(lat,6);
      //Serial.println(gps.location.lat());
      Serial.print(lat,6); Serial.print(F(",")); Serial.print(lng,6);
      DistanceBetween = gps.distanceBetween(lat,lng,lat0,lng0);
      Serial.print(" DistanceBetween=");  Serial.println(DistanceBetween,6);
      
      /*
        TinyGPSPlus::distanceBetween(
          gps.location.lat(),
          gps.location.lng(),
          LONDON_LAT, 
          LONDON_LON);
      */

    }
    else
    {
      Serial.print(F("Неопределяется локация"));
    }


    /*
    gps.get_position(&lat, &lon);
    gps.get_datetime(&date, &time);
    DistanceBetween=gps.distance_between (lat0,lon0,lat,lon)/10000000;

    Serial.print(ncikl);     Serial.print(":"); 
    Serial.print(" Lat: ");  Serial.print(lat);
    Serial.print(" Long: "); Serial.print(lon);
    Serial.print(" Date: "); Serial.print(date);
    Serial.print(" Time: "); Serial.print(time);
    Serial.print(" Dist: "); Serial.println(DistanceBetween);
    */
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
    // в TinyGPS есть ошибка: не обрабатываются данные с \r и \n
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

// Arduino C/C++ *********************************** Ex4_TwoPortReceive.ino ***
                                                                                                                                                                                                  
