#include "bh1750fvi.h"

BH1750 lightMeter;

#define I2C_SCL       9  // Pin SCL (bên phải)
#define I2C_SDA       8  // Pin SDA (bên phải) 

void BH1750FVI_Init(void)
{
  Wire.begin(I2C_SDA, I2C_SCL); // Khởi động I2C với SDA=IO18, SCL=IO19
  if (  lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x23, &Wire)) {
    Serial.println("BH1750 init success");
  } else {
    Serial.println("BH1750 init failed");
  }
}

float BH1750FVI_ReadLux(void)
{
  return lightMeter.readLightLevel();
} 

