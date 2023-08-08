
#include <ESP8266WiFi.h>
#include <espnow.h>

uint8_t peer1[] = {0x80, 0x7D, 0x3A, 0x6E, 0xF7, 0x06};  // D1R1

void onSent(uint8_t *mac_addr, uint8_t sendStatus) {Serial.println(sendStatus);}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW initialization failed");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_add_peer(peer1, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_register_send_cb(onSent);
}

void loop() {
  String dataToSend = "Beep!";
  esp_now_send(NULL, (uint8_t *) &dataToSend, sizeof(dataToSend));
  delay(500);
}
