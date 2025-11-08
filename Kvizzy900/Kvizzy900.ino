/** Arduino C/C++ ******************************************* Kvizzy900.ino ***
 *
 * Выводить данные GPS на сайт                  
 * (ориентировочно - от окна гостинной:  по гармину = 61.80193,  34.32983   
 *                                       по яндекс  = 61.802082, 34.329586)  
 * По материалам сайтов:
 * https://github.com/arduino/ArduinoCore-primo/blob/master/libraries/SoftwareSerial/examples/TwoPortReceive/TwoPortReceive.ino
 * https://docs.arduino.cc/tutorials/communication/TwoPortReceive/
 * 
 * В скетче принимает данные с двух программных последовательных портов и
 * отправляет их на аппаратный последовательный порт.
 * 
 * Чтобы прослушать программный порт, вызывается метод port.listen().
 * При использовании двух программных последовательных портов необходимо переключать порты,
 * прослушивая каждый из них по очереди. Следует выбирать логичное время для переключения
 * портов, например, в конце ожидаемой передачи или когда буфер пуст. 
 *
 * Вместо устаревшей TinyGPS используется TinyGPSPlus.
 * 
 * v2.0.4, 01.11.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 16.10.2025
 *
**/

#include <SoftwareSerial.h>
SoftwareSerial VKEL_TTL(12,13); // синий на 12 - будет RX; зеленый на 13 - будет TX
SoftwareSerial   SIM900( 7,8 ); // SIM900 

#include <MemoryFree.h>

// Определяем интервал подачи координат в мс на сайт
uint32_t deltaGPS=1000;             
// Резервируем буфер нефиксированных сообщений
char charMess[34];    

// Подключаем список 16-символьных сообщений приложения Kvizzy900
// и функцию вывода сообщений
#include "s16_Kvizzy900.h"

bool isSIM900=false;                    // "Не работает SIM900" = SIM900 does not work
uint32_t ncikl=0;                       // счетчик циклов
bool isFullCikl=true;                   // 9: true - "Выполняем прослушивание";        false - "Отрабатываем пустой цикл"
bool isMemTrass=false;                  // 8: true - "Показываем свободную память";    false - "Отменяем трассирование памяти"
bool isATTrass=true;                    // 7: true - "Показываем ответ на AT-команды"; false - "Отменяем трассирование AT-команд"

char charNumby[6]; // char[5]+'\0'
char* IntToChar(uint32_t numbIn) 
{
  int numby=numbIn;
  //static char charNumby[6]; // char[5]+'\0'
  memset(charNumby,'\0',6); 
  if (numby>99999) numby=99999;
  String(numby).toCharArray(charNumby,6);
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
  //char FreeM3[6];
  //memset(FreeM3,'\0',6); 
  strcat(charFree,FreeM1); 
  //String(getFreeMemory()).toCharArray(FreeM3,6);
  //strcat(charFree,FreeM3);   
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

// Обеспечиваем взаимодействие и выборку данных из приёмника GPS VKEL_TTL 
#include "VKEL_TTL.h"     
// Обеспечиваем взаимодействие с SIM900 и передачу данных на сайт  
#include "SIM900.h"     

void setup()
{
  Serial.begin(115200);
  VKEL_TTL.begin(9600); 
  SIM900.begin(9600);
  // Выводим сводку по памяти в начале программы
  Serial.println(" ");
  saymest(FreeMemoryToChar());
  // saymess(m1_Fill1);
}

void loop()
{
  // Отрабатываем управляющие команды из последовательного порта
  if (Serial.available())
  {
    int ccom = Serial.read();
    // Выполняем команду на пустое зацикливание
    // (например для того, чтобы посмотреть предыдущие сообщения)
    // или отменяем её
    if (ccom == '9') 
    {
      if (isFullCikl) {isFullCikl=false; saymess(m1_EmptyLoop);}
      else            {isFullCikl=true;  saymess(m1_anAudition);}
    }
    // Выполняем команду по трассировке утечек памяти
    // (показывать оставшуюся свободную память)
    // или отменяем её
    if (ccom == '8') 
    {
      if (isMemTrass) 
      {
        isMemTrass=false; 
        saymess(m1_NoMemoryTrace);
        if (!isFullCikl) {isFullCikl=true;  saymess(m1_anAudition);}
      }
      else {isMemTrass=true;  saymess(m1_FreeMemory);}
    }
    // Выполняем команды по трассировке AT-команд SIM900
    if (ccom == '7') 
    {
      if (isATTrass) 
      {
        isATTrass=false; 
        saymess(m1_NoATtrass);
        if (!isFullCikl) {isFullCikl=true;  saymess(m1_anAudition);}
      }
      else {isATTrass=true;  saymess(m1_ATcom);}
    }
    // Выполняем принудительную передачу последних принятых координат на сайт
    if (ccom == '1') 
    {
      SIM900.listen();
      glat=lat*1000000; glon=lng*1000000;   
      bool isSend=send_coords_at(glat,glon,67);
      if (!isSend) Serial.println("Неудачная отправка координат"); 
      VKEL_TTL.listen();
    }

  }
  // При необходимости трассируем память
  if (isMemTrass) saymest(FreeMemoryToChar());
  // Начинаем прослушивать устройства, так как разрешено
  if (isFullCikl)
  {
    ncikl++;
    // Прослушиваем приемник GPS V.KEL-TTL
    // (по умолчанию прослушивается последний инициализированный порт,
    // если требуется прослушивать другой, следует его явно указать)
    VKEL_TTL.listen();
    // Очищаем буфер последовательного порта V.KEL-TTL и делаем 
    // задержку чуть более секунды для того, чтобы он заполнился данными с координатами
    while (VKEL_TTL.available()) VKEL_TTL.read();
    delay(1100);
    // Выбираем данные навигации из приёмника GPS V.KEL TTL 
    isVKEL_TTL=Talk_VKEL_TTL(ncikl);
    // Если данные от приемника GPS есть, то
    // начинаем прослушивать и работать с портом SIM900
    if (isVKEL_TTL)
    {
      // Начинаем отсчет интервал в мс до следующего опроса GPS 
      BdelayGPS=millis();            
      // Работаем с SIM900
      SIM900.listen();
      // Проверяем, реагирует ли на команды SIM900
      // и включаем GPRS, если нет ответа
      if (ATcom("AT","OK",500)!=0)
      { 
        // Включаем SIM900
        saymess(m1_TurnOnSIM900);
        SIM900powerUpOrDown();
      }
      // Talk_SIM900(ncikl);
    }
    // Пересчитываем и указываем интервал отсутствия сигнала GPS
    else
    {
      delayGPS=millis()-BdelayGPS; 
      uint32_t deltaSec=delayGPS/1000;
      if (deltaSec<100) saymest(SecToChar(deltaSec));
      else 
      {
        uint32_t deltaMin=deltaSec/60;
        if (deltaMin<100) saymest(SecToChar(deltaMin,false));
        else saymess(m1_Delay99);
      }  
    }  
  }
  // Если закрыто прослушивание, то делаем заглушку 2 сек, 
  // чтобы по-человечески реагировать на другие команды 
  else delay(2000);
}

// Arduino C/C++ ******************************************** Kvizzy900.ino ***
                                                                                                                                                                                                  
