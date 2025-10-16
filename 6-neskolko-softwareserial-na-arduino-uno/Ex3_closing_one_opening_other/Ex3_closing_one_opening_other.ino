/** Arduino C/C++ *********************** Ex3_closing_one_opening_other.ino ***
 *
 * 3 пример из набора примеров "Несколько SoftwareSerial на Ардуино UNO"
 * 
 * Здесь основной цикл построен следующим образом:
 * 
 * в начале цикла условное ожидание 1 секунды для того,
 * чтобы буфер последовательного порта V.KEL-TTL заполнился данными 
 * с координатами;
 * 
 * далее через программный последовательный порт на пинах 2 = RX, 3 - TX, 
 * производится выборка данных навигации из приёмника GPS V.KEL TTL
 * 
 * и в итоге через программный последовательный порт на пинах 7 = RX, 8 - TX, 
 * производится отправка AT-команд на SIM900.
 * 
 * v1.0.0, 16.10.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 16.10.2025
 *
**/

#include <SoftwareSerial.h>
SoftwareSerial VKEL_TTL(2,3); // синий на 2 - будет RX; зеленый на 3 - будет TX
//SoftwareSerial   SIM900(7,8); // SIM900 
unsigned long ncikl=0;

// Настраиваем переменные и модули для работы с V.KEL TTL
#include "TinyGPS.h"
TinyGPS gps;
long lat, lon;
long lat0=61801900, lon0=34329700;  // координаты выбранной точки
float DistanceBetween;
unsigned long time, date;

void setup()
{
  Serial.begin(115200);

  //VKEL_TTL.begin(9600); // скорость обмена с GPS-приемником
  Serial.println(" ");
  Serial.println("Ожидаем ii разговора с V.KEL-TTL ...");

  //SIM900.begin(9600);
  //delay(500);
  //update_SIM900();
  //Talk_SIM900();
}
 
void loop()
{
  ncikl++;
  // Делаем задержку в 1 секунду для того, чтобы буфер последовательного
  // порта V.KEL-TTL заполнился данными с координатами
  VKEL_TTL.begin(9600); 
  delay(1000);
  // Выбираем данные навигации из приёмника GPS V.KEL TTL 
  Talk_VKEL_TTL();
  VKEL_TTL.end();

  //update_SIM900();
}

// ****************************************************************************
// *            Выбрать данные навигации из приёмника GPS V.KEL TTL           *
// ****************************************************************************
bool Talk_VKEL_TTL()
{
  bool newdata = false;
  // Если данные есть, то считываем их и публикуем
  newdata = readgps();
  if (newdata)
  {
    gps.get_position(&lat, &lon);
    gps.get_datetime(&date, &time);
    DistanceBetween=gps.distance_between (lat0,lon0,lat,lon)/10000000;

    Serial.print(ncikl);     Serial.print(":"); 
    Serial.print(" Lat: ");  Serial.print(lat);
    Serial.print(" Long: "); Serial.print(lon);
    Serial.print(" Date: "); Serial.print(date);
    Serial.print(" Time: "); Serial.print(time);
    Serial.print(" Dist: "); Serial.println(DistanceBetween);
  }
  return newdata;
}
// ****************************************************************************
// *          Проверить наличие данных в буфере приёмника GPS V.KEL TTL       *
// ****************************************************************************
bool readgps()
{
  while (VKEL_TTL.available())
  {
    int b = VKEL_TTL.read();
    // в TinyGPS есть ошибка: не обрабатываются данные с \r и \n
    if ('\r' != b)
    {
      if (gps.encode(b)) return true;
    }
  }
  return false;
}

/*
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
*/

// Arduino C/C++ ************************ Ex3_closing_one_opening_other.ino ***
                                                                                                                                                                                                  
