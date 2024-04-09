// ****************************************************** skaner-ehfira.ino ***
// *                                                                          *
// * Сканер каналов                                                           *
// * Пример обнаружения помех на различных доступных каналах. Это хороший     *
// * диагностический инструмент для проверки того, правильно ли вы выбран     *
// * канал для беспроводной передачи данных TZT: NRF24L01, NRF24L01+PA+LNA    *
// *                                                                          *
// * v1.0, 09.04.2024                              Автор:       Труфанов В.Е. *
// * Copyright © 2024 tve                          Дата создания:  09.04.2024 *
// ****************************************************************************

/*
                                                            Inspired by cpixip.
                       See http://arduino.cc/forum/index.php/topic,54795.0.html

                           Победа над nRF24L01: на три шага ближе, сканер эфира
                                               https://habr.com/ru/post/476716/
*/

/**
 * Замечания:
 * 
 * В скетче изменено время между стартом и остановкой сканирования одного канала 
 * с 128 мксек на 512 мксек. Увеличение времени позволило за один цикл сканирования 
 * всего диапазона выявлять больше источников помех и сигналов. Это равнозначно 
 * замене результата измерений в канале на сумму четырех соседних результатов 
 * в этом канале до изменения времени сканирования. При этом, время прохода всего 
 * прослушиваемого диапазона сканером увеличилось несущественно: примерно с 8 до 10 сек.
 * 
 * В разных скетчах адрес канала в командах приводится в разных форматах: 
 * в одних — ...(0x6f), в других — ...(112). Перевод с одного формата в другой 
 * станет понятным с примера перевода. Например, 
 * 
 * для (0x1а) — это: (1+1)*16 + а = (1+1)*16 + 10 = 42.
 * 
 * Отсчет каналов начинается с частоты 2,4 ГГц, далее идет увеличение частоты на 
 * 1 МГц с увеличением номера канала на 1.
 * 
**/ 

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10
RF24 radio(9, 10); //Arduino UNO

// Channel info
const uint8_t num_channels = 128;
uint8_t values[num_channels];

void setup(void)
{
  // Print preamble
  Serial.begin(9600);
  Serial.println("Scanner Air On");
  printf_begin();
  // Setup and configure rf radio
  radio.begin();
  radio.setAutoAck(false);
  // Get into standby mode
  radio.startListening();
  radio.printDetails();  
  delay(5000);              
  // Print out header, high then low digit
  int i = 0;
  while ( i < num_channels )
  {
    printf("%x", i >> 4);
    ++i;
  }
  printf("\n\r");
  i = 0;
  while ( i < num_channels )
  {
    printf("%x", i & 0xf);
    ++i;
  }
  printf("\n\r");
}

const int num_reps = 100;
void loop(void)
{
  // Clear measurement values
  memset(values, 0, sizeof(values));

  // Scan all channels num_reps times
  int rep_counter = num_reps;
  while (rep_counter--)
  {
    int i = num_channels;
    while (i--)
    {
      // Select this channel
      radio.setChannel(i);

      // Listen for a little
      radio.startListening();
      delayMicroseconds(512);
      radio.stopListening();

      // Did we get a carrier?
      if ( radio.testCarrier() )
        ++values[i];
    }
  }

  // Print out channel measurements, clamped to a single hex digit
  int i = 0;
  while ( i < num_channels )
  {
    printf("%x", min(0xf, values[i] & 0xf));
    ++i;
  }
  printf("\n\r");
}

// ****************************************************** skaner-ehfira.ino ***
