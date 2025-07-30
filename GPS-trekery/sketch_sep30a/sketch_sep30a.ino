#include <SoftwareSerial.h>

#include "TinyGPS.h"

TinyGPS gps;
//SoftwareSerial gsmSerial(2,8);
//SoftwareSerial gpsSerial(3, 3);
#define gsmSerial Serial1
#define gpsSerial Serial2
#define LEDPIN 13
#define RSTGPRSPIN 22
#define RSTGPSPIN 23
int led_period = 2000;
void setup()
{
  Serial.begin(19200); //USB
  gsmSerial.begin(19200); //GSM
  gsmSerial.setTimeout(6000);
  gpsSerial.begin(9600);
  pinMode(LEDPIN, OUTPUT);
  pinMode(RSTGPRSPIN, OUTPUT); 
  pinMode(RSTGPSPIN, OUTPUT); 
  digitalWrite(LEDPIN, LOW);
  digitalWrite(RSTGPRSPIN, LOW);
  digitalWrite(RSTGPSPIN, HIGH);  
//  gpsSerial.begin(9600);
//  gsmSerial.begin(19200);
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
 digitalWrite(RSTGPRSPIN,HIGH);
 delay(2000);
 digitalWrite(RSTGPRSPIN,LOW);
}
int printgps = 1;
int tolower(int c)
{
  if (c >= 'A' && c <= 'Z')
    c -= 'A';
    c += 'a';
}
int readGps()
{
  return gpsSerial.read();
}
void reset_gps()
{
  Serial.println("Resetting gps");
  led_period = 1000;
  digitalWrite(RSTGPSPIN, LOW);
  delay(1000);
  digitalWrite(RSTGPSPIN, HIGH);
}
int availableGps()
{
  return gpsSerial.available();
}

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

long lat = -1, lng = -1;
int had_coords = false;
int new_coords = false;
int count_same_coords = 0;
unsigned long last_data_from_gps = 0;
#define MAXGPSDELAY 60000
void loop_gps()
{
  int c;
  if (availableGps())
  {
    c = readGps();
    last_data_from_gps = millis();
    if (!printgps)
    {
      Serial.write(c);
    }
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
  }
  else 
  {
    if (millis() - last_data_from_gps > MAXGPSDELAY)
    {
      reset_gps();
      last_data_from_gps = millis();
    }
  }
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
  blinkLed();
  loop_gps();
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
  //loop_gps();
//  init_gsm();
}
