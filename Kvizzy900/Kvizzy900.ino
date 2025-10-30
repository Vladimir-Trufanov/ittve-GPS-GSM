/** Arduino C/C++ ******************************************* Kvizzy900.ino ***
 *
 * Выводить данные GPS на сайт - ознакомительный вариант,                  
 * ориентировочно - от окна гостинной:  по гармину = 61.80193,  34.32983   
 *                                      по яндекс  = 61.802082, 34.329586  
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
 * v2.0.3, 28.10.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 16.10.2025
 *
**/

#include <SoftwareSerial.h>
SoftwareSerial VKEL_TTL(12,13); // синий на 12 - будет RX; зеленый на 13 - будет TX
SoftwareSerial   SIM900( 7,8 ); // SIM900 

#include <MemoryFree.h>

// Подключаем список 16-символьных сообщений приложения Kvizzy900
// и функцию вывода сообщений
#include "s16_Kvizzy900.h"
// Обеспечиваем взаимодействие и выборку данных из приёмника GPS VKEL_TTL 
#include "VKEL_TTL.h"     
// Обеспечиваем взаимодействие с SIM900 и передачу данных на сайт  
#include "SIM900.h"     

bool isSIM900=false;                    // "Не работает SIM900" = SIM900 does not work
unsigned long ncikl=0;                  // счетчик циклов
bool isFullCikl=true;                   // "Выполняем прослушивание" то есть не "Отрабатываем пустой цикл"

void setup()
{
  Serial.begin(115200);
  VKEL_TTL.begin(9600); 
  SIM900.begin(9600);
  
  Serial.println(" ");
  // Включаем SIM900
  saymess(m1_TurnOnSIM900);
  SIM900powerUp();

  // Очищаем буфер SIM900
  while (SIM900.available()) SIM900.read();
  saymess(m1_CliBSIM900);
  // saymess(m1_Fill1);

  Serial.print("0. freeMemory()=");
  Serial.println(getFreeMemory());

  Serial.println("Ожидаем разговора с V.KEL-TTL ...");
}

void loop()
{
  // Отрабатываем управляющие команды из последовательного порта
  if (Serial.available())
  {
    int c = Serial.read();
    // Выполняем команду на пустое зацикливание
    // (например для того, чтобы посмотреть предыдущие сообщения)
    if (c == 'a') 
    {
      isFullCikl=false;
      saymess(m1_EmptyLoop);
    }
    // Отменяем команду на пустое зацикливание
    else if (c == 's') 
    {
      isFullCikl=true;
      saymess(m1_anAudition);
    }
  }
  if (isFullCikl)
  {
    // Выполняем задержку перед очередным снятием показаний приёмника GPS
    delay(deltaGPS); 
    ncikl++;

    Serial.print(ncikl);
    Serial.print(". freeMemory()=");
    Serial.println(getFreeMemory());

    // Прослушиваем приемник GPS V.KEL-TTL
    // (по умолчанию прослушивается последний инициализированный порт,
    // если требуется прослушивать другой, следует его явно указать)
    VKEL_TTL.listen();
    // Делаем задержку в 1 секунду для того, чтобы буфер последовательного
    // порта V.KEL-TTL заполнился данными с координатами
    delay(1000);
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
      delay(500);
      Talk_SIM900(ncikl);
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
}

// Arduino C/C++ ******************************************** Kvizzy900.ino ***
                                                                                                                                                                                                  
