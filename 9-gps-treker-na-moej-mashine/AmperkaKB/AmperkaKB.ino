/** Arduino UNO, SIM900 ************************************* AmperkaKB.ino ***
 * 
 * Отладить подключение матричная клавиатуры через SIM900
 * 
 * v1.0.0, 21.11.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 21.11.2025
 * 
 * Матричная клавиатура 4×3 кнопки: инструкция, подключение и примеры использования
 * https://wiki.amperka.ru/продукты:keypad-4x3
 * 
 * Библиотека для Arduino, для быстрого поиска нужной кнопки для эластичных клавиатур
 * https://github.com/amperka/AmperkaKB
 * 
**/


// подключаем библиотеку для работы с матричной клавиатурой
#include <AmperkaKB.h>
 
// создаём объект для работы с матричной клавиатурой
// указывая номера arduino подключенные к шлейфу клавиатуры
// начиная с первого вывода шлейфа
AmperkaKB KB(2, 3, 4, 5, 6, 10, 11);
 
void setup()
{
  // открываем монитор Serial порта
  Serial.begin(115200);
  // указываем тип клавиатуры
  KB.begin(KB4x3,5000);
  // второй необзятельный параметр:
  // время длительного зажатия кнопки
  // по умолчанию 2000, изменим на 5000 мс
  // KB.begin(KB4x3, 5000);
}
 
void loop()
{
  // считывание данных с клавиатуры
  KB.read();
  // опеределяем клик кнопки
  if (KB.justPressed()) 
  {
    // печатаем номер кнопки и её символ в последовательный порт
    Serial.print("Key is press ");
    Serial.print(KB.getNum);
    Serial.print(" = \"");
    Serial.print(KB.getChar);
    Serial.println("\"");
  }
  // определяем отжатие кнопки
  if (KB.justReleased()) 
  {
    // печатаем номер кнопки и её символ в последовательный порт
    Serial.print("Key is release ");
    Serial.print(KB.getNum);
    Serial.print(" = \"");
    Serial.print(KB.getChar);
    Serial.println("\"");
  }
  // определяем зажатие кнопки на 3 секунды
  if (KB.isHold()) 
  {
    // печатаем номер кнопки и её символ в последовательный порт
    Serial.print("Key on long press ");
    Serial.print(KB.getNum);
    Serial.print(" = \"");
    Serial.print(KB.getChar);
    Serial.println("\"");
  }
}

// ********************************************************** AmperkaKB.ino ***

