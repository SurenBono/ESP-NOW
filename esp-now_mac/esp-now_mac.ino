// Find device mac address for access control 

#include <ESP8266WiFi.h>

void setup(){

Serial.begin(9600);
Serial.print("MAC Address:  ");
Serial.println(WiFi.macAddress());
}

void loop(){}
