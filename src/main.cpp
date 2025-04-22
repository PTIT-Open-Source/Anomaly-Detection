#include <Arduino.h>
#include "trungnm2111-project-1_inferencing.h"
#include "dht11.h"
#include "bh1750fvi.h"
#include "watchdog_reset.h"

void Anomaly_Detect(float temp, float humi, float lux);

void setup() {
  Serial.begin(115200);
  DHT11_Init();
  BH1750FVI_Init();
  if (!EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME) {
    Serial.println("ERR: Model not loaded correctly");
    while (1);
  }
  SetUp_WDT();
}

void loop() {
  float light , temp, humi;
  light = BH1750FVI_ReadLux();
  temp = DHT11_ReadTemperature();
  humi = DHT11_ReadHumidity();
  // temp = 28;
  unsigned long time_out_data1 = 0, time_out_data2 = 0 ;
  if (!isnan(humi) && !isnan(temp) && (light > 0)) {
    time_out_data1 = 0;
    Anomaly_Detect(temp, humi, light);
  }
  time_out_data2 = millis();
  Serial.println("time data");
  Serial.println(time_out_data2 - time_out_data1);
  Serial.println("humi");
  Serial.println(humi);
  Serial.println("temp");
  Serial.println(temp);
  Serial.println("light");
  Serial.println(light);
    
  delay(2000);
  Serial.println("Scanning...");
  watchDogRefresh();
}


// 
void Anomaly_Detect(float temp, float humi, float lux)
{
  float features[] = {temp, humi, lux};
  // Chuẩn bị buffer cho Edge Impulse
  ei::signal_t signal;
  ei::numpy::signal_from_buffer(features, 3, &signal);

  // Chạy mô hình
  ei_impulse_result_t result = {0};
  EI_IMPULSE_ERROR res = run_classifier(&signal, &result, false);
  
  if (res != EI_IMPULSE_OK) {
    Serial.println("ERR: Failed to run classifier");
    return;
  }
    
  // Kiểm tra điểm anomaly
  float anomaly_score = result.anomaly;
  // Serial.print("Anomaly score: ");
  // Serial.println(anomaly_score);
   
  // Ngưỡng anomaly
  float threshold = 0.5; // Điều chỉnh theo huấn luyện
  if (anomaly_score > threshold) {
  Serial.println("Anomaly detected!");
    
  float avg_temp = 25.0;   
  float avg_humi = 60.0;   
  float avg_lux = 500.0;   
     
  // Tính độ lệch của từng trục (so với giá trị trung bình)
  float temp_deviation = abs(temp - avg_temp);
  float humi_deviation = abs(humi - avg_humi);
  float lux_deviation = abs(lux - avg_lux);
     
  
  float temp_dev_threshold = 10.0;  
  float humi_dev_threshold = 30.0;  
  float lux_dev_threshold = 1000.0; 
     
  // Xác định và hiển thị trục bất thường
  Serial.print("Abnormal data: ");
  bool has_anomaly = false;
  if (temp_deviation > temp_dev_threshold) {
    Serial.print("Temperature ");
    has_anomaly = true;
  }
  if (humi_deviation > humi_dev_threshold) {
    Serial.print("Humidity ");
    has_anomaly = true;
  }
  if (lux_deviation > lux_dev_threshold) {
    Serial.print("Light ");
    has_anomaly = true;
  }
  if (!has_anomaly) {
    Serial.print("Unknown");
  }
  Serial.println();
  } else {
  Serial.println("Normal");
  }
}
