/* ======================================================= sketch_sep30a.ino */
/*                                                                           */
/*   Выводить данные GPS на сайт - ознакомительный вариант,                  */
/*   ориентировочно - от окна гостинной:  по гармину = 61.80193,  34.32983   */
/*                                        по яндекс  = 61.802082, 34.329586  */
/*   (c) 2025 tve                                                            */
/*                                                                           */
/*   v2.0.1 2025-10-13 - 2025.08.07                                          */
/* ========================================================================= */

#include <SoftwareSerial.h>
#include "TinyGPS.h"

TinyGPS gps;
SoftwareSerial gpsSerial(2,3);   // синий на 2 - будет RX; зеленый на 3 - будет TX, приемник gps-координат

SoftwareSerial gsmSerial(7,8);   // SIM900
#define LEDPIN 13
//#define RSTgprsPIN 22
#define RSTGPSPIN  23
int led_period = 2000;           // интервал мигания лампочки 2 секунды

void setup()
{
  Serial.begin(9600);          // USB
  //gsmSerial.begin(19200);       // GSM
  // Задаем максимальное время ожидания данных во время операций чтения 6 секунд
  //gsmSerial.setTimeout(6000);
  gpsSerial.begin(9600);
  pinMode(LEDPIN, OUTPUT);
  //pinMode(RSTgprsPIN, OUTPUT); 
  //pinMode(RSTGPSPIN, OUTPUT); 
  digitalWrite(LEDPIN, LOW);
  //digitalWrite(RSTgprsPIN, LOW);
  //digitalWrite(RSTGPSPIN, HIGH);  
}

unsigned long last_powerup_attempt = 0;
#define MINPOWERUPDELAY 10000
#define MINFIRSTPOWERUPDELAY 100
void power_up_gprs()
{
  if ((millis() - last_powerup_attempt < MINPOWERUPDELAY) &&
  ((last_powerup_attempt != 0) || (millis() <= MINFIRSTPOWERUPDELAY)))
    return;
 Serial.println("Powering up the gprs shield");
 last_powerup_attempt = millis();
 //digitalWrite(RSTgprsPIN,HIGH);
 delay(2000);
 //digitalWrite(RSTgprsPIN,LOW);
}
int printgps = 1;
int tolower(int c)
{
  if (c >= 'A' && c <= 'Z')
    c -= 'A';
    c += 'a';
}

// ****************************************************************************
// * Считать очередной доступный байт из буфера последовательного соединения  *
// * приемника GPS. Возвращаемое значение: следующий доступный байт (или -1,  *
// * если его нет). Тип данных — int.                                         *
// * Особенности: если ввести слово из 5 букв, каждый символ будет считан     *
// * отдельно, и его код из таблицы будет выведен в монитор последовательно.  *
// * Для конвертации принятого значения в символ нужно привести его к         * 
// * символьному типу — (char)Serial.read().                                  *
// ****************************************************************************
int readGps()
{
  return gpsSerial.read();
}

// ****************************************************************************
// *                            Перегрузить приемник GPS                      *
// ****************************************************************************
void reset_gps()
{
  Serial.println("Перегрузка приемника GPS");
  led_period = 1000;
  digitalWrite(RSTGPSPIN, LOW);
  delay(1000);
  digitalWrite(RSTGPSPIN, HIGH);
}

// ****************************************************************************
// *         Проверить наличие байт(символов), доступных для чтения из        *
// *                последовательного интерфейса приемника GPS                *
// ****************************************************************************
int availableGps()
{
  return gpsSerial.available();
}

// ****************************************************************************
// *         Переключить лампочку по истечении интервала мигания              *
// ****************************************************************************
unsigned long led_last_millis = 0;
int led_pin_val = 0;
void blinkLed()
{
  if (led_period == -1 || millis() < led_last_millis)
  {
    led_last_millis = millis();
    return;
  }
  if (led_last_millis + led_period < millis())
  {
    led_pin_val = 1 - led_pin_val;
    digitalWrite(LEDPIN, led_pin_val?HIGH:LOW);
    led_last_millis = millis();
  }
}

// ****************************************************************************
// *                         Принять и обработать данные GPS                  *
// ****************************************************************************
long lat = -1, lng = -1;
int had_coords = false;
int new_coords = false;
int count_same_coords = 0;
// Временная метка начала ожидания сигнала GPS
unsigned long last_data_from_gps = 0;
// Максимальное ожидание очередного сигнала GPS
#define MAXGPSDELAY 60000 // 60 секунд

void loop_gps()
{
  //Serial.println("loop_gps BEGIN");
  int c;
  // Проверяем есть ли байты(символы), доступные для чтения из      
  // последовательного интерфейса приемника GPS    
  //c=availableGps();
  //Serial.print("c=");
  //Serial.println(c);
 
  if (availableGps())
  {
    // Считываем очередной доступный байт из буфера последовательного 
    // соединения приемника GPS. 
    c = readGps();
    //Serial.print("с="); Serial.println(с);
    Serial.write(c);

    // Начинаем отсчет ожидания сигнала GPS
    //last_data_from_gps = millis();
    
    //if (!printgps)
    //{
      // Передаем символ в последовательный порт
      //Serial.write(c);
    //}
    /*
    if (gps.encode(c))
    {
      led_period = 500;
      unsigned long age;
      long old_lat = lat, old_lng = lng;
      gps.get_position(&lat, &lng, &age);
      if (old_lat == lat && old_lng == lng)
      {
        if (++count_same_coords == 300)
        {
          new_coords = 1;
          count_same_coords = 0;
        }
      }
      else
      {
        new_coords = 1;
        count_same_coords = 0;
      }
    
      if (!had_coords)
      {
        printgps = 0;
        unsigned long m = millis();
        Serial.print("\nAcquisition time: ");
        Serial.print(m);
        Serial.println(" ms");
        had_coords = true;
      }
    }
    */
  }
  /*
  else 
  {
    // Если данных GPS нет и истекло максимальное ожидание очередного сигнала 
    // GPS, то перегружаем приёмник и начинаем отсчет нового ожидания
    if (millis() - last_data_from_gps > MAXGPSDELAY)
    {
      // Перегружаем приемник GPS
      reset_gps();
      // Начинаем отсчет ожидания сигнала GPS
      last_data_from_gps = millis();
    }
  }
  */
  //Serial.println("loop_gps END");
}

int SendAT(const char * command, const char * expect)
{
  const char * e = command;
  delay(100);
  if (gsmSerial.available())
  {
    Serial.print("Ignore from SIM900: ");
    while (gsmSerial.available())
    {
      int c = gsmSerial.read();
      Serial.write(c);
    }
  }
  Serial.print("Send to SIM900: ");
  Serial.println(command);
  while (*e)
  {
    gsmSerial.write(*e);
    char buf[1];
    if (gsmSerial.readBytesUntil(0, buf, 1) != 1 || *buf != *e)
    {
      Serial.println("No command backprint");
      return false;
    }

    ++e;
  }
  --e;
    gsmSerial.print("\r\n");
    char buf[4];
    if (gsmSerial.readBytesUntil(0, buf, 4) != 4 || buf[0] != '\r' || buf[1] != '\n')
    {
      Serial.println("No command backprint");
      return false;
    }
  
  Serial.print("Recieved from SIM900: ");
  while (*expect)
  {
    char buf[1];
    if (gsmSerial.readBytesUntil(0, buf, 1) != 1)
    {
      Serial.println("Timeout reading next char...");
      return false;
    }
    int c = buf[0];
    Serial.write(c);
    if (c == '\r' || c == '\n' && *expect != c)
      continue; //skip CR and LF if unexpected

    if (c != *expect)
    {
      Serial.println("Incorrect response, abort");
      return false;
    }
    ++expect;  
  }
  Serial.println();
  gsmSerial.readBytesUntil(0, buf, 2); //Skip \r\n from the response
  return true;
}
int gsm_initialized = false;
void init_gsm()
{
  if (gsm_initialized)
    return;
  if (!SendAT("ATZ", "OK"))
    return;
  gsm_initialized = true;
}
int send_coords_at(long lat, long lng)
{

  if (!SendAT("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"", "OK"))
    return false;
  if (!SendAT("AT+SAPBR=3,1,\"APN\",\"internet.mts.ru\"", "OK"))
    return false;
  if (!SendAT("AT+SAPBR=1,1", "OK"))
  {
    SendAT("AT+SAPBR=0,1", "OK"); //close bearer
      if (!SendAT("AT+SAPBR=1,1", "OK"))
        return false;
  }
    
  if (!SendAT("AT+HTTPINIT", "OK"))
    return false;
  if (!SendAT("AT+HTTPPARA=\"CID\",1", "OK"))
    return false;    
  char url[1024];
  sprintf(url,"AT+HTTPPARA=\"URL\",\"http://gurux13.net84.net/GpsTracking/record.php?Lat=%ld&Lng=%ld\"", lat, lng);
  if (!SendAT(url, "OK"))
    return false;    
  if (!SendAT("AT+HTTPACTION=0", "OK"))
    return false;
  new_coords = 0;
  return true;
}

void stop_http()
{
  SendAT("AT+HTTPTERM", "OK");
//    SendAT("AT+SAPBR=0,1", "OK");
}
void send_coords()
{
  gsm_initialized = 0;
  init_gsm();
  stop_http();
  send_coords_at(lat, lng);

}
int stopme = false;

void loop()
{
  // Serial.println("loop_ BEGIN");
  // Переключаем лампочку по истечении интервала мигания
  blinkLed();
  //
  loop_gps();
  /*
  if (!gsm_initialized)
    init_gsm();
  if (!gsm_initialized)
  {
    power_up_gprs();
  }
  if (gsm_initialized && new_coords)
  {
    send_coords();
  }
  if (Serial.available())
  {
    int c;
    c = Serial.read();
    if (c == 'x')
      send_coords();
    else
    if (c == '!')
      stop_http();
    else
    if (c == 'p')
    {
      Serial.println("Printgps switched");
      printgps = 1 - printgps;
    }
    else
    if (c == 'g')
      reset_gps();
    else
      gsmSerial.write(c);
  }
  if (gsmSerial.available())
  {
    int c;
    c = gsmSerial.read();
    Serial.write(c);
  }
  */
  // loop_gps();
  // init_gsm();
  // Serial.println("loop_ END");
  //delay(1000);
}
