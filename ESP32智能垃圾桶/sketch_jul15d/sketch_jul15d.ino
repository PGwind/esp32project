#include "sonar.h"
#include "cover.h"
#include "servo.h"
int ledPin = 2;//板载灯
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED; // 自旋锁

// 打开盖子
void open_cover()
{
  bool shouldAct = false; // 开盖行为
  
  /*
     临界区是一段代码片段，用于在多任务环境下保护共享资源，以确保对资源的访问不会被并发任务中断或干扰。
     临界区的作用是提供一种互斥机制，使得同一时间只有一个任务可以访问共享资源，避免并发访问导致的数据竞争和不一致性。
  */
  portENTER_CRITICAL(&mux); // 进入临界区
  if (openTime == 0) // 打开时间为0时
    shouldAct = true;
  openTime = micros(); // 记录当前打开盖子的时间
  portEXIT_CRITICAL(&mux);  // 离开临界区

  if (shouldAct)
    servo.write(145); // 舵机旋转145，即开盖°
}

void close_cover()
{
  servo.write(0); // 舵机为0°，即关盖
}

void setup() {
  pinMode(ledPin,OUTPUT); //输出模式
  Serial.begin(115200);
  sonar_init(&mux); // 超声波模块初始化
  cover_detect_init(&mux); // 初始化盖子检测相关的设置
  servo_init(); // 舵机初始化
  close_cover(); // 刚开始为盖上盖子状态
}

void loop() {
  // 超声波有信号回来
  if (xSemaphoreTake(open_semaphore, 0) == pdTRUE) // 打开信号为true
  {
    Serial.println("open"); // 串口打印信息
    open_cover();
    digitalWrite(ledPin, HIGH); // 灯亮
  }

  if (xSemaphoreTake(close_semaphore, 0) == pdTRUE) // 关闭信号为true
  {
    Serial.println("close");
    close_cover();
    digitalWrite(ledPin, LOW); // 灯灭
  }
}