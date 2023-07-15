#include "servo.h"

// 舵机部分
Servo servo;
int minUs = 500;
int maxUs = 2500;
int servoPin = 13;

void servo_init()
{
  // 舵机
  ESP32PWM::allocateTimer(1);
  servo.setPeriodHertz(50);
  servo.attach(servoPin, minUs, maxUs);
}