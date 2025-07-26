/** Arduino C/C++ *********************************** TurnSIM900_upDown.ino ***
 *
 * Включение/выключение чипа SIM900.
 * Даже при включении питания платы SIM900 необходимо включить чип SIM900, чтобы он заработал.
 * Согласно техническому описанию, для включения/выключения питания микросхемы необходимо 
 * нажимать контакт PWRKEY в течение как минимум 1 секунды. Это можно сделать двумя способами:
 * - Hardware Trigger. Плата оснащена прямоугольным тактильным переключателем, 
 * расположенным рядом со светодиодным индикатором PWR. Для включения/выключения экрана 
 * необходимо нажимать на этот переключатель в течение примерно 2 секунд.
 * - Software trigger. Вместо того чтобы каждый раз нажимать клавишу PWR вручную, 
 * вы можете программно включать/выключать SIM900. 
 * 
 * Для этого, сначала вам нужно припаять перемычку SMD между контактами R13 на 
 * плате. Далее вам нужно подключить вывод D9 на экране к выводу D9 на Arduino.
 * 
 * v1.0, 26.07.2025                                   Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 26.07.2025
 *
**/

#include <SoftwareSerial.h>

int waitingForSIM900=5000;  // ожидание готовности SIM900
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

  // Проверяем, отвечает ли SIM900
  delay(500);
  if (SIM900.available())
  {
    Serial.println("SIM900 доступен!");
  }
  else
  {
    Serial.println("SIM900 НЕТ!");
  }
 
  
  SIM900power();
   
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



void SIM900power()
{
 pinMode(9, OUTPUT);
 digitalWrite(9,LOW);
 delay(1000);
 digitalWrite(9,HIGH);
 delay(2000);
 digitalWrite(9,LOW);
 delay(3000);
}

// Arduino C/C++ ************************************ TurnSIM900_upDown.ino ***
                                                                                                                                                                                                  
