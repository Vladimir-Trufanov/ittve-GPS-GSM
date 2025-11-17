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
 * v3.0.1, 17.11.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 16.10.2025
 *
**/

#include <SoftwareSerial.h>
SoftwareSerial VKEL_TTL(12,13); // синий на 12 - будет RX; зеленый на 13 - будет TX
SoftwareSerial   SIM900( 7,8 ); // SIM900 

// Подключаем список 16-символьных сообщений приложения Kvizzy900
// и функцию вывода сообщений
#include "s16_Kvizzy900v3.h"

bool isSIM900=false;                    // "Не работает SIM900" = SIM900 does not work
uint32_t ncikl=0;                       // счетчик циклов
bool isFullCikl=true;                   // 9: true - "Выполняем прослушивание";        false - "Отрабатываем пустой цикл"
bool isMemTrass=false;                  // 8: true - "Показываем свободную память";    false - "Отменяем трассирование памяти"
bool isATTrass=true;                    // 7: true - "Показываем ответ на AT-команды"; false - "Отменяем трассирование AT-команд"

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
  saymess(DefToChar(m1_Fill));
  saymess(FreeMemoryToChar());
  delay(1500);
 }

void loop()
{
  bool isSend; // флаг успешности отправки координат
  // Отрабатываем управляющие команды из последовательного порта
  if (Serial.available())
  {
    int ccom = Serial.read();
    // Выполняем команду на пустое зацикливание
    // (например для того, чтобы посмотреть предыдущие сообщения)
    // или отменяем её
    if (ccom == '9') 
    {
      if (isFullCikl) {isFullCikl=false; saymess(DefToChar(m1_EmptyLoop));}
      else            {isFullCikl=true;  saymess(DefToChar(m1_anAudition));}
    }
    // Выполняем команду по трассировке утечек памяти
    // (показывать оставшуюся свободную память)
    // или отменяем её
    if (ccom == '8') 
    {
      if (isMemTrass) 
      {
        isMemTrass=false; 
        saymess(DefToChar(m1_NoMemoryTrace));
        if (!isFullCikl) {isFullCikl=true; saymess(DefToChar(m1_anAudition));}
      }
      else {isMemTrass=true; saymess(DefToChar(m1_FreeMemory));}
    }
    // Выполняем команды по трассировке AT-команд SIM900
    if (ccom == '7') 
    {
      if (isATTrass) 
      {
        isATTrass=false; 
        //saymess(m1_NoATtrass);
        if (!isFullCikl) {isFullCikl=true;  /*saymess(m1_anAudition);*/}
      }
      else {isATTrass=true;  /*saymess(m1_ATcom);*/}
    }
    // Выполняем принудительную передачу последних принятых координат на сайт
    if (ccom == '1') 
    {
      SIM900.listen();
      glat=lat*1000000; glon=lng*1000000;   
      isSend=send_coords_at(glat,glon,7);
      VKEL_TTL.listen();
    }
  }
  // При необходимости трассируем память
  //if (isMemTrass) saymest(FreeMemoryToChar());
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
      /*
      // Работаем с SIM900
      SIM900.listen();
      // Проверяем, реагирует ли на команды SIM900
      // и включаем GPRS, если нет ответа
      if (AT_com(AT_AT)!=0)
      { 
        // Включаем SIM900
        saymess(m1_TurnOnSIM900);
        SIM900powerUpOrDown();
        // Начинаем новый отсчет времени для передачи на сайт 
        BdelaySIM=millis();   
      }
      // Отсчитываем время и отправляем данные положения на сайт
      else
      {
        delaySIM=millis()-BdelaySIM; 
        if (delaySIM>dTimeSIM) 
        {
          glat=lat*1000000; glon=lng*1000000;   
          isSend=send_coords_at(glat,glon,ncikl);
          //if (!isSend) Serial.println("Неудачная отправка координат"); 
          //else Serial.println("УШЛИ КООРДИНАТЫ!"); 
          // Начинаем новый отсчет времени для передачи на сайт 
          BdelaySIM=millis();   
        }
      }
      */
      // Начинаем отсчет интервал в мс до следующего опроса GPS 
      BdelayGPS=millis();            
    }
    // Выводим причину, пересчитываем и указываем интервал отсутствия сигнала GPS
    else
    {
      // Формируем уточняющее сообщение о задержке
      delayGPS=millis()-BdelayGPS; 
      uint32_t deltaSec=delayGPS/1000;
      if (deltaSec<100) SecToChar(deltaSec);
      else 
      {
        uint32_t deltaMin=deltaSec/60;
        if (deltaMin<100) SecToChar(deltaMin,false);
        else DefToChar(m1_Delay99); 
      } 
      // Выводим уточняющее сообщение о задержке
      saymess(charMess);
    }  
  }
  // Если закрыто прослушивание, то делаем заглушку 2 сек, 
  // чтобы по-человечески реагировать на другие команды 
  else delay(2000);
}

// Arduino C/C++ ******************************************** Kvizzy900.ino ***
                                                                                                                                                                                                  
