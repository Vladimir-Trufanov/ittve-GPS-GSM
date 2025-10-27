/** Arduino UNO, SIM900 *********************************** s16_Kvizzy900.h ***
 *  (based on timer interrupts by Amanda Ghassaei June 2012)
 * 
 * Обеспечить размещение сообщений в программной память и вывод их в
 * последовательный порт без копирования в оперативную память
 * 
 * v2.0.2, 26.10.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 16.10.2025
**/

#ifndef s16_Kvizzy900_h
#define s16_Kvizzy900_h

// Готовим определения для вывода сообщений во внешнее приложение
#define _app1    1      // Номер текущего приложения для внешнего приложения
//#define _extmess_h    // Если определено, то передавать сообщение во внешнее приложение
#ifdef _extmess_h
  #include "_extmess.h"
#endif

// Определяем макрос для размещения массива символов в программной памяти:
// const char pstr[] PROGMEM = "Массив символов pgm в программной памяти, Flash вместо RAM";
#define _DS(name,value) const char name[] PROGMEM = value;
// Определяем макрос для выборки массива символов из Flash 
// напрямую, без копирования их в оперативную память RAM:
// (const __FlashStringHelper*) pstr
#define _FS(name) (const __FlashStringHelper*) name

// Определяем тестовые сообщения
_DS(m1_Fill1,      "12")                                 // 2 байта
_DS(m1_Fill2,      "12345678901234567890123456789012")   // 32 байта 

// Представляем все сообщения 1 приложения "m1" (из 16 символов юникода)
_DS(m1_Fill,          "1234567890123456")    // 16 байт
_DS(m1_CliBSIM900,    "Чистим SIM900   ")    // "Чистим буфер SIM900"
_DS(m1_NotSignGPS,    "Нет сигнала GPS ")    // "Приемник GPS не подает сигналы"
_DS(m1_SIM900notWork, "Отключен SIM900 ")    // "Не работает SIM900"
_DS(m1_anAudition,    "Прослушиваем GPS")    // "Выполняем прослушивание" - Performing an audition
_DS(m1_EmptyLoop,     "Идет пустой цикл")    // "Отрабатываем пустой цикл" - Working out an empty loop
_DS(m1_Delay99,       "Задержка >99 мин")    // "Задержка >99 мин"

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

void saymess(char mess[])
{
  #ifdef _extmess_h
    // Передаем сообщение во внешнее приложение
    extmess(_app1,_FS(mess));
  #endif
  Serial.println(_FS(mess));  
}

void saymest(char mess[])
{
  #ifdef _extmess_h
    // Передаем сообщение во внешнее приложение
    extmess(_app1,mess);
  #endif
  Serial.println(mess);
}

#endif

// ******************************************************** s16_Kvizzy900.h ***

