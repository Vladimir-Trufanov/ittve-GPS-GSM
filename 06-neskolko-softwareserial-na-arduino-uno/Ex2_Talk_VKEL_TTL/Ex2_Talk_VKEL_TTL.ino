/** Arduino C/C++ *********************************** Ex2_Talk_VKEL_TTL.ino ***
 *
 * 2 пример из набора примеров "Несколько SoftwareSerial на Ардуино UNO"
 * 
 * Здесь через программный последовательный порт на пинах 2 = RX, 3 - TX, 
 * производится выборка данных навигации из приёмника GPS V.KEL TTL
  * 
 * v2.0.2, 16.10.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 07.08.2025
 *
**/

#include <SoftwareSerial.h>
#include "TinyGPS.h"

TinyGPS gps;
SoftwareSerial VKEL_TTL(2,3);      // синий на 2 - будет RX; зеленый на 3 - будет TX
unsigned long ncikl=0;
long lat, lon;
long lat0=61801900, lon0=34329700;  // координаты выбранной точки
float DistanceBetween;
unsigned long time, date;

void setup()
{
  Serial.begin(115200);
  VKEL_TTL.begin(9600); // скорость обмена с GPS-приемником
  Serial.println(" ");
  Serial.println("Ожидаем разговора с V.KEL-TTL ...");
}

void loop()
{
  ncikl++;
  // Делаем задержку в 1 секунду для того, чтобы буфер последовательного
  // порта V.KEL-TTL заполнился данными с координатами
  delay(1000);
  // Выбираем данные навигации из приёмника GPS V.KEL TTL 
  Talk_VKEL_TTL();
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

// Arduino C/C++ ************************************ Ex2_Talk_VKEL_TTL.ino ***
                                                                                                                                                                                                  
