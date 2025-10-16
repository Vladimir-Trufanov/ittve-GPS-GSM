/** Arduino C/C++ ****************************************** MakingCall.ino ***
 *
 * Выполнить вызов. Этот эскиз очень полезен, когда вы хотите, чтобы ваш
 * Arduino позволили подать сигнал SOS/бедствия в случае чрезвычайной ситуации
 * 
 * v1.0.0, 29.07.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 29.07.2025
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

  // ATD+ +ZZXXXXXXXXXXX; – Набирает указанный номер. Точка с запятой ; - модификатор в конце
  // разделяет строку набора на несколько команд набора. Все команды, кроме последней, должны заканчиваться
  // точкой с запятой ; - модификатором.
  // SIM900.println("ATD+ +ZZxxxxxxxxxx;"); // change ZZ with country code and xxxxxxxxxxx with phone number to dial
  
  // SIM900.println("ATD+ +79214524295;"); 
  SIM900.println("ATD+79214524295;"); 
  updateSerial();
  delay(20000); // wait for 20 seconds...
  SIM900.println("ATH"); // hang up
  updateSerial();
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

// Arduino C/C++ ******************************************* MakingCall.ino ***
                                                                                                                                                                                                  
