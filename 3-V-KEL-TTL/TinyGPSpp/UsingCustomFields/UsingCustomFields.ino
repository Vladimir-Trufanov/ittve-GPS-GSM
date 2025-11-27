#include "TinyGPSpp.h"
#include <SoftwareSerial.h>

/*
   Этот пример демонстрирует возможности TinyGPSPlus по извлечению пользовательских
   полей из любого предложения NMEA.  В TinyGPSPlus есть встроенные средства для
   извлечения широты, долготы, высоты и т.д. из предложений $GPGGA и 
   $GPRMC.  Но с помощью типа TinyGPSCustom вы можете извлекать
   другие поля NMEA даже из нестандартных предложений NMEA.
*/
static const int RXPin = 12, TXPin = 13;
static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

/*
By declaring TinyGPSCustom objects like this, we announce that we
are interested in the 15th, 16th, and 17th fields in the $GPGSA 
sentence, respectively the PDOP (F("positional dilution of precision")),
HDOP (F("horizontal...")), and VDOP (F("vertical...")).

Объявляя объекты TinyGPSCustom подобным образом, мы объявляем, что нас
интересуют 15-е, 16-е и 17-е поля в предложении $GPGSA, 
соответственно PDOP (F("позиционное снижение точности")),
HDOP (F("горизонтальный...")) и VDOP (F("вертикальный...")).

HDOP (Horizontal Dilution of Precision)
---------------------------------------

HDOP — параметр в системах глобального позиционирования (GPS), 
который отражает точность горизонтальной составляющей положения (широта и долгота). 
Он указывает на то, насколько точно может быть определено горизонтальное положение 
в данных условиях. 

HDOP зависит от геометрического расположения спутников относительно приёмника: 
Низкое значение — спутники расположены далеко друг от друга (широкое покрытие неба).
Высокое значение — спутники сгруппированы в одной области, что снижает точность.

HDOP показывает, насколько велика погрешность координат из-за геометрического 
расположения спутников, даже если сигналы от каждого из них получены без ошибок. 

Некоторые значения HDOP и их интерпретация:
0–1 — идеальная точность, характерна для открытых пространств и качественного оборудования при отличных условиях приёма и большом количестве спутников. 
Координаты имеют погрешность в пределах 2–5 метров.
1–3 — высокая точность, подходит для большинства задач, включая мониторинг транспорта и расчёт пробега. 
Погрешность обычно в диапазоне 5–10 метров.
3–6 — средняя точность, погрешность может достигать 20–50 метров, особенно если спутники находятся низко над горизонтом 
или сгруппированы в одной части неба.
6 и более — низкая точность, погрешность превышает 
100 метров, данные становятся бесполезными. Такой уровень HDOP наблюдается в условиях 
плотной городской застройки, в туннелях или при плохом расположении спутников.

VDOP (Vertical Dilution of Precision)
-------------------------------------

VDOP — это снижение точности в вертикальной плоскости (высота) в системах глобального позиционирования. 
VDOP измеряет влияние геометрии спутников на точность высоты, которую определяет устройство GPS. Он показывает, 
как ошибки в положении спутников относительно друг друга могут влиять на точность данных о вертикальном расположении. 
Чем ниже значение VDOP, тем выше точность данных о высоте.

PDOP (Position Dilution of Precision)
-------------------------------------

PDOP — параметр в спутниковой навигации, который учитывает геометрическое расположение спутников 
относительно приёмника на Земле. Он показывает, насколько точно может быть определено положение, включая горизонтальную (широта и долгота) 
и вертикальную (высота) составляющие. 

Значение PDOP:
Низкое значение (например, 1–3) указывает на высокую точность определения положения.
Высокое значение (например, 5–10) указывает на более низкую точность.
 
Формула: PDOP^2 = HDOP^2 + VDOP^2. 

GPS Altitude
------------

GPS Altitude — это измерение высоты объекта над уровнем моря. 
Значение GPS Altitude определяется с помощью технологии Глобальной системы позиционирования (GPS). Для расчёта используется эллипсоид всей Земли. 
Важно учитывать, что значение GPS Altitude не всегда точно указывает на высоту объекта над землёй. На него могут влиять различные факторы, 
такие как атмосферное давление, температура и точность сигнала. 

(Counting starts with the field immediately following the sentence name, 
i.e. $GPGSA.  For more information on NMEA sentences, consult your
GPS module's documentation and/or http://aprs.gids.nl/nmea/.)
If your GPS module doesn't support the $GPGSA sentence, then you 
won't get any output from this program.
*/

TinyGPSCustom pdop(gps, "GPGSA", 15); // $GPGSA sentence, 15th element
TinyGPSCustom hdop(gps, "GPGSA", 16); // $GPGSA sentence, 16th element
TinyGPSCustom vdop(gps, "GPGSA", 17); // $GPGSA sentence, 17th element

void setup() 
{
  Serial.begin(115200);
  ss.begin(GPSBaud);

  Serial.println(F("UsingCustomFields.ino"));
  Serial.println(F("Demonstrating how to extract any NMEA field using TinyGPSCustom"));
  Serial.print(F("Testing TinyGPSPlus library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();
}

void loop() 
{
  // Every time anything is updated, print everything.
  if (gps.altitude.isUpdated() || gps.satellites.isUpdated() ||
    pdop.isUpdated() || hdop.isUpdated() || vdop.isUpdated())
  {
    Serial.print(F("ALT="));   Serial.print(gps.altitude.meters()); 
    Serial.print(F(" PDOP=")); Serial.print(pdop.value()); 
    Serial.print(F(" HDOP=")); Serial.print(hdop.value()); 
    Serial.print(F(" VDOP=")); Serial.print(vdop.value());
    Serial.print(F(" SATS=")); Serial.println(gps.satellites.value());
  }

  while (ss.available() > 0)
    gps.encode(ss.read());
}

