/** Arduino UNO, SIM900 ********************************* s16_Kvizzy900v3.h ***
 * 
 * Обеспечить размещение 16-символьных сообщений дисплея в программной памяти 
 * и вывод их в последовательный порт или другой интерфейс
 * без копирования в оперативную память
 * 
 * v3.0.0, 16.11.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 16.10.2025
**/

#ifndef s16_Kvizzy900_h
#define s16_Kvizzy900_h
// Указываем, что данный файл нужно подключить только один раз
#pragma once    

#include <MemoryFree.h>

// Готовим массивы символов для формирования сообщений
char charMess[34];        // буфер сообщений для вывода на дисплей  
char chardec[8];          // буфер координат и дистанции, max 7 знаков и точка

// Готовим определения для вывода сообщений во внешнее приложение
char app[]="Kvizzy900";   // имя текущего приложения для внешнего приложения
#define _extmess_h      // если определено, то передавать сообщение во внешнее приложение
#ifdef _extmess_h
  #include "mess_Lcd1602v3.h"
#endif

// Определяем макрос для размещения массива символов в программной памяти:
// const char pstr[] PROGMEM = "Массив символов pgm в программной памяти, Flash вместо RAM";
#define _DS(name,value) const char name[] PROGMEM = value;
// Определяем макрос для выборки массива символов из Flash 
// напрямую, без копирования их в оперативную память RAM:
// (const __FlashStringHelper*) pstr
#define _FS(name) (const __FlashStringHelper*) name

// Представляем все сообщения 1 приложения "m1" (из 16 символов юникода)
// (из-за особенностей драйвера для LCD1602 по максимуму русские буквы
// представлены латинскими)
_DS(m1_Fill,          "                ")    // 16 байт
_DS(m1_Full,          "1234567890123456")    // 16 байт
_DS(m1_NotSignGPS,    "HET CИГHAЛA GPS ")    // "Приемник GPS не подает сигналы"
_DS(m1_SIM900notWork, "OTKЛЮЧEH SIM900 ")    // "Не работает SIM900"
_DS(m1_anAudition,    "ПPOCЛУШИBAEM GPS")    // "Выполняем прослушивание" - Performing an audition
_DS(m1_EmptyLoop,     "ИДET ПУCTOЙ ЦИKЛ")    // "Отрабатываем пустой цикл" - Working out an empty loop
_DS(m1_Delay99,       "ЗAДEPЖKA >99 min")    // "Задержка >99 мин"
_DS(m1_TimeIsNot,     "HET XOДA BPEMEHИ")    // "Не определяется время" - The time is not being determined
_DS(m1_DateIsNot,     "HET ДAHHЫX ДATЫ ")    // "Не определяется дата" - The date is not being determined
_DS(m1_LocateIsNot,   "HE ИДET ЛOKAЦИЯ ")    // "Не определяется локация" - "Location is not being determined"
_DS(m1_TurnOnSIM900,  "BKЛЮЧAEM SIM900 ")    // "Включаем SIM900" - "Turning on the SIM900"
_DS(m1_FreeMemory,    "CMOTPИM ПAMЯTЬ  ")    // "Показываем свободную память" - Showing free memory
_DS(m1_NoMemoryTrace, "HE ГЛЯДИM ПAMЯTЬ")    // "Отменяем трассирование памяти" - Canceling memory tracing
_DS(m1_ATcom,         "ECTЬ AT-KOMAHДЫ ")    // "Показываем ответ на AT-команды"
_DS(m1_NoATtrass,     "HE TPACCИPУEM AT")    // "Отменяем трассирование AT-команд"
_DS(m1_ResponsExceed, "SIM900 > 169 cим")    // "Ответ SIM900 превышает 169 символов" - The SIM900 response exceeds 169 characters
_DS(m1_NoReception,   "HET ПPИEMA GPRS ")    // "За время тайм-аута не начат приём" - No reception is started during the timeout period
_DS(m1_NoConfirmed,   "HE УCПEШEH GPRS ")    // "Oтвет на команду не подтвержден" - The response to the command has not been confirmed
_DS(m1_NotCompleted,  "HE ПOЛHЫЙ OTBET ")    // "За время тайм-аута не завершён ответ" - The response was not completed during the timeout period
_DS(m1_Wait5sek,      "ЖДEM OTBET 5 sec")    // "Ждем 5 сек для получения ответа" - "Waiting for a response for 5 seconds"

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

void saymess(char mess[])
{
  Serial.println(FPSTR(mess));  // v1
  Serial.println(_FS(mess));    // v2
}
*/
// Здесь есть важный момент:
// когда прослушивается SIM900, то не работают A4,A5,
// поэтому переключаем прослушивание на VKEL_TTL
void saymess(char mess[])
{
  Serial.println(mess);
  #ifdef _extmess_h
    VKEL_TTL.listen();
    delay(500);
    extmess(app,mess);
    delay(500);
    SIM900.listen();
  #endif
}
// ****************************************************************************
// *    Поместить в буфер для вывода на дисплей текст из программной памяти   *
// ****************************************************************************
char* DefToChar(char mess[]) 
{
  memset(charMess,'\0',34); 
  strcat_P(charMess,mess); 
  return charMess; 
}
// ****************************************************************************
// *            Преобразовать беззнаковое  целое в строку символов            *
// ****************************************************************************
char charNumby[10]; // char[9]+'\0'
char* IntToChar(uint32_t numbIn) 
{
  uint32_t numby=numbIn;
  memset(charNumby,'\0',10); 
  if (numby>999999999) numby=999999999;
  String(numby).toCharArray(charNumby,10);
  return charNumby;
}
// ****************************************************************************
// *                       Сформировать сообщение по памяти                   *
// *                      (кириллица занимает 2 байта а UTF8)                 *
// ****************************************************************************
const  char FreeM1[14]="ПAMЯTЬ ";  
const  char FreeM2[10]=" БAЙT";  
char* FreeMemoryToChar() 
{
  memset(charMess,'\0',34); 
  // "1234567890123456"
  // "Память 1017 байт"
  strcat(charMess,FreeM1); 
  strcat(charMess,IntToChar(getFreeMemory()));   
  strcat(charMess,FreeM2);
  return charMess; 
}  
// ****************************************************************************
// *                Сформировать сообщение о задержке сигнала GPS             *
// ****************************************************************************
const char SecToCh1[18]="ЗAДEPЖKA ";  
const char SecToCh2[10]=" sec.";  
const char SecToCh3[10]=" min.";  
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
const char DistT1[6]=" m.";  
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
  dtostrf(lng,2,4,chardec); strcat(charMess,chardec);
  return charMess;  
}  

#endif

// ****************************************************** s16_Kvizzy900v3.h ***

