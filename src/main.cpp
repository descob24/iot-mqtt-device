#include <Arduino.h>

#include <WiFi.h>
#include <MQTT.h>

#include <Wire.h>
#include "ClosedCube_HDC1080.h"

#include <constants.h>
#include <connect.h>

ClosedCube_HDC1080 hdc1080;

WiFiClient net;
MQTTClient client;

long reportTimer = millis();
long alarmTimer = millis();

void setup() {
  
  pinMode(INPUT_BTN, INPUT);

  Serial.begin(115200);
  WiFi.begin(SSID, SSID_PASSWORD);

  client.begin(BROKER, net);

  connect(Serial, net, client);

  hdc1080.begin(0x40);
}

void loop() {
  client.loop();

  if (!client.connected()) {
    connect(Serial, net, client);
  }
  
  if(millis()-reportTimer >= REPORT_FREQ){
    reportTimer=millis();
    client.publish(String(MQTT_ID)+"/Temperature", String(hdc1080.readTemperature()));
    client.publish(String(MQTT_ID)+"/Humidity", String(hdc1080.readHumidity()));
  }

  /*
  if(( millis()-alarmTimer >= MAX_ALARM_FREQ)){    
    alarmTimer = millis();
    client.publish(String(MQTT_ID)+"/alarm", "Warning!");
    delay(500);
  }
  */
}