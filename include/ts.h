#ifndef _TS_H
#define _TS_H

// Include Library
#include <WiFi.h>
#include "ThingSpeak.h"

// Define
extern unsigned long myChannelNumber;
extern const char * myWriteAPIKey;



// Global Variables
void Wifi_Init(void);

#endif