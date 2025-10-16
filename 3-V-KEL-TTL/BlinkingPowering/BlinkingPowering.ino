/*
  BlinkingPowering.ino

*/

#include "iarduino_VCC.h"              // Подключаем библиотеку для чтения напряжения питания.

// the setup function runs once when you press reset or power the board
void setup() 
{
  Serial.begin(9600);               // Инициируем передачу данных по шине UART на скорости 9600 бит/сек.
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() 
{
  float i = analogRead_VCC();       // Читаем напряжение питания Arduino.
  Serial.println( i );              // Выводим напряжение питания Arduino.

  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(200);                       // wait for a 0.2 second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);                      // wait for a second
}

