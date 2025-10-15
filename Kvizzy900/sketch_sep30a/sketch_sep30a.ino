/* ======================================================= sketch_sep30a.ino */
/*                                                                           */
/*   Выводить данные GPS на сайт - ознакомительный вариант,                  */
/*   ориентировочно - от окна гостинной:  по гармину = 61.80193,  34.32983   */
/*                                        по яндекс  = 61.802082, 34.329586  */
/*   (c) 2025 tve                                                            */
/*                                                                           */
/*   v2.0.2 2025.10.14 - 2025.08.07                                          */
/* ========================================================================= */

#include <SoftwareSerial.h>
#include "TinyGPS.h"
TinyGPS gps;

//#include <TinyGPSPlus.h>
//TinyGPSPlus gps;
SoftwareSerial gpsSerial(2,3);   // синий на 2 - будет RX; зеленый на 3 - будет TX, приемник gps-координат
SoftwareSerial gsmSerial(7,8);   // SIM900

#define LEDPIN       13          // пользовательский светодиод на 13 пине Arduino UNO
#define RSTGPSPIN    23          // 

//#define RSTgprsPIN 22

// Определяем начальный интервал мигания лампочки 2 секунды
// (до тех пор, пока не пойдут данные с приемника GPS)
int led_period = 2000;    


void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    gsmSerial.write(Serial.read()); // Forward what Serial received to Software Serial Port
  }
  while (gsmSerial.available())
  {
    Serial.write(gsmSerial.read()); // Forward what Software Serial received to Serial Port
  }
}

/**
 * Выполнить AT команду SIM900
**/ 
void sendCommand(const char* command, char* info) 
{
  // Показываем информацию по предстоящей команде
  Serial.print("--- ");
  Serial.println(info);
  // Показываем команду
  //Serial.println(command);
  //Serial.println("-----");
  // Отправляем команду SIM900
  gsmSerial.println(command);
  // Serial.flush() — функция в аппаратной платформе Arduino, которая ожидает окончания передачи исходящих данных. 
  // После вызова этой функции можно быть уверенным, что все данные отправлены, а буфер пуст. 
  // Кроме того, Serial.flush() очищает буфер приёма, сбрасывая любые входящие данные, которые ещё не были прочитаны. 
  // Это особенно полезно в ситуациях, когда входящие данные могут быть устаревшими или неактуальными. 
  // gsmSerial.flush();
  // Uncomment this delay if you need to wait a while
  delay(1000);   
  ShowSerialData();
}
/**
 * Prints the serial data, and waits 1 second
**/
void ShowSerialData() 
{
  while (gsmSerial.available()) 
  {
    char c = gsmSerial.read();
    Serial.write(c);
  }
  Serial.println("");
  delay(1000);
}


void setup()
{

  Serial.begin(9600);
  gsmSerial.begin(9600);
  delay(500);
  updateSerial();
  //gpsSerial.begin(9600);

  // Задаем максимальное время ожидания данных во время операций чтения 6 секунд
  //gsmSerial.setTimeout(6000);

  Serial.println(" ");
  Serial.println("Инициализация и ожидание 1 сек...");
  delay(1000);

  gsmSerial.println("AT"); // Handshaking with SIM900
  updateSerial();

  // Проверяем, реагирует ли модуль, ожидаемое значение  OK
  sendCommand("AT","Проверяем, реагирует ли SIM900 на команды"); 
  sendCommand("AT+CIPSHUT","Закрываем все соединения TCP/UDP"); 
  sendCommand("AT+SAPBR=2,1","Определяем состояние сессии"); 
  sendCommand("AT+SAPBR=0,1","Закрываем сессию (ответ может быть ERROR)"); 






  /*

  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW);

  //pinMode(RSTGPSPIN, OUTPUT); // вызывает проблемы с выводом на Serial !!!
  //digitalWrite(RSTGPSPIN, HIGH);  

  //pinMode(RSTgprsPIN, OUTPUT); 
  //digitalWrite(RSTgprsPIN, LOW);
  */
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
  //return gpsSerial.read();
}

// ****************************************************************************
// *                            Перегрузить приемник GPS                      *
// ****************************************************************************
void reset_gps()
{
  Serial.println("Перегрузка приемника GPS");
  /*
  led_period = 1000;
  digitalWrite(RSTGPSPIN, LOW);
  delay(1000);
  digitalWrite(RSTGPSPIN, HIGH);
  */
}

// ****************************************************************************
// *         Проверить наличие байт(символов), доступных для чтения из        *
// *                последовательного интерфейса приемника GPS                *
// ****************************************************************************
int availableGps()
{
  //return gpsSerial.available();
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

// Запрещаем показ текстовых строк, поступающих с приемника GPS
int printgps = false;
// Инициируем начальные значения широты и долготы
long lat = -1, lng = -1;
// Отмечаем, что текущие координаты с начала сеанса приема сигналов GPS
// еще не идут
int had_coords = false;
// Сбрасываем счетчик последовательно поступивших одинаковых координат
int count_same_coords = 0;
// Устанавливаем максимально контроллируемое количество поступающих одинаковых координат
int max_same_coords = 300;
// Сбрасываем флаг поступления изменённых координат
int new_coords = false;
// Временная метка начала ожидания сигнала GPS
unsigned long last_data_from_gps = 0;
// Максимальное ожидание очередного сигнала GPS
#define MAXGPSDELAY 60000 // 60 секунд

void loop_gps()
{
  /*
  //Serial.println("loop_gps BEGIN");
  int c;
  // Проверяем есть ли байты(символы), доступные для чтения из      
  // последовательного интерфейса приемника GPS    
  if (availableGps())
  {
    // Считываем очередной доступный байт из буфера последовательного 
    // соединения приемника GPS. 
    c = readGps();
    // Начинаем отсчет ожидания сигнала GPS
    last_data_from_gps = millis();
    // Если разрешено, то показываем строки, приходящие из приемника GPS
    if (printgps)
    {
      // Передаем символ в последовательный порт
      Serial.write(c);
    }
    
    // Если пошли координаты, обрабатываем данные
    if (gps.encode(c))
    {
      // Переопределяем интервал мигания лампочки в пол-секунды,
      // так как пошли данные с приемника GPS
      led_period = 500;
      // Считываем координаты и возраст текущего состояния
      unsigned long age;
      long old_lat = lat, old_lng = lng;
      gps.get_position(&lat, &lng, &age);
      Serial.print("  lat="); Serial.print(lat);
      Serial.print("  lng="); Serial.print(lng);
      Serial.print("  age="); Serial.println(age);

      // Проверяем равенство текущих и предыдущих координат
      if (old_lat == lat && old_lng == lng)
      {
        // Сбрасываем счетчик поступающих одинаковых координат и 
        // отмечаем поступление отличных от предыдущих координат
        // при превышении максимального контроллируемого значения
        if (++count_same_coords == max_same_coords)
        {
          new_coords = true;
          count_same_coords = 0;
        }
      }
      // Сбрасываем счетчик поступающих одинаковых координат и 
      // отмечаем поступление отличных от предыдущих координат
      else
      {
        new_coords = true;
        count_same_coords = 0;
      }
      
      // Отмечаем, что пошли текущие координаты с начала сеанса приема сигналов GPS
      if (!had_coords)
      {
        printgps = 0;
        unsigned long m = millis();
        Serial.print("\Время начала приема сигналов GPS: ");
        Serial.print(m);
        Serial.println(" мс");
        had_coords = true;
      }
    }
  }
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
  new_coords = false;
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
  //loop_gps();
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
