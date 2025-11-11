/** Arduino UNO, SIM900 ************************************ mess_Lcd1602.h ***
 * 
 * Обеспечить вывод 16-символьных сообщений на дисплей LCD1602 из оперативной
 * и программной памяти в режиме выталкивания более старой строки сверху-вниз 
 * 
 * v1.0.1, 11.11.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 10.11.2025
**/

#ifndef mess_Lcd1602_h
#define mess_Lcd1602_h
// Указываем, что данный файл нужно подключить только один раз
#pragma once            

// Определяем макрос для размещения массива символов в программной памяти:
// const char pstr[] PROGMEM = "Массив символов pgm в программной памяти, Flash вместо RAM";
#ifndef _DS(name,value)
  #define _DS(name,value) const char name[] PROGMEM = value;
#endif

// Определяем макрос для выборки массива символов из Flash 
// напрямую, без копирования их в оперативную память RAM:
// (const __FlashStringHelper*) pstr
#ifndef _FS(name)
  #define _FS(name) (const __FlashStringHelper*) name
#endif

//               "123456789012345678901234567890123"
char oldmess[33]="                                 ";

void extmess(String _app1, char mess[])
{
  char newmess[33];
  memset(newmess,'\0',33); 
  strncpy_P(newmess, (const char*)mess,33);  // _P is the version to read from program space
  Serial.println(newmess);  

  /*
  Функция strncpy() в Arduino позволяет скопировать ограниченную часть одной строки (source) 
  в другую строку (destination), указав максимальную длину символов для копирования. 
  Это помогает предотвратить переполнение буфера. 

  Особенности:
  Нулевой символ включается в счёт длины.
  Если строка src короче, чем длина, буфер dest будет заполнен нулями до этой длины.
  Если src длиннее, чем длина, то нулевой символ добавлен не будет — dest может не являться валидной строкой после операции.

  strncpy(destination, source, length);
  
  Параметры:
  destination — массив символов, куда будет скопирована исходная строка.
  source — исходная строка, постоянный массив символов.
  length — максимальное количество символов, которые нужно скопировать из источника в destination.
  
  Важно: не рекомендуется пересечение памяти — копирование туда же, откуда идёт чтение. 

  Примеры:
  char buf = "01234567"; strncpy(buf, "hello", 5) — buf == "hello567".
  strncpy(buf, "hello", 6) — buf == "hello".
  */


}

void extmest(String _app1,char mess[])
{
  Serial.println(mess);  
}


#endif

// ********************************************************* mess_Lcd1602.h ***

