/* ========================================================================== */
/*                                                                            */
/*   gps-treker-dlya-avto-s-otpravkoj-dannyh-na-server.ino                    */
/*   (c) 2012 Author                                                          */
/*                                                                            */
/* v2.0.5, 2025-08-04 - 2025-08-07 ----Отлаживаю включение SIM900 на работу       */
/* ========================================================================== */

// Трекер читает данные по протоколу NMEA с TX пина от GPS V.KEL 16. Уровень — 5V, скорость — 9600 бод. 
// Для подключения к Arduino используем программный UART на пинах 2 и 3. Для чтения данных используем библиотеку TinyGPS. 

// Для получения страницы по определенному URL нужно послать следующие команды:
/*
AT+SAPBR=1,1                             // Открыть несущую (Carrier)
AT+SAPBR=3,1,"CONTYPE","GPRS"            // тип подключения - GPRS
AT+SAPBR=3,1,"APN","internet.mts.ru"     // APN, для МТС: internet.mts.ru

AT+HTTPINIT                              // Инициализировать HTTP 
AT+HTTPPARA="CID",1                      // Carrier ID для использования.
AT+HTTPPARA="URL","http:/????????.ru/gps_tracker/gps_tracker1.php?id_avto=?N&lat=XXXXXlon=YYYYY"    // Собственно URL, после sprintf с координатами
AT+HTTPACTION=0                          // Запросить данные методом GET
AT+HTTPREAD                              // дождаться ответа
AT+HTTPTERM                              // остановить HTTP
*/

// Предусмотрим еще один момент - при написании серверной части и web-интерфейса реализовано получение и вывод результатов для нескольких модулей 
// (id_avto=N), поставлен переключатель на 3 позиции, что позволит получать информацию от 8 транспортных средств. 

// Чтобы не отправлять данные во время стоянки транспортного средства, отправлять будем данные, только если GPS дынные будут изменяться на значение,
// указанное в константе MINCANGE.
// Интервал отправки данных - константа INTERVALSEND мсек. Наличие двух активных программных последовательных портов приводит к ошибкам получения/отправки данных, 
// поэтому приходится переключать  состояние программных последовательных портов для работы с каждым шилдом: GPRS.end(); gpsSerial.begin(9600);

// В процедуре setup() - выбор номера модуля для отправки данных на сервер (id_avto=) - считывается их трехпереключателей

#define INTERVALSEND 10000 //30000
#define MINCHANGE 1 // 100
 
#include "SoftwareSerial.h"
#include "SayMessage.h"

SoftwareSerial GPRS(7, 8);
char aux_str[150];            // буфер формирования AT-команды
char aux;
char data[512];
int data_size;
uint8_t answer=0;
 
 
#include "TinyGPS.h"
TinyGPS gps;
//Tx, Rx 
SoftwareSerial gpsSerial(2, 3);
long lat, lon;
long endlatsend=0; 
long endlonsend;
unsigned long time1,date1;
unsigned long age;
bool newdata = false;
unsigned long millis1=0;
unsigned long millissend=0;
unsigned long millisdata=0;
 
 
char apn[]="internet.mts.ru";
char url[150];
// String surl="http://yoursite/gps_tracker/gps_tracker1.php?id_avto=";
String surl="http://probatv.ru/";
int id_avto=1;                    // идентификатор авто (одного из восьми)
 
void setup()
{
  GPRS.begin(9600);               // the GPRS baud rate   
  Serial.begin(9600);             // the Serial port of Arduino baud rate.
  //gpsSerial.begin(9600);
  Serial.println(" ");
  Serial.println("Starting...");
   
  pinMode(12,OUTPUT);
  digitalWrite(12,LOW);
   
  power_on();
  delay(3000);
  IniSendOnGPRS();
  Serial.println("-------------------------- сессия1 ------");
  SendOnGPRS();
  Serial.println("-------------------------- сессия2 ------");
  SendOnGPRS();
  Serial.println("-------------------------- сессия ------");
  SendOnGPRS();
  Serial.println("-------------------------- все     ------");
  
  /*
  sendATcommand("AT", "OK", 2000);
  delay(3000);

  sendATcommand("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"", "OK", 2000);

  // sets APN, user name and password: AT+SAPBR=3,1,"APN","internet.mts.ru"

  // snprintf — функция в C++ для форматирования строк. Она позволяет создавать строки с контролем формата, но записывает результат в буфер, 
  // вместо того чтобы вывести строку в стандартный вывод. 
  // Синтаксис: int snprintf(char* buffer, size_t buf_size, const char* format, ...). 
  // Параметры: buffer — указатель на строковый буфер для записи результата. buf_size — максимальное количество символов, которые могут быть записаны 
  // в буфер (равна buf_size-1). 
  // format — указатель на строку, заканчивающуюся нулём, которая записывается в буфер. Состоит из символов вместе с необязательными спецификаторами формата, 
  // начинающимися с %. Дополнительные аргументы — определяют данные для печати, их количество зависит от используемых спецификаторов формата в строке формата. 
  // Принцип работы: функция записывает строку, на которую указывает параметр format, в буфер. Если результирующая строка будет длиннее, чем buf_size-1 символов, 
  // оставшиеся символы отбрасываются и не хранятся, но учитываются для значения, возвращаемого функцией. 
  // После записи символов автоматически добавляется завершающий нулевой символ. Если buf_size равно нулю, ничего не записывается, и buffer может быть нулевым указателем. 
  snprintf(aux_str, sizeof(aux_str), "AT+SAPBR=3,1,\"APN\",\"%s\"", apn);
  sendATcommand(aux_str, "OK", 2000);

  // Отключил контроль на OK
  //while (sendATcommand("AT+SAPBR=1,1", "OK", 2000) == 0)
  //{
  //  delay(2000);
  //}
  sendATcommand("AT+SAPBR=1,1", "OK", 2000);
  delay(1000);
  */
  
  Serial.println("Завершен Setup");
}
 
void loop() 
{
  /*
  digitalWrite(12,HIGH);
  GPRS.end();

  
  gpsSerial.begin(9600);
  while (millis() - millis1 < 2000) 
  {
    if (readgps()) newdata = true; 
  }   
  if (newdata) 
  {
    gps.get_position(&lat, &lon, &age);
    millisdata=millis();
    Serial.print("lat=");Serial.print(lat);
    Serial.print("  lon=");Serial.print(lon);     
    gps.get_datetime(&date1, &time1, &age);
    Serial.print(" date=");Serial.print(date1);
    Serial.print("  time=");Serial.println(time1);     
    newdata=false;
  }
  // if(millis()-millissend>INTERVALSEND && millis()-millisdata<INTERVALSEND
  // && abs(lat-endlatsend)>MINCHANGE && abs(lon-endlonsend)>MINCHANGE)
  
  if(millis()-millissend>INTERVALSEND && millis()-millisdata<INTERVALSEND
  / *&& abs(lat-endlatsend)>MINCHANGE && abs(lon-endlonsend)>MINCHANGE* /)
  {


    GPRS.begin(9600);
    gpsSerial.end();
    // Initializes HTTP service
    answer = sendATcommand("AT+HTTPINIT", "OK", 10000);
    if (answer == 1)
    {
      // Sets CID parameter
      answer = sendATcommand("AT+HTTPPARA=\"CID\",1", "OK", 5000);
      if (answer == 1)
      {// Sets url 
        endlatsend=lat; endlonsend=lon;
        
        / *
        String surl1=set_url_avto()+"&lat="+String(lat)+"&lon="+String(lon);
        surl1+="&date="+String(date1)+"&time="+String(time1);
        surl1.toCharArray(url,surl1.length()+1);
        * /
        String surl1=set_url_avto();
        surl1.toCharArray(url,surl1.length()+1);

        snprintf(aux_str, sizeof(aux_str), "AT+HTTPPARA=\"URL\",\"%s\"", url);
        answer = sendATcommand(aux_str, "OK", 5000);
        if (answer == 1)
        {// Starts GET action
          answer = sendATcommand("AT+HTTPACTION=0", "+HTTPACTION:0,200", 10000);
          if (answer == 1)
          {
            sprintf(aux_str, "AT+HTTPREAD");
            sendATcommand(aux_str, "OK", 5000);
          }
          else
          {
            Serial.println("Error getting the url");
          }
        }
        else
        {
          Serial.println("Error setting the url");
        }
      }
      else
      {
        Serial.println("Error setting the CID");
      }    
    }
    else
    {
      Serial.println("Error initializating");
    }
    sendATcommand("AT+HTTPTERM", "OK", 5000);
    millissend=millis();


  }
  else Serial.println("data not change!!!");

  millis1=millis();
  GPRS.begin(9600);
  gpsSerial.end();
  */
}

// отправка AT-команд
uint8_t sendATcommand(char* ATcommand, char* expected_answer, unsigned int timeout)
{
  uint8_t x=0;                                 // позиция в заполняемом буфере ответа GPRS (SIM900)
  uint8_t answer=0;                            // возвращаемый ответ
  char response[150];                          // буфер ответа GPRS
  unsigned long previous;
 
  memset(response, '\0', 150);                 // Initialize the string
  delay(100);
  while (GPRS.available() > 0) GPRS.read();    // Clean the input buffer
  GPRS.println(ATcommand);                     // Send the AT command 
  x = 0;
  previous = millis();
  // Циклимся, пока не выберем весь ответ
  do
  {
    if (GPRS.available() != 0)
    {    
      // if there are data in the UART input buffer, reads it and checks for the asnwer
      response[x] = GPRS.read();
      //Serial.print(response[x]);
      x++;
      // check if the desired answer  is in the response of the module
      // Функция strstr в C++ используется для поиска первого вхождения подстроки в строке. Она определена в стандартной библиотеке C, поэтому доступна и в C++.
      // Параметры: str1 — строка, в которой выполняется поиск; str2 — подстрока для поиска в строке str1. 
      // Возвращаемое значение: указатель на первое вхождение подстроки str2 в строке str1; нулевой указатель (nullptr), если подстрока не найдена.
      // Важно: функция чувствительна к регистру — например, поиск по «hello» не соответствует «Hello». 
      // Синтаксис: char* strstr(const char* str1, const char* str2). 
      // Чтобы использовать strstr, в начале кода нужно включить заголовочный файл <cstring> или <string.h> — это гарантирует, что компилятор распознаёт функцию. 
      if (strstr(response, expected_answer) != NULL)    
      {
        answer = 1;
      }
    }
  }
  // Waits for the asnwer with time out
  while((answer == 0) && ((millis() - previous) < timeout));    
  
  Serial.println(response);
  return answer;
}
// ****************************************************************************
// *                     Отправить текущие координаты по GPRS                 *
// ****************************************************************************
void IniSendOnGPRS()

// Для получения страницы по определенному URL нужно послать следующие команды:
/*
AT+SAPBR=1,1                             // Открыть несущую (Carrier)
AT+SAPBR=3,1,"CONTYPE","GPRS"            // тип подключения - GPRS
AT+SAPBR=3,1,"APN","internet.mts.ru"     // APN, для МТС: internet.mts.ru

AT+HTTPINIT                              // Инициализировать HTTP 
AT+HTTPPARA="CID",1                      // Carrier ID для использования.
AT+HTTPPARA="URL","http:/????????.ru/gps_tracker/gps_tracker1.php?id_avto=?N&lat=XXXXXlon=YYYYY"    // Собственно URL, после sprintf с координатами
AT+HTTPACTION=0                          // Запросить данные методом GET
AT+HTTPREAD                              // дождаться ответа
AT+HTTPTERM                              // остановить HTTP
*/
{

  /*
  // Проверяем, реагирует ли модуль, ожидаемое значение  OK
  sendCommand("AT","Проверяем, реагирует ли SIM900 на команды"); 

  // https://stackoverflow.com/questions/63187583/arduino-sim900-atsapbr-1-1-operation-not-allowed
  // Для уверенности в работе SIM900 у провайдера запрашиваем статус регистрации. 
  // Это можно сделать отдельно для сети GSM (AT+CREG?) и для сети передачи данных 2G (AT+CGREG?).
  // В обоих случаях запрос к команде приведёт к такому ответу:
  // +CGREG: <n>,<stat> (или +CREG: <n>,<stat>), где
  // <n> — это настройка, выполняемая с помощью команды set. Она используется для включения нежелательных сообщений о результатах. 
  // Поэтому её значение обычно равно 0;
  // <stat> — текущий статус регистрации. Он может принимать следующие значения
  // 0 — не зарегистрирован. Служба GPRS отключена; 1 — зарегистрировано, домашняя сеть;
  // 2 — не зарегистрирован, но в данный момент SIM900 пытается подключиться или ищет оператора для регистрации. Служба GPRS включена;
  // 3 — в регистрации отказано. Служба GPRS отключена; 4 - Неизвестно; 5 — зарегистрирован, в роуминге.
  // Итак, если вы уверены, что устройство находится в зоне действия сигнала, вам нужно всего лишь каждую секунду 
  // вводить команду AT+CGREG? и ждать +CGREG=0,1 (или +CGREG=0,5 , если вы работаете в роуминге).
  //sendCommand("AT+CREG?","Проверяем регистрацию в сети GSM"); 
  //sendCommand("AT+CGREG?","Проверяем регистрацию в сети передачи данных 2G"); 
  
  // Закрываем все соединения TCP/UDP, которые могли быть открыты на модуле. 
  // Использование этой команды может быть полезно, например, при переустановке 
  // соединения или для освобождения ресурсов, занятых предыдущими соединениями. 
  sendCommand("AT+CIPSHUT","Закрываем все соединения TCP/UDP"); 

  // "AT+SAPBR=2,1" - "Получить состояние сессии". 
  // Команда AT+SAPBR применяется для управления параметрами подключения к GPRS. 
  // Первое число в ней указывает тип команды: 0 — закрыть сессию, 1 — открыть сессию, 
  // 2 — получить состояние сессии, 3 — задать параметры сессии, 4 — получить параметры сессии. 
  // Ответ при получении состояния сессии вида +SAPBR: 1,3,"0.0.0.0", где 
  // первое число в ответе этой команды — состояние сессии: 0 — сессия не открыта; 1 — сессия активна (открыта);
  // 2 — сессия открыта (в процессе установки); 3 — сессия закрыта (в процессе разрыва),
  // второе число — тип подключения: 0 — нет подключения; 1 — активное подключение;
  // 2 — пассивное подключение; 3 — GPRS.
  // Далее в ответе команды идет адрес IP, выделенный в рамках сессии.
  // В нашем случае сессия открыта, ее тип — GPRS, но адрес IP не выделен. Это означает, что подключение к GPRS еще не выполнено.
  // sendCommand("AT+SAPBR=2,1","Определяем состояние сессии"); 

  // Закрываем носитель контекста GPRS (сессию), если он был оставлен открытым
  // (команда AT+SAPBR=0,1 используется для закрытия сессии и соединения TCP/IP, 
  // чтобы не расходовать ресурсы мобильного провайдера. 
  sendCommand("AT+SAPBR=0,1","Закрываем сессию (ответ может быть ERROR)"); 

  // Открываем контекст GPRS и устанавливаем GPRS-соединение
  sendCommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"","Открываем контекст GPRS и устанавливаем GPRS-соединение");

  // Определяем имя точки доступа Access Point Name (APN) — как “internet.mts.ru”.
  sendCommand("AT+SAPBR=3,1,\"APN\",\"internet.mts.ru\"","Определяем имя точки доступа APN, как МТС"); // change this apn value for the SIM card

  // Устанавливаем соединение для профиля с идентификатором 1.
  // Когда соединение будет установлено, можно получить параметры сеанса с помощью 
  // рассмотренной выше команды AT+SAPBR=2,1. Однако теперь эта команда вернет адрес 
  // IP из внутренней сети мобильного провайдера   
  sendCommand("AT+SAPBR=1,1","Устанавливаем соединение для профиля с идентификатором 1"); // open GPRS context bearer
  sendCommand("AT+SAPBR=2,1","Снова получаем состояние сессии c IP-адресом"); 
  // Инициализируем сервис HTTP 
  sendCommand("AT+HTTPINIT","Инициализируем сервис HTTP"); // initiate HTTP request

  // Задаём идентификатор профиля сеанса
  sendCommand("AT+HTTPPARA=\"CID\",1","Задаём идентификатор профиля сеанса"); // set parameters for http session

  // Задаём URL сайта, к которому будет отправляться запрос HTTP GET.
  // sendCommand("AT+HTTPPARA=\"URL\",\"http://google.com/\"","Задаём URL сайта http://google.com/"); // Change the URL from google.com to the server you want to reach
  // sendCommand("AT+HTTPPARA=\"URL\",\"http://my-json-server.typicode.com/typicode/demo/posts\"","Задаём URL демонстрационного сервера");
     sendCommand("AT+HTTPPARA=\"URL\",\"http://probatv.ru/\"","Задаём URL сайта http://probatv.ru/"); // Change the URL from google.com to the server you want to reach
  // sendCommand("AT+HTTPPARA=\"URL\",\"http://87.242.70.183/\"","Задаём URL сайта https://probatv.ru/"); // Change the URL from google.com to the server you want to reach

  // xn--http://-2hga2ewc.xn--p1ai
  // sendCommand("AT+HTTPPARA=\"URL\",\"xn--http://-2hga2ewc.xn--p1ai\"","Задаём URL сайта http://пифи.рф"); // Change the URL from google.com to the server you want to reach
  // sendCommand("AT+HTTPPARA=\"URL\",\"http://пифи.рф/\"","Задаём URL сайта http://пифи.рф/"); // Change the URL from google.com to the server you want to reach
  
  //http://probatv.ru/
  //https://probatv.ru/

  // Вводим команду для выполнения запроса GET: AT+HTTPACTION=0.
  // Параметр команды AT+HTTPACTION задает тип запроса HTTP: 0 — GET, 1 — POST, 2 — HEAD, 3 — DELETE.
  // В нашем случае нулевое значение предписывает выполнить запрос GET.
  // +HTTPACTION: 0,200,134
  sendCommand("AT+HTTPACTION=0","Вводим команду для выполнения запроса GET"); // send http request to specified URL, GET session start

  Serial.println("Ждем 3 сек, чтобы запросить ответ");
  delay(9000); 
  
  // Cчитываем результаты запроса, обычно содержит код состояния 200 в случае успеха
  sendCommand("AT+HTTPREAD", "Cчитываем результаты запроса, обычно содержит код состояния 200");

  //sendCommand("AT+HTTPTERM");//close http connection
  //sendCommand("AT+CIPSHUT");//close or turn off network connection
  //sendCommand("AT+SAPBR=0,1");// close GPRS context bearer
  */


  // Проверяем, реагирует ли модуль AT-команды
  sendATcommand("AT","OK",2000);
  // Открываем контекст GPRS и устанавливаем GPRS-соединение
  sendATcommand("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"","OK",2000);
  // Определяем точку доступа в интернет: Access Point Name (APN) = “internet.mts.ru”
  // (snprintf — функция в C++ для форматирования строк. Она позволяет создавать строки с контролем формата, но записывает результат в буфер, 
  // вместо того чтобы вывести строку в стандартный вывод. 
  // Синтаксис: int snprintf(char* buffer, size_t buf_size, const char* format, ...). 
  // Параметры: buffer — указатель на строковый буфер для записи результата. buf_size — максимальное количество символов, которые могут быть записаны 
  // в буфер (равна buf_size-1). 
  // format — указатель на строку, заканчивающуюся нулём, которая записывается в буфер. Состоит из символов вместе с необязательными спецификаторами формата, 
  // начинающимися с %. Дополнительные аргументы — определяют данные для печати, их количество зависит от используемых спецификаторов формата в строке формата. 
  // Принцип работы: функция записывает строку, на которую указывает параметр format, в буфер. Если результирующая строка будет длиннее, чем buf_size-1 символов, 
  // оставшиеся символы отбрасываются и не хранятся, но учитываются для значения, возвращаемого функцией. 
  // После записи символов автоматически добавляется завершающий нулевой символ. Если buf_size равно нулю, ничего не записывается, и buffer может быть нулевым указателем)
  snprintf(aux_str,sizeof(aux_str),"AT+SAPBR=3,1,\"APN\",\"%s\"",apn);
  sendATcommand(aux_str,"OK",2000);
  // Устанавливаем соединение для профиля с идентификатором 1.
  // Когда соединение будет установлено, можно получить параметры сеанса с помощью 
  // команды AT+SAPBR=2,1. Однако теперь эта команда вернет адрес 
  // IP из внутренней сети мобильного провайдера   
  while (sendATcommand("AT+SAPBR=1,1", "OK", 2000) == 0)
  {
    delay(2000);
  }
  delay(1000);
}

void SendOnGPRS()
{
  uint8_t answer=0;                            // возвращаемый ответ
  // Инициализируем сервис HTTP 
  answer = sendATcommand("AT+HTTPINIT","OK",10000);
  if (answer == 1)
  {
    // Serial.println("AT+HTTPINIT сработало");
    // Задаём идентификатор профиля сеанса
    answer = sendATcommand("AT+HTTPPARA=\"CID\",1","OK",5000);
    if (answer == 1)
    {
      //Serial.println("Идентификатор профиля CID=1 установлен");
      /*
      //
      // Sets url 
      endlatsend=lat; endlonsend=lon;
        
      //String surl1=set_url_avto()+"&lat="+String(lat)+"&lon="+String(lon);
      //surl1+="&date="+String(date1)+"&time="+String(time1);
      //surl1.toCharArray(url,surl1.length()+1);
      String surl1=set_url_avto();
      surl1.toCharArray(url,surl1.length()+1);
      snprintf(aux_str, sizeof(aux_str), "AT+HTTPPARA=\"URL\",\"%s\"", url);
      answer = sendATcommand(aux_str, "OK", 5000);
      */
      // Задаём URL сайта, к которому будет отправляться запрос HTTP GET
      answer = sendATcommand("AT+HTTPPARA=\"URL\",\"http://probatv.ru/\"","OK",5000); 
      if (answer == 1)
      {
        //Serial.println("URL сайта для запроса HTTP GET отправлен");
        // Отправляем команду для выполнения запроса GET: AT+HTTPACTION=0:
        // параметр команды AT+HTTPACTION задает тип запроса HTTP: 0 — GET, 1 — POST, 2 — HEAD, 3 — DELETE
        answer = sendATcommand("AT+HTTPACTION=0", "+HTTPACTION:0,200", 10000);
        if (answer == 1)
        {
          //Serial.println("Успешно отправлен запрос GET");
          sprintf(aux_str, "AT+HTTPREAD");
          answer = sendATcommand(aux_str,"OK",5000);
          if (answer == 1) Serial.println("Ответ получен");
          else             Serial.println("Ответа НЕТ!!!");

        }
        else
        {
          Serial.println("Ошибка выполнения запроса GET");
        }
      }
      else
      {
        Serial.println("Ошибка отправки URL сайта для запроса HTTP GET");
      }
    }
    else
    {
      Serial.println("Идентификатор профиля CID=1 НЕ УСТАНОВЛЕН!");
    }
    // Завершаем HTTP
    sendATcommand("AT+HTTPTERM", "OK", 5000);
  }
  else Serial.println("AT+HTTPINIT НЕТ!!!");

}


// ****************************************************************************
// *            Выполнить программное включение модуля GPRS (2G)              *
// ****************************************************************************
int onModulePin=9;    // пин включения модуля
// Выполнить очередную попытку включения модуля GPRS
uint8_t _power_on()
{
  uint8_t answer=0;   // состояние ответа на команду - возвращаемый результат
  // Выполняем контрольное выключение модуля GPRS
  sendATcommand("AT+CPOWD=1","OK",2000);
  // Выполняем контрольные тики включения-выключения
  // и посылаем контрольную AT-команду
  digitalWrite(onModulePin,LOW);
  delay(1000);
  digitalWrite(onModulePin,HIGH);
  delay(2000);
  digitalWrite(onModulePin,LOW);
  delay(3000);
  answer = sendATcommand("AT","OK",2000);  
  delay(1000); 
  return answer;
}
// Выполнить программное включение модуля GPRS (2G)
void power_on()
{
  uint8_t nCycle=0;            // Счетчик попыток включения модуля
  uint8_t answer=0;            // Состояние ответа на команду - возвращаемый результат
  // Настраиваем 9 пин на включение
  pinMode(onModulePin,OUTPUT);
  // "Выполняем включение модуля GPRS"
  sayln(vypolnyaem_vklyuchenie_modulya_gprs);
  // Выполняем и трассируем попытки включить GPRS
  while (answer == 0)
  {  
    answer=_power_on();
    nCycle=nCycle+1; 
    // Отмечаем "Не включается GPRS/нет питания"
    if (answer<1) 
    {
      say(ne_vklyuchaetsya_gprs_net_pitaniya);
      say(" ["); say(String(nCycle)); say("]");
    }
  }
  // Отмечаем включение модуля
  if (answer==1) sayln(modul_gprs_rabotaet);
  // Вставка для отладки поведения модуля 2025-08-05
  // sendATcommand("AT+CPOWD=1","OK",2000);
}

// проверка наличия данных gps
bool readgps() 
{
  while (gpsSerial.available())
  {
    int b = gpsSerial.read();
    // в TinyGPS есть баг, когда не обрабатываются данные с \r и \n
    if('\r' != b) 
    {
      if (gps.encode(b)) return true;
    }
  }
  return false;
}
   
String set_url_avto()
{
  String surl1;
  /*
  if(digitalRead(14)==1) id_avto=id_avto+1;
  if(digitalRead(15)==1) id_avto=id_avto+2;
  if(digitalRead(16)==1) id_avto=id_avto+4;
  */
  id_avto=5;
  //surl1=surl+String(id_avto);
  //surl1=surl+String(id_avto);
  surl1=surl;
  //Serial.print("surl1="); Serial.println(surl1); 
  id_avto=0;
  return surl1; 
}
  
