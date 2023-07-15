#include "cover.h"

hw_timer_t *cover_timer = NULL; // 定时器
static portMUX_TYPE *_mux = NULL;
volatile SemaphoreHandle_t close_semaphore; // 关盖信号量
volatile unsigned long openTime = 0; // 打开盖子的时间

// 中断服务程序ISR:检测盖子是否关闭
void IRAM_ATTR close_detect()
{
  portENTER_CRITICAL_ISR(_mux);
  auto now = micros();
  if (openTime != 0 && (now - openTime) >= 4000000) // 打开盖子时间大于等于4s则关闭
  { 
    openTime = 0;
    xSemaphoreGiveFromISR(close_semaphore, NULL);
  }
  portEXIT_CRITICAL_ISR(_mux);
}

void cover_detect_init(portMUX_TYPE *mux)
{
  _mux = mux;
  close_semaphore = xSemaphoreCreateBinary();

  // 检测到关闭部分，0.5秒检测一次
  cover_timer = timerBegin(2, 80, true);  // 初始化计时器2，分频系数80，使能中断
  timerAttachInterrupt(cover_timer, close_detect, true);  // 附加中断处理函数 close_detect 到计时器
  timerAlarmWrite(cover_timer, 500000, true);  // 设置计时器的定时时间为500000微秒（0.5秒），并使能重复触发
  timerAlarmEnable(cover_timer);  // 启动计时器
}