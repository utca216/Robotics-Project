#include <AFMotor.h>
#include <NewPing.h> // Библиотека для работы с ультразвуковым датчиком
#include <Servo.h>
Servo myservo;

#define TRIG_PIN A3       // Пин TRIG ультразвукового датчика
#define ECHO_PIN A4       // Пин ECHO ультразвукового датчика
#define MAX_DISTANCE 200  // Максимальная дистанция, которую измеряет датчик

#define TARGET_DISTANCE 70   // Расстояние, при котором цель считается найденной
#define Servo_Distance 10
#define TURN_DURATION 2000   // Длительность поворота вправо в миллисекундах
#define SPEED_FORWARD 200    // Скорость движения вперед
#define SPEED_TURN 150       // Скорость при поворотах
#define DISTANCE_CHECK_RATE 50 // Частота проверки датчика (в миллисекундах)

AF_DCMotor motor1(1, MOTOR12_64KHZ); // Левый мотор
AF_DCMotor motor2(2, MOTOR12_64KHZ); // Правый мотор

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

unsigned long lastDistanceCheck = 0; // Время последней проверки расстояния
unsigned long lastTurnTime = 0;      // Время начала поворота
int currentState = 0;                // 0: остановка, 1: вращение вправо, 2: вращение влево, 3: движение вперед

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(A2, OUTPUT);
  digitalWrite(A5, LOW);
  digitalWrite(A2, HIGH);
  Serial.begin(9600); // Для отладки
  myservo.attach(10);
  myservo.write(60);
}

void loop() {
  unsigned long currentMillis = millis();

  // Проверка расстояния каждые 50 мс
  if (currentMillis - lastDistanceCheck >= DISTANCE_CHECK_RATE) {
    lastDistanceCheck = currentMillis;
    int distance = measureDistance();
    Serial.println(distance);
    // Логика определения состояния на основе расстояния
    if (distance > 0 && distance < Servo_Distance) { // Цель найдена
      currentState = 4;
    }
    else if (distance > 0 && distance < TARGET_DISTANCE) { // Цель найдена
      currentState = 3;
    } else if (currentState == 3) { // Если цель потеряна
      currentState = 1;  // Начать вращение вправо
      lastTurnTime = currentMillis;
    }
  }

  // Выполнение действий в зависимости от текущего состояния
  switch (currentState) {
    case 0: // Остановиться
      stopMotors();
      break;

    case 1: // Вращение вправо
      turnRight();
      if (currentMillis - lastTurnTime >= TURN_DURATION) { // Переход на вращение влево
        currentState = 2;
      }
      break;

    case 2: // Вращение влево
      turnLeft();
      if (measureDistance() < TARGET_DISTANCE) { // Если цель найдена
        currentState = 3;
      }
      break;

    case 3: // Движение вперед
      moveForward();
      break;
    case 4:
      myservo.write(20);
      moveForward();
     delay(200);
      myservo.write(10);

  }

}

// Функция измерения дистанции
int measureDistance() {
  int distance = sonar.ping_cm();
  if (distance == 0 || distance > MAX_DISTANCE) {
    return -1; // Некорректные данные
  }
  return distance;
}

// Движение вперед
void moveForward() {
  motor1.setSpeed(SPEED_FORWARD);
  motor2.setSpeed(SPEED_FORWARD);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
}

// Поворот на месте вправо
void turnRight() {
  motor1.setSpeed(SPEED_TURN);
  motor2.setSpeed(SPEED_TURN);
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
}

// Поворот на месте влево
void turnLeft() {
  motor1.setSpeed(SPEED_TURN);
  motor2.setSpeed(SPEED_TURN);
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
}

// Остановка моторов
void stopMotors() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
}
