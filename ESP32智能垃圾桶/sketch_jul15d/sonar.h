#pragma once
#include <freertos/FreeRTOS.h>
#include <esp32-hal-timer.h>
#include <freertos/semphr.h>

extern volatile SemaphoreHandle_t open_semaphore; // 信号量
void sonar_init(portMUX_TYPE *mux);