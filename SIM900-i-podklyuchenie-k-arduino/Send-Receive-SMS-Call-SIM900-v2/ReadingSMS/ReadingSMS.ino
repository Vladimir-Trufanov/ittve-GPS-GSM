/** Arduino C/C++ ****************************************** ReadingSMS.ino ***
 *
 * Запрограммировать отправку SMS на телефон по его номеру.
 * 
 * v1.0.1, 28.07.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 27.07.2025
 *
 * 
 * Увеличение размера буфера последовательного порта в коде программного обеспечения Arduino:
 * 
 * Если ваше сообщение достаточно длинное, то, вероятно, вы получите его с некоторыми
 * пропущенными символами. Это не связано с ошибочным кодом. Ваш буфер приема 
 * последовательного кода программного обеспечения заполняется и отбрасывает символы. 
 * Вы недостаточно быстро считываете данные из буфера.
 * 
 * Самое простое решение этой проблемы - увеличить размер программного последовательного буфера с
 * 64 байт по умолчанию до 256 байт (или меньше, в зависимости от того, что вам подходит).
 * На ПК с Windows перейдите по соответствующей ссылке SoftwareSerial.h и измените строку:
 * 
 * C:\Users\Евгеньевич\AppData\Local\Arduino15\packages\arduino\hardware\avr\1.8.6\libraries\SoftwareSerial\src
 * C:\Program Files (x86) -> Arduino -> hardware -> Arduino -> avr -> libraries -> SoftwareSerial (-> src for newer version of Arduino IDE)
 * C:\Program Files (tve) -> arduino-1.8.19 -> hardware -> arduino -> avr -> libraries -> SoftwareSerial -> src
 
**/

#define _SS_MAX_RX_BUFF 256

#include <SoftwareSerial.h>
SoftwareSerial SIM900(7,8); // SIM900 

void setup()
{
  Serial.begin(9600);
  SIM900.begin(9600);
  delay(500);
  updateSerial();
 
  Serial.println(" ");
  Serial.print("_SS_MAX_RX_BUFF = "); Serial.println(_SS_MAX_RX_BUFF);
  Serial.println("Инициализация и ожидание 1 сек...");
  delay(1000);

  SIM900.println("AT"); // Handshaking with SIM900
  updateSerial();

  SIM900.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  
  // AT+CNMI=1,2,0,0,0 – указывает, как следует обрабатывать вновь поступающие SMS-сообщения. 
  // Таким образом, вы можете приказать SIM900 shield либо переслать вновь поступившие 
  // SMS-сообщения непосредственно на ПК, либо сохранить их в хранилище сообщений, 
  // а затем уведомить компьютер об их местоположении в хранилище сообщений.
  
  // Его ответ начинается с +CMT: Все поля в ответе разделены запятыми, первое
  // поле - это номер телефона. Второе поле - это имя человека, отправляющего SMS. 
  // Третье поле - это временная метка, а четвертое поле - само сообщение.
  
  SIM900.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
  updateSerial();
  
  // Обратите внимание, что на этот раз мы не оставили функцию повторной отправки 
  // пустой, поскольку мы проводим опрос на предмет поступления новых SMS-сообщений. 
  // Как только вы отправите SMS на SIM900 GSM/GPRS shield, вы увидите вывод на последовательный монитор.
}

void loop()
{
 updateSerial();
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


/*
void setup()
{

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
  SIM900.print("DoorTry - PHP7 Error Collector13 | https://doortry.ru/"); // text content
  updateSerial();
  SIM900.write(26);

  
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
  
}

void loop()
{
}
*/

// Arduino C/C++ ******************************************* ReadingSMS.ino ***
                                                                                                                                                                                                  
