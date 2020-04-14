#include <Arduino.h>
#include <WiFi.h>
#include <MQTT.h>

#include <constants.h>
#include <connect.h>

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
}

void loop() {
  client.loop();

  if (!client.connected()) {
    connect(Serial, net, client);
  }

  if(millis()-reportTimer >= REPORT_FREQ){
    reportTimer=millis();
    client.publish(String(MQTT_ID)+"/read", String(analogRead(A0)));
  }

  if(( millis()-alarmTimer >= MAX_ALARM_FREQ)){    
    alarmTimer = millis();
    client.publish(String(MQTT_ID)+"/alarm", "Warning!");
    delay(500);
  }
}