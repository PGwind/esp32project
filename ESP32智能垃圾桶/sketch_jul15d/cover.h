#pragma once //预处理指令，用于确保头文件只被编译一次

#include <freertos/FreeRTOS.h>
#include <esp32-hal-timer.h>
#include <freertos/semphr.h>

extern volatile unsigned long openTime;
extern volatile SemaphoreHandle_t close_semaphore;

void cover_detect_init(portMUX_TYPE *mux);