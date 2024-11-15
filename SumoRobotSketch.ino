#include <Ultrasonic.h>
#include <Servo.h>

// Пины для моторов и L298N
#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 5
#define ENA 6
#define ENB 7

// Пины для ультразвукового датчика
#define TRIG_PIN 8
#define ECHO_PIN 9

// Пины для сервомотора
#define SERVO_PIN 10      // Управляющий пин для сервомотора
#define SERVO_VCC_PIN 11  // Питание для сервомотора
#define SERVO_GND_PIN 12  // GND для сервомотора

// Пины для датчиков цвета
#define photoResistorFrontPin = A0;  // Передний фоторезистор
#define photoResistorBackPin = A1;   // Задний фоторезистор

Ultrasonic ultrasonic(TRIG_PIN, ECHO_PIN); // Инициализация ультразвукового датчика
Servo servo;                               // Объект для управления сервомотором

// Переменные для управления сервомотором
unsigned long servoLastMoved = 0; // Время последнего поворота сервомотора
const int servoDelay = 500;       // Задержка для стабилизации сервопривода
bool servoTurned = false;         // Флаг для отслеживания состояния поворота сервомотора

// Время движения при обнаружении черной линии
const int moveTimeOnLine = 500;

void setup() {
  // Устанавливаем пины для управления моторами как выходы
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Настройка пинов питания для сервомотора
  pinMode(SERVO_VCC_PIN, OUTPUT);
  pinMode(SERVO_GND_PIN, OUTPUT);
  digitalWrite(SERVO_VCC_PIN, HIGH);  // Подаем питание на серво
  digitalWrite(SERVO_GND_PIN, LOW);   // Подключаем землю для серво

  // Подключение сервопривода к управляющему пину
  servo.attach(SERVO_PIN);

  // Настройка фоторезисторов
  pinMode(photoResistorFrontPin, INPUT);
  pinMode(photoResistorBackPin, INPUT);

  // Инициализация скорости моторов
  analogWrite(ENA, 255); // Максимальная скорость для первого двигателя
  analogWrite(ENB, 255); // Максимальная скорость для второго двигателя

  // Инициализация серийного монитора для отладки
  Serial.begin(9600);
}

void loop() {
  long distance = ultrasonic.read(); // Считываем расстояние с ультразвукового датчика
  
  // Чтение значений с фоторезисторов
  int frontLightLevel = analogRead(photoResistorFrontPin);
  int backLightLevel = analogRead(photoResistorBackPin);

  // Проверка на черную линию сзади и спереди
  if (frontLightLevel < lightThreshold) {
    // Черная линия спереди - отъехать назад
    moveBackward();
    delay(500);
    stopMotors();
  } else if (backLightLevel < lightThreshold) {
    // Черная линия сзади - ехать вперед
    moveForward();
    delay(500);
    stopMotors();
  }
  else {
    // Основная логика движения
    if (distance < 5) {
      // Если цель очень близко, проверяем состояние поворота сервомотора
      if (!servoTurned) {
        servo.write(60);                  // Поворачиваем сервомотор на 60 градусов
        servoLastMoved = millis();        // Запоминаем время поворота
        servoTurned = true;               // Устанавливаем флаг поворота
      }
    } else if (distance < 255) {
      // Если цель найдена, но не слишком близко, едем вперед
      moveForward();
      Serial.println("Цель найдена!");
    } else {
      // Если цель не найдена, начинаем крутиться
      turnRight(TURN_TIME);
      distance = ultrasonic.read(); // Переходим к следующей проверке

      if (distance >= 255) {
        turnLeft(TURN_TIME); // Если после поворота вправо цель не найдена, поворачиваем влево
        distance = ultrasonic.read(); // Считываем новое расстояние

        if (distance >= 255) {
          // Если после поворота влево цель все равно не найдена, продолжаем крутиться влево
          turnLeft(TURN_TIME);
        }
      }
    }
  }

  // Проверяем, прошло ли достаточно времени для стабилизации сервомотора
  if (servoTurned && (millis() - servoLastMoved >= servoDelay)) {
    servo.write(0);         // Возвращаем сервомотор в исходное положение
    servoTurned = false;    // Сбрасываем флаг поворота
  }
}

// Функция движения вперед
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Функция движения назад
void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// Функция поворота влево
void turnLeft(int duration) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(duration); // Время поворота
}

// Функция поворота вправо
void turnRight(int duration) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(duration); // Время поворота
}

// Функция движения вперед на заданное время
void moveForwardFor(int duration) {
  moveForward();
  delay(duration);
  stopMotors();
}

// Функция движения назад на заданное время
void moveBackwardFor(int duration) {
  moveBackward();
  delay(duration);
  stopMotors();
}

// Функция остановки моторов
void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
