// ESP-NOW ESP8266 TRANSMITTER

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <SoftwareSerial.h>
#include "TimeLib.h"
#include "TinyGPSPlus.h"

#define time_offset   8

uint8_t peer1[] = {0x80, 0x7D, 0x3A, 0x6E, 0xF7, 0x06};  // D1R1
uint8_t peer2[] = {0x2C, 0xF4, 0x32, 0x57, 0x45, 0xC6};  // D1MINI

byte last_second, Second, Minute, Hour, Day, Month;
int Year;

typedef struct message {
  int W, h, m, s, d, mt, y, M ;
};

TinyGPSPlus gps;
struct message myMessage;
SoftwareSerial NEO(13,15);  //Esp (RX,TX)--> NEO TX(EspRX2,13/D7),NEO RX(EspTX2,15/D8)

void onSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.println(sendStatus);
}

void setup() {

  Serial.begin(9600);
  NEO.begin(9600);
  WiFi.mode(WIFI_STA);
  Serial.println("GPS Tx");
   
  if (esp_now_init() != 0) {
    Serial.println("Reboot GPS Tx ");
    return;

  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

  esp_now_add_peer(peer1, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_add_peer(peer2, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  esp_now_register_send_cb(onSent);
}

void prn_GPS_Local()
{
  while (NEO.available() > 0)
  {
    if (gps.encode(NEO.read()))
    {
      if (gps.time.isValid())
      {
        Hour   = gps.time.hour();
        Minute = gps.time.minute();
        Second = gps.time.second();
      }

      if (gps.date.isValid())
      {
        Day   = gps.date.day();
        Month = gps.date.month();
        Year  = gps.date.year();
      }

      if (last_second != gps.time.second())
      {
        last_second = gps.time.second();
        setTime(Hour, Minute, Second, Day, Month, Year);
        adjustTime(time_offset * SECS_PER_HOUR);

        if (weekday() == 1) {Serial.print("Sunday");}
        if (weekday() == 2) {Serial.print("Monday"); }
        if (weekday() == 3) {Serial.print("Tuesday");}
        if (weekday() == 4) {Serial.print("Wednesday");}
        if (weekday() == 5) {Serial.print("Thursday");}
        if (weekday() == 6) {Serial.print("Friday");}
        if (weekday() == 7) { Serial.print("Saturday");}
          
        Serial.print("  ");
          
        if (isAM()) { myMessage.M = 0;Serial.print("AM ");} else { myMessage.M = 1; Serial.print("PM "); }
  
        Serial.print(hourFormat12());Serial.print(":");
        if (minute() < 10)Serial.print('0'); Serial.print(minute());Serial.print(":");
        if (second() < 10)Serial.print('0'); Serial.print(second());
        Serial.print("  ");
        Serial.print(day());Serial.print("/");Serial.print(month());
        Serial.print("/");Serial.print(year());Serial.println();

      }
    }
  }transmit() ;
}

void transmit() {

  myMessage.W = weekday();
  myMessage.M ;
  myMessage.h  = hourFormat12();
  myMessage.m  = minute();
  myMessage.s  = second();
  myMessage.d  = day();
  myMessage.mt = month();
  myMessage.y  = year();

  esp_now_send(NULL, (uint8_t *) &myMessage, sizeof(myMessage));
  
}

void loop() {
  prn_GPS_Local();
  delay(100);
}
