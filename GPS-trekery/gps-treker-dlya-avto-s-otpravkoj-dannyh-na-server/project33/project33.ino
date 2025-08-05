/* ========================================================================== */
/*                                                                            */
/*    project33.ino                                                               */
/*   (c) 2012 Author                                                          */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#include <SoftwareSerial.h>
#include "TinyGPS.h"

TinyGPS gps;
SoftwareSerial gpsSerial(2,3);  // синий на 2 - будет RX; зеленый на 3 - будет TX
bool newdata = false;
unsigned long start;
long lat, lon;
unsigned long time, date;

void setup()
{
  gpsSerial.begin(9600); // скорость обмена с GPS-приемником
  Serial.begin(9600);
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
      Serial.print("Lat: "); Serial.print(lat);
      Serial.print(" Long: "); Serial.print(lon);
      Serial.print(" Date: "); Serial.print(date);
      Serial.print(" Time: "); Serial.println(time);
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
