#include "sonar.h"

volatile SemaphoreHandle_t open_semaphore; // 信号量

// 超声波测距部分
const int trigPin = 17; 
const int echoPin = 18;
int distance = 0;

static portMUX_TYPE *_mux = NULL;
hw_timer_t *sonar_timer = NULL; // 定时器
volatile unsigned long startTime = 0; // 发出超声波时间
volatile unsigned long endTime = 0; // 收到超声波时间

// 硬件定时器ISR
void IRAM_ATTR ping()
{
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin, LOW);
}

// ECHO 引脚ISR
void IRAM_ATTR changeISR() 
{
  auto now = micros(); // 当前时间
  auto state = digitalRead(echoPin);

  portENTER_CRITICAL_ISR(_mux);
  if (state) // 高电平，即刚发出超声波
    startTime = now;
  else
    endTime = now;
// 变成低电平时表示已经收到回声
// 如果 < 10cm 就发信号开盖
  if (!state) {
    auto t = endTime - startTime;
    auto dis = t * 0.01715;
    if (dis <= 10)
    {
      xSemaphoreGiveFromISR(open_semaphore, NULL); // 给一个开盖信号量发送信号
    }
  }
  portEXIT_CRITICAL_ISR(_mux);
}

void sonar_init(portMUX_TYPE* mux)
{
  _mux = mux;
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  open_semaphore = xSemaphoreCreateBinary();

  //测距定时器部分
  sonar_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(sonar_timer, ping, true);
  timerAlarmWrite(sonar_timer, 200000, true); // 定时时间为 0.2s

  // echo引脚的中断
  attachInterrupt(digitalPinToInterrupt(echoPin), changeISR, CHANGE);

  // 开始周期测量
  timerAlarmEnable(sonar_timer);
}