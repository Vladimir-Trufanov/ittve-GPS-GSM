/** Arduino C/C++ *************************************** AboutATcommand.ino ***
 * 
 * Познакомиться с AT-командами SIM900
 * 
 * v1.0.1, 10.11.2025                                  Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                                Дата создания: 26.07.2025
 *
**/

#include <Regexp.h>

#include <SoftwareSerial.h>

// Configure software serial port
SoftwareSerial SIM900(7,8); 


// called for each match
void match_callback  (const char * match,          // matching string (not null-terminated)
                      const unsigned int length,   // length of matching string
                      const MatchState & ms)      // MatchState in use (to get captures)
{
char cap [10];   // must be large enough to hold captures
  
  Serial.print ("Matched: ");
  Serial.write ((byte *) match, length);
  Serial.println ();
  
  for (byte i = 0; i < ms.level; i++)
    {
    Serial.print ("Capture "); 
    Serial.print (i, DEC);
    Serial.print (" = ");
    ms.GetCapture (cap, i);
    Serial.println (cap); 
    }  // end of for each capture

}  // end of match_callback 



void setup() 
{
  Serial.begin(115200);
  SIM900.begin(9600);
  Serial.println(" ");
  // AT - Рукопожатие с SIM900. "AT" – Это самая простая команда AT. Она также инициализирует передачу данных в бодах
  // Если это сработает, далее вы будете видеть эхо-сигнал символов AT, а затем OK, сообщающий вам, что все в порядке и SIM900 правильно вас понимает!
  // Проверяем, реагирует ли модуль, ожидаемое значение  OK
  sendCommand("AT","Проверяем, реагирует ли SIM900 на команды"); 
  // AT+COPS? – Убедится, что вы подключены к сети и получить информацию об операторе
  sendCommand("AT+COPS?","Убеждаемся в подключении к сети и получаем информацию об операторе"); 
  // AT+CSQ – Проверить "уровень сигнала" – первое значение это уровень сигнала в дБ, он должен быть выше 5. Чем выше, тем лучше, до 31.
  sendCommand("AT+CSQ","Проверяем уровень сигнала – первое значение это уровень сигнала в дБ, он должен быть выше 5. Чем выше, тем лучше, до 31."); 
  // AT+CCID – Получить номер SIM–карты, это проверка того, что SIM-карта найдена нормально, и вы можете проверить, какой номер присвоен карте
  sendCommand("AT+CCID","Получаем номер SIM–карты, это проверка того, что SIM-карта найдена нормально и какой она имеет номер"); 
  // AT+CREG? Проверить, зарегистрированы ли вы в сети. Второе число должно быть 1 или 5. 1 означает, что вы зарегистрированы в домашней сети,
  // а 5 - в роуминговой сети. Кроме этих двух цифр, вы не зарегистрированы ни в какой сети
  sendCommand("AT+CREG?","Проверяем регистрацию в сети. Второе число: 1 - регистрация в домашней сети, 5 - в роуминговой сети"); 
  // ATI – Получить название платы и её редакцию
  sendCommand("ATI","Получаем название платы и её редакцию"); 
  // AT+CBC – Получить состояние батареи lipo. Вторая цифра - это % заполнения, а третья цифра - фактическое напряжение в мВ
  sendCommand("AT+CBC"," Получаем состояние батареи. Вторая цифра - это % заполнения, а третья цифра - фактическое напряжение в мВ"); 
  delay(1000);   

  /*
  // match state object
  MatchState ms;

  // what we are searching (the target)
  char buf [100] = "The quick  brown fox jumps over the lazy wolf";
  ms.Target (buf);  // set its address
  Serial.println (buf);

  //char result = ms.Match ("f.x");
  char result = ms.Match ("%s%s(%a+)( )");
  
  if (result > 0)
    {
    Serial.print ("Found match at: ");
    Serial.println (ms.MatchStart);        // 16 in this case     
    Serial.print ("Match length: ");
    Serial.println (ms.MatchLength);       // 3 in this case
    }
  else
    Serial.println ("No match.");
  */  

  Serial.println ();
  unsigned long count;

  // what we are searching (the target)
  char buf [100] = "The quick brown fox jumps over the lazy wolf";

  // match state object
  MatchState ms (buf);

  // original buffer
  Serial.println (buf);

  // search for three letters followed by a space (two captures)
  count = ms.GlobalMatch ("(%a+)( )", match_callback);

  // show results
  Serial.print ("Found ");
  Serial.print (count);            // 8 in this case
  Serial.println (" matches.");
 


}

void loop() 
{
}

/**
 * Выполнить AT команду SIM900
**/ 
void sendCommand(const char* command, char* info) 
{
  // Показываем информацию по предстоящей команде
  Serial.print("--- ");
  Serial.println(info);
  // Отправляем команду SIM900
  SIM900.println(command);
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
  while (SIM900.available()) 
  {
    char c = SIM900.read();
    Serial.write(c);
  }
  Serial.println("");
  delay(1000);
}


// Arduino C/C++ **************************************** AboutATcommand.ino ***
                                                                                                                                                                                                  
