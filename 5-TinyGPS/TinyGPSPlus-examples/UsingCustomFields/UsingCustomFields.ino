#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

/*
  Этот пример демонстрирует возможности TinyGPSPlus по извлечению пользовательских
  полей из любого предложения NMEA.  В TinyGPSPlus есть встроенные средства для
  извлечения широты, долготы, высоты и т.д. из предложений $GPGGA и 
  $GPRMC.  Но с помощью типа TinyGPSCustom вы можете извлекать
  другие поля NMEA даже из нестандартных предложений NMEA.
*/

static const int RXPin = 2, TXPin = 3; // синий на 2 - будет RX; зеленый на 3 - будет TX
static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

/*
  Объявляя объекты TinyGPSCustom подобным образом, мы объявляем, что нас
  интересуют 15-е, 16-е и 17-е поля в предложении $GPGSA, соответственно 
  PDOP (F("позиционное снижение точности")), HDOP (F("горизонтальный...")) и VDOP (F("вертикальный...")).

  (Подсчет начинается с поля, следующего непосредственно за названием предложения,
  т.е. с $GPGSA.  Для получения дополнительной информации о предложениях NMEA обратитесь к 
  документации по GPS-модулю и/или http://aprs.gids.nl/nmea/.)

  Если ваш GPS-модуль не поддерживает предложение $GPGSA, то вы
  не получите никаких выходных данных от этой программы.
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

