#include "motor_control.h"

#include <Arduino.h>

void initMotorPins() {
  // Настройка пинов как выходов
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);
  pinMode(IN7, OUTPUT);
  pinMode(IN8, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(ENC, OUTPUT);
  pinMode(END, OUTPUT);
}

void setMotor(int motor, int direction, int speed) {
  if (speed < 0) speed = 0;
  if (speed > 255) speed = 255;

  // Выбор мотора
  if (motor == 1) {        // Motor A
    if (direction == 1) {  // Вперед
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
    } else if (direction == -1) {  // Назад
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
    } else {  // Стоп
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
    }
    ledcSetup(pwmChannelA, freq, resolutionMotor);
    ledcWrite(pwmChannelA, speed);
  } else if (motor == 2) {  // Motor B
    if (direction == 1) {   // Вперед
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    } else if (direction == -1) {  // Назад
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    } else {  // Стоп
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
    }
    ledcSetup(pwmChannelB, freq, resolutionMotor);
    ledcWrite(pwmChannelB, speed);
  } else if (motor == 3) {  // Motor C
    if (direction == 1) {   // Вперед
      digitalWrite(IN5, HIGH);
      digitalWrite(IN6, LOW);
    } else if (direction == -1) {  // Назад
      digitalWrite(IN5, LOW);
      digitalWrite(IN6, HIGH);
    } else {  // Стоп
      digitalWrite(IN5, LOW);
      digitalWrite(IN6, LOW);
    }
    ledcSetup(pwmChannelC, freq, resolutionMotor);
    ledcWrite(pwmChannelC, speed);
  } else if (motor == 4) {  // Motor D
    if (direction == 1) {   // Вперед
      digitalWrite(IN7, HIGH);
      digitalWrite(IN8, LOW);
    } else if (direction == -1) {  // Назад
      digitalWrite(IN7, LOW);
      digitalWrite(IN8, HIGH);
    } else {  // Стоп
      digitalWrite(IN7, LOW);
      digitalWrite(IN8, LOW);
    }
    ledcSetup(pwmChannelD, freq, resolutionMotor);
    ledcWrite(pwmChannelD, speed);
  }
}

void initMotors() {
  // Инициализация пинов
  initMotorPins();

  // Инициализация PWM
  ledcSetup(pwmChannelA, freq, resolutionMotor);
  ledcSetup(pwmChannelB, freq, resolutionMotor);
  ledcSetup(pwmChannelC, freq, resolutionMotor);
  ledcSetup(pwmChannelD, freq, resolutionMotor);
  ledcAttachPin(ENA, pwmChannelA);
  ledcAttachPin(ENB, pwmChannelB);
  ledcAttachPin(ENC, pwmChannelC);
  ledcAttachPin(END, pwmChannelD);

  // Начальное состояние - двигатели выключены
  setMotor(1, 0, 0);
  setMotor(2, 0, 0);
  setMotor(3, 0, 0);
  setMotor(4, 0, 0);
}

void goForward(uint8_t speed) {
  setMotor(1, 1, speed);
  setMotor(2, 1, speed);
  setMotor(3, 1, speed);
  setMotor(4, 1, speed);
}

void goBackward(uint8_t speed) {
  setMotor(1, -1, speed);
  setMotor(2, -1, speed);
  setMotor(3, -1, speed);
  setMotor(4, -1, speed);
}

void goLeft(uint8_t speed) {
  setMotor(1, 1, speed);
  setMotor(2, 1, speed);
  setMotor(3, -1, speed);
  setMotor(4, -1, speed);
}

void goRight(uint8_t speed) {
  setMotor(1, -1, speed);
  setMotor(2, -1, speed);
  setMotor(3, 1, speed);
  setMotor(4, 1, speed);
}

void stop() {
  setMotor(1, 0, 0);
  setMotor(2, 0, 0);
  setMotor(3, 0, 0);
  setMotor(4, 0, 0);
}

void goForwardLeft(uint8_t speed) {
  setMotor(1, 1, speed);
  setMotor(2, 1, speed);
  setMotor(3, 1, speed);
  setMotor(4, 1, speed - 20);
}

void goForwardRight(uint8_t speed) {
  setMotor(1, 1, speed);
  setMotor(2, 1, speed);
  setMotor(3, 1, speed - 20);
  setMotor(4, 1, speed);
}

void goBackwardLeft(uint8_t speed) {
  setMotor(1, -1, speed);
  setMotor(2, -1, speed);
  setMotor(3, 1, speed);
  setMotor(4, 1, speed - 20);
}

void goBackwardRight(uint8_t speed) {
  setMotor(1, -1, speed);
  setMotor(2, -1, speed);
  setMotor(3, 1, speed - 20);
  setMotor(4, 1, speed);
}