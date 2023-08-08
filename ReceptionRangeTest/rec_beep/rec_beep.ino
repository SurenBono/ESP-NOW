
#include <ESP8266WiFi.h>
#include <espnow.h>

#define PIEZO_PIN  13  //D11 GPIO pin piezo buzzer

void onDataReceiver(uint8_t * mac, uint8_t *Data, uint8_t len){

  Serial.print(" - ");
  Serial.write(Data, len);
  Serial.println();
  tone(PIEZO_PIN, 4000, 30);//f,time
  delay(500);  // Pause between sounds
}

void setup() {
  
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW initialization failed");
    return;
  }
  esp_now_register_recv_cb(onDataReceiver);
  tone(PIEZO_PIN, 4000, 30);
}

void loop() {}
