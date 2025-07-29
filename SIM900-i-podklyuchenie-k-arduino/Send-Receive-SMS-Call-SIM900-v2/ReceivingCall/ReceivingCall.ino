/** Arduino C/C++ *************************************** ReceivingCall.ino ***
 *
 * Для получения вызова не требуется никакого специального кода; вам просто нужно 
 * продолжать прослушивать эфир платой SIM900. Этот набросок может оказаться очень 
 * полезным, когда вам нужно активировать действие при поступлении вызова с определенного телефонного номера.
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
}

// Входящий вызов обычно обозначается ‘RING’ на последовательном мониторе, за 
// которым следует номер телефона и идентификатор вызывающего абонента. 
// Для принятия/отмены вызова используются следующие команды AT:
// 
// ATA – Принимает входящий вызов.
// ATH – завершает вызов. При завершении вызова на последовательном мониторе не 
// отображается оператор сотовой связи, что указывает на невозможность подключения.

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

// Arduino C/C++ **************************************** ReceivingCall.ino ***
                                                                                                                                                                                                  
