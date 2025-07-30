#include <SoftwareSerial.h>
// создание объекта
SoftwareSerial gprs(7, 8);  // RX, TX
// скорость обмена
#define GPRSbaud 9600

String str1;
char buff[100];

void setup() {
  Serial.begin(9600);
  gprs.begin(GPRSbaud);
  Serial.println("Start");
}

void loop() {
  if (Serial.available()) {
    str1 = Serial.readStringUntil('\n');
    str1.toCharArray(buffer, hh.length() + 1);
    gprs.write(buffer);
    gprs.board.write('\n');
  }
  if (gprs.available()) {
    Serial.write(gprs.read());
  }
}
