#include "watchdog_reset.h"

hw_timer_t * watchDogTimer = NULL;

void IRAM_ATTR watchDogInterrupt() 
{
  Serial.println("reboot");
  ESP.restart();
}

void watchDogRefresh()
{
  timerWrite(watchDogTimer, 0);                    //reset timer (feed watchdog)
}

void SetUp_WDT(void)
{
  Serial.println("Watchdog sketch start");

  watchDogTimer = timerBegin(2, 80, true);
  timerAttachInterrupt(watchDogTimer, &watchDogInterrupt, true);
  timerAlarmWrite(watchDogTimer, WATCHDOG_TIMEOUT_S * 1000000, false);
  timerAlarmEnable(watchDogTimer);
}