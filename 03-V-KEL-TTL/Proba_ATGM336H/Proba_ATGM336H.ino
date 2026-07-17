/*

https://pastebin.com/nrckSEve

GPS-модуль ATGM336H — это компактный GPS-модуль с интерфейсом UART. Его можно использовать 
с платой Arduino и библиотекой TinyGPS++ для считывания данных GPS. 

Connect your ATGM336H GPS module to the Arduino board as follows:
 
GPS module VCC to Arduino 3.3V
GPS module GND to Arduino GND
GPS module TX to Arduino pin 8 (RX)
GPS module RX to Arduino pin 7 (TX)
 
Open the Serial Monitor in the Arduino IDE and set the baud rate to 9600. You should see the GPS coordinates displayed when new data is received from the GPS module.
*/
 
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
 
#define RX_PIN 2
#define TX_PIN 3
#define BAUD_RATE 9600
 
SoftwareSerial gpsSerial(RX_PIN, TX_PIN);
TinyGPSPlus gps;
 
void setup() 
{
  Serial.begin(BAUD_RATE);
  gpsSerial.begin(BAUD_RATE);
  Serial.println("GPS module initialized.");
}
 
void loop() 
{
  while (gpsSerial.available()) 
  {
    gps.encode(gpsSerial.read());
  }
 
  if (gps.location.isUpdated()) 
  {
    float latitude = gps.location.lat();
    float longitude = gps.location.lng();
    
    Serial.print("Latitude: ");
    Serial.println(latitude, 6);
    Serial.print("Longitude: ");
    Serial.println(longitude, 6);
  }
}