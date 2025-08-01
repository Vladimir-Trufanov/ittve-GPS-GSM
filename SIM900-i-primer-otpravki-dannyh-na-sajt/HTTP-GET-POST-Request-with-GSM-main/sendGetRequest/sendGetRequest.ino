#include <SoftwareSerial.h>

// SoftwareSerial object for communication with SIM900
SoftwareSerial gprsShield(7, 8); // RX, TX

void setup() {
  //int baudRate=19200; // change the baud rate for your GSM modem eg: 9600, 115200
  int baudRate=9600; // change the baud rate for your GSM modem eg: 9600, 115200
  gprsShield.begin(baudRate);          // gprsShield module
  delay(1000);
  Serial.begin(baudRate);              // Serial monitor
  String apn = "internet.mts.ru";      // Change this to the desired APN of cell provider
  String url = "http://google.com/";   // Change this to the desired URL

  //sendGetRequestWithParameters(apn, url);  
  Serial.println("");
  sendGetRequest();  

  // Add any additional code here if needed
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
  gprsShield.println(command);
  // Serial.flush() — функция в аппаратной платформе Arduino, которая ожидает окончания передачи исходящих данных. 
  // После вызова этой функции можно быть уверенным, что все данные отправлены, а буфер пуст. 
  // Кроме того, Serial.flush() очищает буфер приёма, сбрасывая любые входящие данные, которые ещё не были прочитаны. 
  // Это особенно полезно в ситуациях, когда входящие данные могут быть устаревшими или неактуальными. 
  // gprsShield.flush();
  // Uncomment this delay if you need to wait a while
  delay(1000);   
  ShowSerialData();
}
/**
 * Prints the serial data, and waits 1 second
**/
void ShowSerialData() 
{
  while (gprsShield.available()) 
  {
    char c = gprsShield.read();
    Serial.write(c);
  }
  Serial.println("");
  delay(1000);
}

void sendGetRequest()
{
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
  sendCommand("AT+CREG?","Проверяем регистрацию в сети GSM"); 
  sendCommand("AT+CGREG?","Проверяем регистрацию в сети передачи данных 2G"); 
  
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
  sendCommand("AT+SAPBR=2,1","Определяем состояние сессии"); 

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
  //sendCommand("AT+HTTPINIT","Инициализируем сервис HTTP"); // initiate HTTP request

  sendCommand("AT+GMR","AT+GMR"); 
  sendCommand("AT+HTTPSSL=?","AT+HTTPSSL=?"); 
  sendCommand("AT+HTTPSSL?","AT+HTTPSSL?"); 
  sendCommand("AT+HTTPSSL=1","AT+HTTPSSL=1"); 
  sendCommand("AT+HTTPSSL?","AT+HTTPSSL?"); 

  /*
  // AT+HTTPSSL=1 // Включить протокол SSL для HTTP
  sendCommand("AT+HTTPSSL=1","Включить протокол SSL для HTTP"); 
  
  // Задаём идентификатор профиля сеанса
  sendCommand("AT+HTTPPARA=\"CID\",1","Задаём идентификатор профиля сеанса"); // set parameters for http session

  // Задаём URL сайта, к которому будет отправляться запрос HTTP GET.
  // sendCommand("AT+HTTPPARA=\"URL\",\"http://google.com/\"","Задаём URL сайта http://google.com/"); // Change the URL from google.com to the server you want to reach
  // sendCommand("AT+HTTPPARA=\"URL\",\"http://my-json-server.typicode.com/typicode/demo/posts\"","Задаём URL демонстрационного сервера");
  sendCommand("AT+HTTPPARA=\"URL\",\"https://probatv.ru/\"","Задаём URL сайта https://probatv.ru/"); // Change the URL from google.com to the server you want to reach
  // sendCommand("AT+HTTPPARA=\"URL\",\"http://87.242.70.183/\"","Задаём URL сайта https://probatv.ru/"); // Change the URL from google.com to the server you want to reach
  
  //http://probatv.ru/
  //https://probatv.ru/

  // Вводим команду для выполнения запроса GET: AT+HTTPACTION=0.
  // Параметр команды AT+HTTPACTION задает тип запроса HTTP: 0 — GET, 1 — POST, 2 — HEAD, 3 — DELETE.
  // В нашем случае нулевое значение предписывает выполнить запрос GET.
  // +HTTPACTION: 0,200,134
  sendCommand("AT+HTTPACTION=0","Вводим команду для выполнения запроса GET"); // send http request to specified URL, GET session start

  Serial.println("Ждем 3 сек, чтобы запросить ответ");
  delay(3000); 
  
  // Cчитываем результаты запроса, обычно содержит код состояния 200 в случае успеха
  sendCommand("AT+HTTPREAD", "Cчитываем результаты запроса, обычно содержит код состояния 200");
  */

  /*
  sendCommand("AT+HTTPTERM");//close http connection
  sendCommand("AT+CIPSHUT");//close or turn off network connection
  sendCommand("AT+SAPBR=0,1");// close GPRS context bearer
  */

}




void loop() {
  // Add your main program logic here if needed
}







/**
 * The sequence of commands establishes the GPRS connection for performing an HTTP GET request
 * @param apn The Access Point Name for the SIM card's cellular network.
 * @param url The URL of the server you want to reach.
 */
void sendGetRequestWithParameters(const String& apn, const String& url) 
{
  /*
  sendCommand("AT"); //expected value OK
  sendCommand("AT+CIPSHUT"); //expected value OK
  sendCommand("AT+SAPBR=0,1"); //expected value OK
  sendCommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\""); // open GPRS context establish GPRS connection
  
  // Change the APN value for the SIM card based on the provided parameter
  String sapbr_apn_command = "AT+SAPBR=3,1,\"APN\",\"" + apn + "\"";
  sendCommand(sapbr_apn_command.c_str());
  
  sendCommand("AT+SAPBR=1,1"); // open GPRS context bearer
  sendCommand("AT+HTTPINIT"); // initiate HTTP request
  sendCommand("AT+HTTPPARA=\"CID\",1"); // set parameters for http session
  
  // Change the URL to the provided parameter
  String http_url_command = "AT+HTTPPARA=\"URL\",\"" + url + "\"";
  sendCommand(http_url_command.c_str());
  
  sendCommand("AT+HTTPACTION=0"); // send http request to specified URL, GET session start
  delay(9000); // wait for response for 9 seconds, reduce or increase based on your need
  sendCommand("AT+HTTPREAD"); // read results of request, normally contains status code 200 if successful
  sendCommand("AT+HTTPTERM"); // close http connection
  sendCommand("AT+CIPSHUT"); // close or turn off network connection
  sendCommand("AT+SAPBR=0,1"); // close GPRS context bearer
  */
}

/**
 * The sequence of commands establishes the GPRS connection for performing an HTTP request
*/

/*
void sendGetRequestOrig()
{

  sendCommand("AT"); //expected value OK
  sendCommand("AT+CIPSHUT"); //expected value OK
  sendCommand("AT+SAPBR=0,1"); //expected value OK
  sendCommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");// open GPRS context establish GPRS connection
  sendCommand("AT+SAPBR=3,1,\"APN\",\"internet.mtn\"");//change this apn value for the SIM card
  sendCommand("AT+SAPBR=1,1");//open GPRS context bearer
  sendCommand("AT+HTTPINIT");//initiate HTTP request
  sendCommand("AT+HTTPPARA=\"CID\",1");//set parameters for http session
  sendCommand("AT+HTTPPARA=\"URL\",\"http://google.com/\""); //Change the URL from google.com to the server you want to reach
  sendCommand("AT+HTTPACTION=0");//send http request to specified URL, GET session start
  delay(9000); //wait for response for 9 seconds, reduce or increase based on your need
  sendCommand("AT+HTTPREAD");// read results of request, normally contains status code 200 if successful
  sendCommand("AT+HTTPTERM");//close http connection
  sendCommand("AT+CIPSHUT");//close or turn off network connection
  sendCommand("AT+SAPBR=0,1");// close GPRS context bearer
  

}
*/

