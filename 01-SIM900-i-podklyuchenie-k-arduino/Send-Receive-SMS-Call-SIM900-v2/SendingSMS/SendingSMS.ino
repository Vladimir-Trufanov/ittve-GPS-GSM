/** Arduino C/C++ ****************************************** SendingSMS.ino ***
 *
 * Запрограммировать отправку SMS на телефон по его номеру.
 * 
 * v1.0.1, 28.07.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 27.07.2025
 *
**/

#include <SoftwareSerial.h>
SoftwareSerial SIM900(7,8); // SIM900 

void setup()
{
  Serial.begin(9600);
  SIM900.begin(9600);
  delay(500);
  updateSerial();
 
  Serial.println(" ");
  Serial.println("Инициализация и ожидание 1 сек...");
  delay(1000);

  SIM900.println("AT"); // Handshaking with SIM900
  updateSerial();

  SIM900.println("AT+CMGF?"); 
  updateSerial();

  // AT+CMGF=1 – Выбираем формат SMS-сообщения в виде текста. Формат по умолчанию  
  // - протокольная единица данных (PDU)
  // https://en.wikipedia.org/wiki/Protocol_data_unit
  SIM900.println("AT+CMGF=1"); 
  updateSerial();
  
  // AT+CMGS=+79214524295 – Отправляем SMS на указанный номер телефона. Текстовое
  // сообщение, за которым следует символ "Ctrl+z", рассматривается как SMS. 
  // "Ctrl+z" на самом деле является 26-м непечатаемым символом, описанным 
  // в таблице ASCII как "замена". Итак, нам нужно отправить 26 (1A Hex), как 
  // только мы отправим сообщение.
  // https://www.asciitable.com/
  SIM900.println("AT+CMGS=\"+79214524295\""); // change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  SIM900.print("DoorTry - PHP7 Error Collector 14 | https://doortry.ru/"); // text content
  updateSerial();
  SIM900.write(26);

  /*
  SIM900.println("AT+CMGS=\"+79214524295\""); // change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  SIM900.print("DoorTry - PHP7 Error Collector2 | https://doortry.ru/"); // text content
  updateSerial();
  SIM900.write(26);

  // AT+CMGF=0
  SIM900.println("AT+CMGF=0"); 
  updateSerial();

  SIM900.println("AT+CMGF?"); 
  updateSerial();

  // AT+CPAS - Определить состояние: +CPAS: 0 - готов к работе
  SIM900.println("AT+CPAS"); 
  updateSerial();
  */
}

void loop()
{
}

void updateSerial()
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

// Arduino C/C++ ******************************************* SendingSMS.ino ***
                                                                                                                                                                                                  
