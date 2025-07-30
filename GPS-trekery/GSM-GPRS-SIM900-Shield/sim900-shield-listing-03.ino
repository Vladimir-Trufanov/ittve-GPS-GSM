
// подключение библиотеки SoftwareSerial
#include <SoftwareSerial.h>
// номер телефона для отправки sms (поменяйте на свой)
#define PHONE "+79031111111"
// Выводы для SoftwareSerial (у вас могут быть 2,3)
SoftwareSerial Sim900Serial(7, 8);
// подключение библиотеки
#include "DHT.h"
// пин для подключения датчика DHT
#define DHTPIN 2     
// тип датчика DHT
#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT22   
// создание экземпляра объекта DHT
DHT dht(DHTPIN, DHTTYPE);
// пороговое значение температуры
#define TEMPP 18

unsigned long millis1;

void setup() {
   // подключение последовательного порта
   Serial.begin(9600); 
   // запуск датчика DHT
   dht.begin();
   // запуск SoftwareSerial 
   Sim900Serial.begin(9600); 
}

void loop() {
   if (millis()-millis1>30*1000)  { // прошло 30 секунд?
      // получение данных влажности
      int h = dht.readHumidity();
      // получение данных влажности
      int t = dht.readTemperature();
      if(t<TEMPP) {
         SendTextMessage(t); // отправить sms
         //  10 минут
         delay(10*60*1000);    
      }
      millis1=millis();
   }
}
// подпрограмма отправки sms
void SendTextMessage(int t) {
   // AT-команда установки text mode
   Sim900Serial.print("AT+CMGF=1\r");
   delay(100);
   // номер телефона получателя
   Sim900Serial.println("AT + CMGS = \"");
   Sim900Serial.println(PHONE);
   Sim900Serial.println("\"");
   delay(100);
   // сообщение – данные температуры
   Sim900Serial.println(t);
   delay(100);
   // ASCII код ctrl+z – окончание передачи
   Sim900Serial.println((char)26);
   delay(100);
   Sim900Serial.println();
}

