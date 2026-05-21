// testSerial.ino

// Ввести команду в строке последовательного порта,
// отправить команду модулю GSM и посмотреть ответ

#include <SoftwareSerial.h>

SoftwareSerial SIM900(7, 8);

void setup() 
{
  SIM900.begin(9600);
  Serial.begin(9600);
  Serial.println("Вводите команды, получайте ответы");
}

void loop() 
{
  if (Serial.available()) 
  {
    String command = Serial.readStringUntil('\n');
    Serial.println("=> " + command);
    SIM900.println(command);
    delay(100);
  }
  // Принимаем все загруженные строки ответа
  if (SIM900.available()) 
  {
    String response = SIM900.readString();
    Serial.println("<= " + response);
  }
}
