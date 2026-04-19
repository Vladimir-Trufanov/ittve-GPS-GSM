/** Arduino C/C++ ************************************* Ex1_Talk_SIM900.ino ***
 *
 * 1 пример из набора примеров "Несколько SoftwareSerial на Ардуино UNO"
 * 
 * Здесь через программный последовательный порт на пинах 7 = RX, 8 - TX, 
 * производится отправка AT-команд на SIM900.
 * 
 * v1.2.0, 16.10.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 26.07.2025
 *
**/

#include <SoftwareSerial.h>

SoftwareSerial SIM900(7,8); // SIM900 

void setup()
{
  Serial.begin(115200);
  SIM900.begin(9600);
  delay(500);
  update_SIM900();
  Talk_SIM900();
}
 
void loop()
{
  update_SIM900();
}

// ****************************************************************************
// *               Выбрать ответное сообщение из буфера SIM900                *
// ****************************************************************************
void update_SIM900()
{
  delay(500);
  while (Serial.available())
  {
    SIM900.write(Serial.read()); // Forward what Serial received to Software Serial Port
  }
  while (SIM900.available())
  {
    Serial.write(SIM900.read()); // Forward what Software Serial received to Serial Port
  }
}
// ****************************************************************************
// *                   Отправить набор AT-команд на SIM900                    *
// ****************************************************************************
void Talk_SIM900()
{
  Serial.println(" ");
  Serial.println("Разговор с SIM900 ...");
  delay(1000);
  // AT
  // Serial.println("Рукопожатие с SIM900. \"AT\" – Это самая простая команда AT.");
  // Serial.println("Она также инициализирует передачу данных в бодах.");
  // Serial.println("Если это сработает, далее вы будете видеть эхо-сигнал символов AT, а затем OK,");
  // Serial.println("сообщающий вам, что все в порядке и SIM900 правильно вас понимает!");
  SIM900.println("AT"); // Handshaking with SIM900
  update_SIM900();
  // AT+CSQ
  // Serial.println("AT+CSQ – Проверить \"уровень сигнала\" – первое значение это уровень сигнала в дБ,");
  // Serial.println("он должен быть выше 5. Чем выше, тем лучше, до 31.");
  // Serial.println("Конечно, это зависит от вашей антенны и местоположения!");
  SIM900.println("AT+CSQ"); // Signal quality test, value range is 0-31 , 31 is the best
  update_SIM900();
  // AT+CCID 
  // Serial.println("AT+CCID – Получить номер SIM–карты,");
  // Serial.println("это проверка того, что SIM-карта найдена нормально,");
  // Serial.println("и вы можете проверить, какой номер присвоен карте.");
  SIM900.println("AT+CCID"); // Read SIM information to confirm whether the SIM is plugged
  update_SIM900();
  // AT+CREG? 
  // Serial.println("AT+CREG? Проверить, зарегистрированы ли вы в сети. Второе число должно быть");
  // Serial.println("1 или 5. 1 означает, что вы зарегистрированы в домашней сети, а 5 - в роуминговой сети.");
  // Serial.println("Кроме этих двух цифр, вы не зарегистрированы ни в какой сети.");
  SIM900.println("AT+CREG?"); // Check whether it has registered in the network
  update_SIM900();
  // ATI
  // Serial.println("ATI – Получить название платы и её редакцию");
  SIM900.println("ATI"); 
  update_SIM900();
  // AT+COPS?
  // Serial.println("AT+COPS? – Убедится, что вы подключены к сети и получить информацию об операторе");
  SIM900.println("AT+COPS?"); 
  update_SIM900();
  // AT+CBC
  Serial.println("AT+CBC – Получить состояние батареи lipo. Вторая цифра - это % заполнения,");
  Serial.println("а третья цифра - фактическое напряжение в мВ.");
  SIM900.println("AT+CBC"); 
  update_SIM900();
}

// Arduino C/C++ ************************************** Ex1_Talk_SIM900.ino ***
                                                                                                                                                                                                  
