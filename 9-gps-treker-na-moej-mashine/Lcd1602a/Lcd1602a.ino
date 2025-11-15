/** Arduino UNO, SIM900 ************************************** Lcd1602a.ino ***
 * 
 * Отладить подключение дисплея LCD1602a через I2C для SIM900
 * 
 * v1.0.1, 11.11.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 16.10.2025
**/

// Подключаем список 16-символьных сообщений приложения Kvizzy900
// и функции подготовки и вывода сообщений
#include <MemoryFree.h>
#include "s16_Kvizzy900.h"
 
void setup()
{
  Serial.begin(115200);

  //lcd.setCursor(0,0);               // установили курсор в начало 1 строки
  //lcd.print("Hello, world");        // вывели текст
  //lcd.setCursor(0,1);               // установили курсор в начало 2 строки
  //lcd.print("РУСИФИК-ация! 58");    // распечатали текст
  
  saymess(m1_NotSignGPS);
  delay(2000);
  saymest("УШЛИ КООРДИНАТЫ!"); 
  delay(2000);
  saymess(m1_SIM900notWork);
  delay(2000);
  saymest("Новое сообщение."); 
  delay(2000);
  saymess(m1_anAudition);
  delay(2000);
  saymess(m1_EmptyLoop);
  delay(2000);
  saymess(m1_Delay99);
  delay(2000);
  saymess(m1_TimeIsNot);
  delay(2000);
  saymess(m1_DateIsNot);
  delay(2000);
  saymess(m1_LocateIsNot);
  delay(2000);
  saymess(m1_TurnOnSIM900);
  delay(2000);
  saymess(m1_FreeMemory);
  delay(2000);
  saymess(m1_NoMemoryTrace);
  delay(2000);
  saymess(m1_ATcom);
  delay(2000);
  saymess(m1_NoATtrass);
  delay(2000);
  saymess(m1_ResponsExceed);
  delay(2000);
  saymess(m1_NoReception);
  delay(2000);
  saymess(m1_NoConfirmed);
  delay(2000);
  saymess(m1_NotCompleted);
  delay(2000);
  saymess(m1_Wait5sek);
  delay(2000);
}

void loop()
{
}

// *********************************************************** Lcd1602a.ino ***

