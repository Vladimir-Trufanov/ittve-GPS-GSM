/*                                                              s16_Kvizzy900.h
*/

/** Arduino UNO ******************************************* *** simple1.ino ***
 *  (based on timer interrupts by Amanda Ghassaei June 2012)
 * 
 * Обеспечить мигание светодиода с интервалом в 1 сек (1Гц) на плате через 
 * прерывания 16-тиразрядного первого таймера - Timer1 по событию совпадения 
 * с регистром сравнения (то есть в режиме CTC)
 * 
 * v1.2, 21.03.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 20.03.2024
**/

#ifndef s16_Kvizzy900_h
#define s16_Kvizzy900_h

#define _app1    1      // Номер приложения для вывода сообщений
//#define _extmess_h    // Если определено, то еще передавать сообщение во внешнее приложение
#ifdef _extmess_h
  #include "_extmess.h"
#endif

// Представляем все сообщения 1 приложения "m1"
//const String m1_Fill  PROGMEM =     "1234567890123456";                  // 16
//const String m1_Fill1 PROGMEM =     "12";                                // 2
//const String m1_Fill2 PROGMEM =     "34"; //"12345678901234567890123456789012";  // 32

//String m1_Fill = F("1234567890123456");

// Определяем макрос для размещения массива символов в программной памяти:
// const char pstr[] PROGMEM = "Массив символов pgm в программной памяти, Flash вместо RAM";
#define _DS(name,value) const char name[] PROGMEM = value;

// Определяем макрос для выборки массива символов из Flash 
// напрямую, без копирования их в оперативную память RAM:
// (const __FlashStringHelper*) pstr
#define _FS(name) (const __FlashStringHelper*) name

_DS(m1_Fill,       "1234567890123456")           // 16
_DS(m1_Fill1,      "12")                         // 2
_DS(m1_Fill2,      "12345678901234567890123456789012")                         // 32 = "12345678901234567890123456789012"; 

_DS(m1_CliBSIM900, "Чист.буф. SIM900")           // Очищаем буфер SIM900

// ****************************************************************************
// *         Вывести сообщение внутри приложения в последовательный порт      *
// ****************************************************************************

/*
В Arduino есть специальный класс-обёртка для PROGMEM строк - __FlashStringHelper. 
Ардуиновский же интерфейс Print умеет печатать такие строки напрямую, без копирования 
их в оперативную память. Это позволяет печатать такие строки почти на все существующие дисплеи - 
библиотеки для них обычно поддерживают Print. Arduino-строки String также умеют 
создаваться из таких строк и прибавлять такие строки.

Работа со строками в программной памяти: https://alexgyver.ru/lessons/progmem/
Директивы препроцессора:                 https://alexgyver.ru/lessons/preprocessor/

1)
const char pstri[] PROGMEM = "pgm из программы string";
PGM_P ppstrp = pstri;

Serial.println((const __FlashStringHelper*) ppstrp);   // напечатает "pgm из программы string"
String s =     (const __FlashStringHelper*) ppstrp;    // создаст строку с "pgm из программы string"
Serial.println(s);                                     // напечатает "pgm из программы string"

2)
#define FPSTR(s) (const __FlashStringHelper*)(s)
const char pstr_g[] PROGMEM = "global pgm str";

Serial.println(FPSTR(pstr_g));  // напечатает "global pgm str"
saymess(pstr_g);

void saymess(char str[])
{
  Serial.println(FPSTR(str));  
}

*/

void saymess(char str[])
{
  //#ifdef _extmess_h
  //  // Передаем сообщение во внешнее приложение
  //  extmess(_app1,mess);
  //#endif
  Serial.println(_FS(str));  
}

#endif


// ************************************************************ simple1.ino ***

