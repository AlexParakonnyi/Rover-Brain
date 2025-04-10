#include <Arduino.h>

#include "battery.h"
#include "glob.h"
#include "motor_control.h"
#include "network.h"
#include "servers.h"

// void testMotors() {
//     goForward(100);
//     delay(500);
//     stop();
//     delay(500);
// }

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");

  initChargerPins();
  initMotorPins();
  initMotors();

  Serial.println("Setup motors complete");

  // testMotors();

  if (!initWiFi()) {
    Serial.println("WiFi connection failed, restarting...");
    ESP.restart();
  }

  initServers();
}

void loop() {
  // webSocket.loop();
  taskYIELD();
}