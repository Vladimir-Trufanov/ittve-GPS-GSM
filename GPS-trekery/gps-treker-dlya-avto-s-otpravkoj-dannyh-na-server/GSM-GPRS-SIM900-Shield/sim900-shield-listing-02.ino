
// подключение библиотек
#include <SoftwareSerial.h>
#include "DHT.h"
// телефон для отправки sms 
#define PHONE_NUMBER "+7928222222"
// создание объектов 
SoftwareSerial gsm(7, 8);
DHT sensorDHT(2, DHT22);
// переменные
String phone = ""
String str1 = ""; //
boolean isSMS = false;

void setup() {
   // подключение последовательного порта
   Serial.begin(9600); 
   // запуск датчика DHT
   dht.begin();
   // запуск SoftwareSerial 
   gsm.begin(9600); 
   // Настрайка приёма сообщений 
   gsm.print("AT+CMGF=1\r");
   delay(300);
   gsm.print("AT+IFC=1, 1\r");
   delay(300);
   gsm.print("AT+CPBS=\"SM\"\r");
   delay(300);
   gsm.print("AT+CNMI=1,2,2,1,0\r");
   delay(500);
}

void loop() {
   if (gsm.available()) {
      char c = gsm.read();
      if ('\r' == c) {
         if (isSMS) { // текущая строка - sms-сообщение,
            if (!str1.compareTo("temp")) { // текст sms - temp
               // отправить sms на приходящий номер
               // получение данных 
               int t = dht.readTemperature();
               // AT-команда установки text mode
               gsm.print("AT+CMGF=1\r");
               delay(100);
               // номер телефона получателя
               gsm.println("AT + CMGS = \"");
               gsm.println(PHONE);
               gsm.println("\"");
               delay(100);
               // сообщение – данные температуры
               gsm.println(t);
               delay(100);
               // ASCII код ctrl+z – окончание передачи
               gsm.println((char)26);
               delay(100);
               gsm.println();
            }
            Serial.println(currStr);
            isSMS = false;
         }
         else {
            if (str1.startsWith("+CMT")) {
               Serial.println(str1);
               // выделить из сообщения номер телефона
               phone=str1.substring(7,19);
               Serial.println(phone);
               // если текущая строка начинается с "+CMT",
               // то следующая строка является сообщением
               isSMS = true;
            }
         }
         str1 = "";
      }
      else if ('\n' != c) {
         str1 += String(c);
      }
   }
}


