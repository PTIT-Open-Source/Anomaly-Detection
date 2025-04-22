#ifndef _WATCHDOG_RESET_H
#define _WATCHDOG_RESET_H

#include <Arduino.h>
// Include Library
#include <esp_task_wdt.h>

// Define
#define WATCHDOG_TIMEOUT_S 10

// Global Variables
extern hw_timer_t * watchDogTimer;

void SetUp_WDT(void);
void IRAM_ATTR watchDogInterrupt();
void watchDogRefresh();

#endif