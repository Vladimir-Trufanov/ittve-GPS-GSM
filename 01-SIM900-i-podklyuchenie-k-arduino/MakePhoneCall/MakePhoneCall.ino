/** Arduino C/C++ **************************************** MakePhoneCall.ino ***
 * 
 * Совершить телефонный звонок - Makе a phone call
 * 
 * v1.0, 26.07.2025                                    Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                                Дата создания: 26.07.2025
 *
 * Complete project details at http://randomnerdtutorials.com
**/

/*
----- Выполнить звонок:
ATD+79214524295;
OK
----- Определить состояние:
AT+CPAS
---
+CPAS: 4    - голосовое соединение
OK
----- Завершить вызов:
ATH0
---
OK
----- Определить состояние:
AT+CPAS
---
+CPAS: 0
OK
*/

#include <SoftwareSerial.h>

int waitingForSIM900=2000;  // ожидание готовности SIM900
String atcom;               // AT-команда
String vk="\r";             // "возврат каретки"

// Configure software serial port
SoftwareSerial SIM900(7,8); 

void setup() 
{
  // Arduino communicates with SIM900 GSM shield at a baud rate of 9600
  // Make sure that corresponds to the baud rate of your module
  Serial.begin(9600);
  SIM900.begin(9600);
  // Give time to your GSM shield log on to network
  delay(waitingForSIM900);

  // Make the phone call
  callSomeone();
}

void loop() 
{
  
}

void callSomeone() 
{
  sayAt(vk); 
  atcom="ATD+79214524295;";
  sayAt(atcom); 
  delay(100);
  sayAt(vk); 
  
  // Делаем звонок в течение 30 секунд
  // Вы можете изменить продолжительность телефонного звонка в поле Время задержки
  delay(30000);
  // "Вешаем трубку"
  atcom="ATH";
  sayAt(atcom); 
}

void sayAt(String at) 
{
  if (at==vk)
  {
    Serial.println("ВК");
  }
  else 
  {
    Serial.println(at);
    SIM900.print(at);
  }
  SIM900.print(vk);
}

// Arduino C/C++ ***************************************** MakePhoneCall.ino ***
                                                                                                                                                                                                  
