/** Arduino UNO, SIM900 **************************************** VKEL_TTL.h ***
 * 
 * Обеспечить взаимодействие и выборку данных из приёмника GPS VKEL_TTL 
 * 
 * v1.1.2, 04.11.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 16.10.2025
**/

#ifndef VKEL_TTL_h
#define VKEL_TTL_h
// Указываем, что данный файл нужно подключить только один раз
#pragma once  

// Настраиваем переменные и модули для работы с V.KEL TTL
#include <TinyGPSPlus.h>
TinyGPSPlus gps;

bool isVKEL_TTL=false;                  // "Приемник GPS не подает сигналы" = The GPS receiver does not send signals
double lat0=61.80193, lng0=34.32983;    // координаты предыдущей точки
uint32_t BdelayGPS=millis();            // начало отсчета задержки сигнала в опросе GPS 
uint32_t delayGPS;                      // задержка сигнала в опросе GPS 

// Данные, выбираемые из приёмника GPS
double lat=lat0,lng=lng0;               // координаты текущей точки (окна гостинной по гармину = 61.80193,34.32983)
double DistanceBetween;                 // расстояние между текущей и предыдущей точкой
int gday,gmonth,gyear;                  // день, месяц, год
int ghour,gmin,gsec;                    // час,минута,секунда
const int timezone_hours=3;             // Корректировка времени на время Москвы

// ****************************************************************************
// *                Сформировать сообщение о задержке сигнала GPS             *
// ****************************************************************************
const  char SecToCh1[18]="Задержка ";  
const  char SecToCh2[10]=" cек.";  
const  char SecToCh3[10]=" мин.";  

char* SecToChar(uint32_t MinSec, bool isSec=true) 
{
  // "1234567890123456"
  // "Задержка 23 сек."
  // "Задержка 99 мин."
  // "Задержка >99 мин"
  memset(charMess,'\0',34); 
  //char SecToCh4[4];
  //memset(SecToCh4,'\0',4); 
  strcat(charMess,SecToCh1); 
  //String(MinSec).toCharArray(SecToCh4,4);
  //strcat(charMess,SecToCh4);   
  strcat(charMess,IntToChar(MinSec));   
  if (isSec) strcat(charMess,SecToCh2); 
  else strcat(charMess,SecToCh3); 
  return charMess; 
}  
// ****************************************************************************
// *                     Сформировать сообщение о дате и времени              *
// ****************************************************************************
char* DTimeToChar(int gday, int gmonth, int ghour, int gmin, int gsec) 
{
  String stringOne;
  // "1234567890123456"
  // "27.10.2025 16:27"
  // "27.10 - 16:27:31"
  memset(charMess,'\0',18); 
  String chour; if (ghour<10) chour="0"+String(ghour); else chour=String(ghour);
  String cmin; if (gmin<10) cmin="0"+String(gmin); else cmin=String(gmin);
  String csec; if (gsec<10) csec="0"+String(gsec); else csec=String(gsec);
  stringOne=String(gday)+"."+=String(gmonth)+" - "+chour+":"+cmin+":"+csec;
  stringOne.toCharArray(charMess,17);
  return charMess;  
}  
// ****************************************************************************
// *               Сформировать сообщение о перемещении и времени             *
// *         (перемещение это расстояние до предыдущей точке локации)         *
// ****************************************************************************
char* DistTimeToChar(double DistanceBetween, int ghour, int gmin, int gsec) 
{
  String stringOne;
  // "1234567890123456"
  // "Движение 10.86 м"
  // "Движение 110.8 м"
  // "16:27:31 110.8 м"
  // "16:27:31 >1000 м"
  memset(charMess,'\0',34); 
  String chour; if (ghour<10) chour="0"+String(ghour); else chour=String(ghour);
  String cmin; if (gmin<10) cmin="0"+String(gmin); else cmin=String(gmin);
  String csec; if (gsec<10) csec="0"+String(gsec); else csec=String(gsec);
  String cdis; char chardis[6]; 
  if (DistanceBetween<100)         {dtostrf(DistanceBetween,2,2,chardis); cdis=chardis;} 
  else if (DistanceBetween<999.99) {dtostrf(DistanceBetween,3,1,chardis); cdis=chardis;}   
  else cdis=">1000";
  stringOne=chour+":"+cmin+":"+csec+" "+cdis+" м.";
  stringOne.toCharArray(charMess,33);
  return charMess;  
}  
// ****************************************************************************
// *                       Сформировать сообщение о локации                   *
// ****************************************************************************
char* LocationToChar(double lat, double lng) 
{
  String stringOne,stringlat,stringlng;
  // "1234567890123456"
  // "61.80191-34.3298"
  memset(charMess,'\0',18); 
  char charlat[12]; dtostrf(lat,2,5,charlat); stringlat=charlat;
  char charlng[12]; dtostrf(lng,2,4,charlng); stringlng=charlng;
  stringOne=stringlat+"-"+stringlng;
  stringOne.toCharArray(charMess,17);
  return charMess;  
}  
// ****************************************************************************
// *      Считать и расшифровать данные из буфера приёмника GPS V.KEL TTL     *
// ****************************************************************************
bool readgps()
{
  while (VKEL_TTL.available())
  {
    int b = VKEL_TTL.read();
    // !!! Windows обратно совместима с MS-DOS (даже в агрессивной форме), а в MS-DOS использовалась комбинация CR-LF, 
    // потому что MS-DOS была совместима с CP/M-80 (в некоторой степени случайно), в которой использовалась комбинация CR-LF, 
    // потому что так работал принтер (ведь изначально принтеры были пишущими машинками с компьютерным управлением).
    // В принтерах есть отдельная команда для перемещения бумаги на одну строку вверх и отдельная команда для возврата 
    // каретки (на которой закреплена бумага) к левому краю.
    // В современных устройствах по-прежнему есть эти команды, потому что они тоже обратно совместимы с более ранними принтерами
    // и другими устройствами. (В частности, HP хорошо справляется с этим).
    // В пишущих машинках тоже, сначала бумага поднимается ("LF" = "\n"), 
    // а затем каретка возвращается в исходное положение   ("CR" = "\r"), 
    // даже если это происходит одним движением. Звук «динг» сообщал, что конец строки близок и нужно подготовиться.

    // Отлавливаем конец строки с \r и \n
    if ('\r' != b)
    {
      if (gps.encode(b)) return true;
    }
  }
  return false;
}
// ****************************************************************************
// *         Выбрать данные навигации из буфера приёмника GPS V.KEL TTL,      *
// *                в случае неудачи вывести сообщение об ошибке              *
// ****************************************************************************
bool Talk_VKEL_TTL(unsigned long ncikl)
{
  // Serial.print(ncikl); Serial.println(": Talk_VKEL_TTL"); 
  // Инициируем данные приёмника GPS
  ghour=0; gmin=0; gsec=0; 
  gday=0; gmonth=0; gyear=0; 
  lat=0; lng=0; DistanceBetween=0;
  // Считываем и расшифроваем данные из буфера приёмника GPS V.KEL TTL 
  bool newdata = readgps();
  if (newdata)
  {
    // Определяем координаты и перемещение от предыдущей точки
    if (gps.location.isValid())
    {
      lat=gps.location.lat();
      lng=gps.location.lng();
      DistanceBetween = gps.distanceBetween(lat,lng,lat0,lng0);
      // Меняем прежнее положение для определения будущего расстояния между точками
      lat0=lat; lng0=lng;  
      saymest(LocationToChar(lat,lng));
    }
    // "Не определяется локация" 
    else
    {
      newdata = false;
      saymess(m1_LocateIsNot);
    }
    // Определяем дату
    if (gps.date.isValid())
    {
      gday=gps.date.day(); gmonth=gps.date.month(); gyear=gps.date.year(); 
      // Определяем время
      if (gps.time.isValid())
      {
        ghour=gps.time.hour(); gmin=gps.time.minute(); gsec=gps.time.second();
        ghour=ghour+timezone_hours;
        if (ghour>=24) ghour=ghour-24;
        else if (ghour<0) ghour=ghour+24;
        //saymest(DTimeToChar(gday,gmonth,ghour,gmin,gsec));
        saymest(DistTimeToChar(DistanceBetween,ghour,gmin,gsec)); 
      }
      // "Не определяется время"
      else 
      {
        newdata = false;
        saymess(m1_TimeIsNot);
      }
    }
    // "Не определяется дата"
    else
    {
      newdata = false;
      saymess(m1_DateIsNot);
    }
  }
  else
  {
    // "Приемник GPS не подает сигналы"
    saymess(m1_NotSignGPS);
  }
  return newdata;
}

#endif

// ************************************************************* VKEL_TTL.h ***

