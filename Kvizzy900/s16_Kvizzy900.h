/** Arduino UNO, SIM900 *********************************** s16_Kvizzy900.h ***
 * 
 * Обеспечить размещение 16-символьных сообщений дисплея в программной памяти 
 * и вывод их в последовательный порт или другой интерфейс
 * без копирования в оперативную память
 * 
 * v2.0.5, 09.11.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 16.10.2025
**/

#ifndef s16_Kvizzy900_h
#define s16_Kvizzy900_h
// Указываем, что данный файл нужно подключить только один раз
#pragma once            

// Готовим массивы символов для формирования сообщений
char charMess[34];      // буфер нефиксированных сообщений   
char chardec[8];        // буфер координат и дистанции, max 7 знаков и точка

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
_DS(m1_NotSignGPS,    "Нет сигнала GPS ")    // "Приемник GPS не подает сигналы"
_DS(m1_SIM900notWork, "Отключен SIM900 ")    // "Не работает SIM900"
_DS(m1_anAudition,    "Прослушиваем GPS")    // "Выполняем прослушивание" - Performing an audition
_DS(m1_EmptyLoop,     "Идет пустой цикл")    // "Отрабатываем пустой цикл" - Working out an empty loop
_DS(m1_Delay99,       "Задержка >99 мин")    // "Задержка >99 мин"
_DS(m1_TimeIsNot,     "Нет хода времени")    // "Не определяется время" - The time is not being determined
_DS(m1_DateIsNot,     "Нет данных даты ")    // "Не определяется дата" - The date is not being determined
_DS(m1_LocateIsNot,   "Не идёт локация ")    // "Не определяется локация" - "Location is not being determined"
_DS(m1_TurnOnSIM900,  "Включаем SIM900 ")    // "Включаем SIM900" - "Turning on the SIM900"
_DS(m1_FreeMemory,    "Смотрим память  ")    // "Показываем свободную память" - Showing free memory
_DS(m1_NoMemoryTrace, "Не глядим память")    // "Отменяем трассирование памяти" - Canceling memory tracing
_DS(m1_ATcom,         "Есть AT-команды ")    // "Показываем ответ на AT-команды"
_DS(m1_NoATtrass,     "Не трассируем AT")    // "Отменяем трассирование AT-команд"
_DS(m1_ResponsExceed, "SIM900 > 255 cим")    // "Ответ SIM900 превышает 255 символов" - The SIM900 response exceeds 255 characters
_DS(m1_NoReception,   "Нет приёма GPRS ")    // "За время тайм-аута не начат приём" - No reception is started during the timeout period
_DS(m1_NoConfirmed,   "Не успешен GPRS ")    // "Oтвет на команду не подтвержден" - The response to the command has not been confirmed
_DS(m1_NotCompleted,  "Не полный ответ ")    // "За время тайм-аута не завершён ответ" - The response was not completed during the timeout period

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
// ****************************************************************************
// *            Преобразовать беззнаковое  целое в строку символов            *
// ****************************************************************************
char charNumby[10]; // char[9]+'\0'
char* IntToChar(uint32_t numbIn) 
{
  int numby=numbIn;
  memset(charNumby,'\0',10); 
  if (numby>999999999) numby=999999999;
  String(numby).toCharArray(charNumby,10);
  return charNumby;
}
// ****************************************************************************
// *                       Сформировать сообщение по памяти                   *
// *                      (кириллица занимает 2 байта а UTF8)                 *
// ****************************************************************************
const  char FreeM1[14]="Память ";  
const  char FreeM2[10]=" байт";  
char* FreeMemoryToChar() 
{
  // v2 => программа=16774, переменные=1451, для локальных 597, free=474   
  static char charFree[28];
  memset(charFree,'\0',28); 
  // "1234567890123456"
  // "Память 1017 байт"
  strcat(charFree,FreeM1); 
  strcat(charFree,IntToChar(getFreeMemory()));   
  strcat(charFree,FreeM2);   
  return charFree; 
  // v1 => программа=16790, переменные=1457, для локальных 591, free=460  
  /*
  static char strFree[34];
  memset(strFree,'\0',34); 
  String stringOne;
  stringOne="Память "+String(getFreeMemory())+" байт";
  stringOne.toCharArray(strFree,33);
  return strFree;
  */  
}  
// ****************************************************************************
// *                Сформировать сообщение о задержке сигнала GPS             *
// ****************************************************************************
const char SecToCh1[18]="Задержка ";  
const char SecToCh2[10]=" cек.";  
const char SecToCh3[10]=" мин.";  
char* SecToChar(uint32_t MinSec, bool isSec=true) 
{
  // "1234567890123456"
  // "Задержка 23 сек."
  // "Задержка 99 мин."
  // "Задержка >99 мин"
  memset(charMess,'\0',34); 
  strcat(charMess,SecToCh1); 
  strcat(charMess,IntToChar(MinSec));   
  if (isSec) strcat(charMess,SecToCh2); 
  else strcat(charMess,SecToCh3); 
  return charMess; 
}  
// ****************************************************************************
// *               Сформировать сообщение о перемещении и времени             *
// *         (перемещение это расстояние до предыдущей точке локации)         *
// ****************************************************************************
const char DistT1[6]=" м.";  
const char DistT2[2]=":";  
const char DistT3[2]="0";  
const char DistT4[2]=" ";  
const char DistT5[6]=">1000";  
char* DistTimeToChar(double DistanceBetween, int ghour, int gmin, int gsec) 
{
  // "1234567890123456"
  // "Движение 10.86 м"
  // "Движение 110.8 м"
  // "16:27:31 110.8 м"
  // "16:27:31 >1000 м"
  memset(charMess,'\0',34); 
  // v2: 16098 + 1463 => 585 => 481
  if (ghour<10) {strcat(charMess,DistT3); strcat(charMess,IntToChar(ghour));}
  else strcat(charMess,IntToChar(ghour)); 
  strcat(charMess,DistT2); 
  if (gmin<10) {strcat(charMess,DistT3); strcat(charMess,IntToChar(gmin));}
  else strcat(charMess,IntToChar(gmin)); 
  strcat(charMess,DistT2); 
  if (gsec<10) {strcat(charMess,DistT3); strcat(charMess,IntToChar(gsec));}
  else strcat(charMess,IntToChar(gsec)); 
  strcat(charMess,DistT4); 
  if (DistanceBetween<100)         {dtostrf(DistanceBetween,2,2,chardec); strcat(charMess,chardec);} 
  else if (DistanceBetween<999.99) {dtostrf(DistanceBetween,3,1,chardec); strcat(charMess,chardec);}   
  else strcat(charMess,DistT5);
  strcat(charMess,DistT1); 
  /*
  // v1: 16620 + 1457 => 591 => 482
  char chardis[6];
  String stringOne;
  String chour; if (ghour<10) chour="0"+String(ghour); else chour=String(ghour);
  String cmin; if (gmin<10) cmin="0"+String(gmin); else cmin=String(gmin);
  String csec; if (gsec<10) csec="0"+String(gsec); else csec=String(gsec);
  String cdis;  
  if (DistanceBetween<100)         {dtostrf(DistanceBetween,2,2,chardis); cdis=chardis;} 
  else if (DistanceBetween<999.99) {dtostrf(DistanceBetween,3,1,chardis); cdis=chardis;}   
  else cdis=">1000";
  stringOne=chour+":"+cmin+":"+csec+" "+cdis+" м.";
  stringOne.toCharArray(charMess,33);
  */
  return charMess;  
}  
// ****************************************************************************
// *                       Сформировать сообщение о локации                   *
// ****************************************************************************
const char LocToCh[2]="-";  
char* LocationToChar(double lat, double lng) 
{
  // "1234567890123456"
  // "61.80191-34.3298"
  memset(charMess,'\0',18); 
  dtostrf(lat,2,5,chardec); strcat(charMess,chardec);
  strcat(charMess,LocToCh); 
  dtostrf(lat,2,4,chardec); strcat(charMess,chardec);
  return charMess;  
}  

#endif

// ******************************************************** s16_Kvizzy900.h ***

