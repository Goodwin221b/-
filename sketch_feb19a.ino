#include <Adafruit_NeoPixel.h>
 
// номер пина, к которому подключена RGB-матрица
#define MATRIX_C_PIN    4
// количество светодиодов в матрице
#define LED_COUNT 16
// зерно для генератора случайных чисел
#define ANALOG_PIN_FOR_RND  A3
// максимальная яркость матрицы от 0 до 255
#define BRIGHT 10
 
// создаём объект класса Adafruit_NeoPixel
Adafruit_NeoPixel matrixOnC = Adafruit_NeoPixel(LED_COUNT, MATRIX_C_PIN, NEO_GRB + NEO_KHZ800);
 
// создаем объект класса String, в который будем записывать информацию с клавиатуры
String player = "";
// создаем объект класса String, в который будет записываться случайная последовательность "цветов"
String simon = "";
// создаем счетчик очков
int score = 0;
// создаем массив для хранения стандартных цветов
const char color [] = {'r', 'g', 'b'};
 
void setup() {
  // открываем последовательный порт
  Serial.begin(9600);
  // инициализируем последовательность случайных чисел
  randomSeed(analogRead(ANALOG_PIN_FOR_RND));
  // записываем первый случайный цвет в последовательность
  simon = color [random(0, 3)];
  // инициализируем матрицу
  matrixOnC.begin();
  // задаем придельную яркость
  matrixOnC.setBrightness(BRIGHT);
  // очищаем матрицу
  matrixOnC.clear();
  matrixOnC.show();
  // "Давай сыграем!"
  Serial.println("Наши цвета: красный(r), зелёный(g), синий(b)");
  Serial.println("Let's play!");
  delay(1000);
  // зажигаем матрицу с заданной последовательностью
  matrix_show_simon();
}
 
void loop() {
  // пока в Serial port поступают символы...
  while (Serial.available() > 0) {
    // ...символ записываем в переменную
    char symbol = Serial.read();
    // если приходит символ разрыва строки...
    if (symbol == '\n') {
      // ...то сравниваем, совпадает ли последовательность игрока с заданной
      if ( player == simon) {
        // если "да", то выполняем условия победы:
        // выводим счет в Serial port
        Serial.print("You win! Your score: ");
        // увеличиваем счет
        score++;
        Serial.println(score);
        // увеличиваем сложность добавив в последовательность цветов новый элемент
        simon += color [random(0, 3)];
        // "Давай сыграем еще раз!"
        Serial.println("Let's play again!");
        delay(1000);
        // показываем цвета на матрице
        matrix_show_simon();
        // опустошаем последовательность, ранее введенную игроком
        player = "";
      } else {
        // если "нет", то выполняем условия поражения:
        Serial.print("You lose. Your score: ");
        Serial.println(score);
        // обнуляем счет
        score = 0;
        // инициализируем новую последовательность случайных чисел
        randomSeed(analogRead(ANALOG_PIN_FOR_RND));
        // записываем первый случайный цвет в последовательность
        simon = color [random(0, 3)];
        // "Да ладно! Давай сыграем!"
        Serial.println("Come on! Let's play!");
        delay(1000);
        // показываем цвета на матрице
        matrix_show_simon();
        // опустошаем последовательность, ранее введенную игроком
        player = "";
      }
    } else {
      // ... если символ не является символом разрыва строки, то вписываем его
      // в последовательность, введенную игроком
      player += symbol;
    }
  }
}
 
// функция показа последовательности цветов
void matrix_show_simon() {
  for  (int i = 0; i <= score; i++) {
    // отправляем, в функцию показа цвета, i-тый символ из объекта simon
    matrix_color(simon.charAt(i));
  }
}
 
// функция для показа нужного цвета на матрице
void matrix_color(char rgb) {
  switch (rgb) {
    case 'r': {
        for (int i = 0; i < LED_COUNT; i++)
          matrixOnC.setPixelColor(i, 255, 0, 0);
        break;
      }
    case 'g': {
        for (int i = 0; i < LED_COUNT; i++)
          matrixOnC.setPixelColor(i, 0, 255, 0);
        break;
      }
    case 'b': {
        for (int i = 0; i < LED_COUNT; i++)
          matrixOnC.setPixelColor(i, 0, 0, 255);
        break;
      }
  }
  matrixOnC.show();
  delay(400);
  matrixOnC.clear();
  matrixOnC.show();
  delay(100);
}
