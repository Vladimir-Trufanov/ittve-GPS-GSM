/** Arduino UNO, SIM900 ************************************** Lcd1602a.ino ***
 * 
 * Отладить подключение дисплея LCD1602a через I2C для SIM900
 * 
 * v1.0.1, 11.11.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 16.10.2025
**/

// Подключаем библиотеку LCD_1602_RUS (наследницу LiquidCrystal_I2C.h)
#include <LCD_1602_RUS.h> 
// Создаём объект для LCD-дисплея
LCD_1602_RUS lcd(0x27,16,2); 
// Подключаем список 16-символьных сообщений приложения Kvizzy900
// и функции подготовки и вывода сообщений
#include <MemoryFree.h>
#include "s16_Kvizzy900.h"
 
void setup()
{
  Serial.begin(115200);

  lcd.init();                       // проинициализировали lcd    
  lcd.backlight();                  // включили подсветку
  lcd.setCursor(0,0);               // установили курсор в начало 1 строки
  lcd.print("Hello, world");        // вывели текст
  lcd.setCursor(0,1);               // установили курсор в начало 2 строки
  lcd.print("РУСИФИК-ация! 58");    // распечатали текст
  
  saymess(m1_anAudition);
  delay(1000);
  saymest("УШЛИ КООРДИНАТЫ!"); 
  delay(1000);
  saymess(m1_ResponsExceed);
  delay(1000);
  saymest("Новое сообщение."); 
  delay(1000);
}

void loop()
{
}

// *********************************************************** Lcd1602a.ino ***

