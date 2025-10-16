/* ============================================================ project33.ino */
/*                                                                            */
/*   Выводить данные GPS c указанием расстояния до выбранной точки            */
/*   (ориентировочно - правый угол балкона квартиры)                          */
/*   (c) 2025 tve                                                             */
/*                                                                            */
/*   v2.0.1 2025-08-07 - 2025.08.07                                           */
/* ========================================================================== */

#include <SoftwareSerial.h>
#include "TinyGPS.h"

TinyGPS gps;
SoftwareSerial gpsSerial(2,3);      // синий на 2 - будет RX; зеленый на 3 - будет TX
bool newdata = false;
unsigned long start;
long lat, lon;
long lat0=61801900, lon0=34329700;  // координаты выбранной точки
float DistanceBetween;
unsigned long time, date;

void setup()
{
  gpsSerial.begin(9600); // скорость обмена с GPS-приемником
  Serial.begin(115200);
  Serial.println(" ");
  Serial.println("Waiting data of GPS...");
}

void loop()
{
  // задержка в секунду между обновлениями координат
  if (millis() - start > 1000)
  {
    newdata = readgps();
    if (newdata)
    {
      start = millis();
      gps.get_position(&lat, &lon);
      gps.get_datetime(&date, &time);
      DistanceBetween=gps.distance_between (lat0,lon0,lat,lon)/10000000;
      //DistanceBetween=gps.distance_between (lat,lon,lat,lon);

      Serial.print("Lat: ");   Serial.print(lat);
      Serial.print(" Long: "); Serial.print(lon);
      Serial.print(" Date: "); Serial.print(date);
      Serial.print(" Time: "); Serial.print(time);
      Serial.print(" Dist: "); Serial.println(DistanceBetween);
    }
  }
}

// проверка наличия данных
bool readgps()
{
  while (gpsSerial.available())
  {
    int b = gpsSerial.read();
    // в TinyGPS есть ошибка: не обрабатываются данные с \r и \n
    if ('\r' != b)
    {
      if (gps.encode(b)) return true;
    }
  }
  return false;
}
