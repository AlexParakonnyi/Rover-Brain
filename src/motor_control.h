#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

// Пины для управления направлением
#define IN1 4   // Motor A
#define IN2 5   // Motor A
#define IN3 12  // Motor B
#define IN4 15  // Motor B
#define IN5 17  // Motor C
#define IN6 18  // Motor C
#define IN7 46  // Motor D
#define IN8 10  // Motor D

// Пины для PWM (скорость)
#define ENA 6   // Motor A
#define ENB 16  // Motor B
#define ENC 3   // Motor C
#define END 11  // Motor D

// Настройки PWM
const int resolutionMotor = 8;  // Разрешение 8 бит (0-255)
const int pwmChannelA = 0;      // Канал PWM для Motor A
const int pwmChannelB = 1;      // Канал PWM для Motor B
const int pwmChannelC = 2;      // Канал PWM для Motor C
const int pwmChannelD = 3;      // Канал PWM для Motor D

// Настройки PWM (перемещены в реализацию для динамической настройки)
const int freq = 1300;  // Базовая частота для скоростей до 150

// Прототип функции
void setMotor(int motor, int direction, int speed);
void initMotorPins();
void initMotors();
void stop();
// void goForwardLeft(uint8_t speed);
// void goForwardRight(uint8_t speed);
// void goBackwardLeft(uint8_t speed);
// void goBackwardRight(uint8_t speed);
// void goForward(uint8_t speed);
// void goBackward(uint8_t speed);
// void goLeft(uint8_t speed);
// void goRight(uint8_t speed);

#endif
