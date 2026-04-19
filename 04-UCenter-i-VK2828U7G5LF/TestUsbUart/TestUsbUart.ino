/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://docs.arduino.cc/hardware/

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://docs.arduino.cc/built-in-examples/basics/Blink/

Для примера загрузим на плату код, выводящий таблицу ASCII. 
ASCII представляет собой кодировку для представления десятичных цифр, 
латинского и национального алфавитов, знаков препинания 
и управляющих символов.


*/
int symbol = 33;

// the setup function runs once when you press reset or power the board
void setup() 
{
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("ASCII Table ~ Character Map");
}

// the loop function runs over and over again forever
void loop() 
{
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  symtbl();
  delay(1000);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);                      // wait for a second
  symtbl();
}

void symtbl() 
{
Serial.write(symbol);
  Serial.print(", dec: ");
  Serial.print(symbol);
  Serial.print(", hex: ");
  Serial.print(symbol, HEX);
  Serial.print(", oct: ");
  Serial.print(symbol, OCT);
  Serial.print(", bin: ");
  Serial.println(symbol, BIN);
  if(symbol == 126) {
    while(true) {
      continue;
    }
  }
  symbol++;  
}

