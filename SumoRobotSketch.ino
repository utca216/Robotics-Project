#include <Ultrasonic.h>

// Пины для моторов и L298N
#define IN1 2     //  Пины для управления направлением
#define IN2 3
#define IN3 4
#define IN4 5
#define ENA 6     // Пины для управления скоростью через PWM
#define ENB 7

// Пины для ультразвукового датчика
#define TRIG_PIN 8
#define ECHO_PIN 9

Ultrasonic ultrasonic(TRIG_PIN, ECHO_PIN); // Инициализация ультразвукового датчика

// Время поворота
#define TURN_TIME 500

void setup() {
  // Устанавливаем пины для управления моторами как выходы
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  
  // Инициализация скорости моторов
  analogWrite(ENA, 255); // Максимальная скорость для первого двигателя
  analogWrite(ENB, 255); // Максимальная скорость для второго двигателя
  
  // Инициализация серийного монитора для отладки
  Serial.begin(9600);
}

void loop() {
  long distance = ultrasonic.read(); // Считываем расстояние с ультразвукового датчика
  
  // Проверяем, что расстояние меньше 255 (обнаружена цель)
  if (distance < 255) {
    moveForward(); // Если цель найдена, едем вперед
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

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnLeft(int duration) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(duration); // Время поворота
}

void turnRight(int duration) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(duration); // Время поворота
}