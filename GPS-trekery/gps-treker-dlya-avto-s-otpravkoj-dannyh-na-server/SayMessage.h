/* ========================================================================== */
/*                                                                            */
/*   SayMessage.h                                                             */
/*   (c) 2025 Author                                                          */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#include "SoftwareSerial.h"

#define vypolnyaem_vklyuchenie_modulya_gprs  "Выполняем включение модуля GPRS"    
#define modul_gprs_rabotaet                  "Модуль GPRS работает"  
#define ne_vklyuchaetsya_gprs_net_pitaniya   "Не включается GPRS/нет питания"  

int say(String Message)
{
  int Result=0;
  Serial.print(Message);
  return Result;
}
int sayln(String Message)
{
  int Result=0;
  Serial.println(Message);
  return Result;
}
