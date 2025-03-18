#include <Arduino.h>
#include "glob.h"
#include "network.h"
#include "servers.h"
#include "motor_control.h"
#include <ArduinoJson.h>
#define IN10 19   // Voltage of battary

AsyncWebServer server2(83);
StaticJsonDocument<200> voltageJson;

const float referenceVoltage = 3.3; // Опорное напряжение ESP32 (может потребовать калибровки)
const float r1 = 47.0; // Значение R1 в кОм
const float r2 = 10.0;

char *getVoltageJson() {
  int sensorValue = analogRead(IN10); // Читаем значение ADC (0-4095)
  float voltageOut = (sensorValue / 4095.0) * referenceVoltage; // Преобразуем в напряжение на пине
  float batteryVoltageRaw = voltageOut / (r2 / (r1 + r2)); // Восстанавливаем напряжение батареи
  float batteryVoltageCorrected = 1.924 * batteryVoltageRaw - 16.774; // Коррекция

  voltageJson["voltage"] = voltageOut;
  voltageJson["battery"] = batteryVoltageCorrected;

  static char jsonBuffer[256];
  serializeJson(voltageJson, jsonBuffer);

  return jsonBuffer;
}

void testMotors() {
    goForward(100);
    delay(500);
    stop();
    delay(500);
}

void setup() {
    Serial.begin(115200);
    Serial.println("Starting...");

    initMotorPins();
    initMotors();
    Serial.println("Setup motors complete");

    // testMotors();

    if (!initWiFi()) {
        Serial.println("WiFi connection failed, restarting...");
        ESP.restart();
    }

    // setupCamera();

    // webSocket.begin();
    // webSocket.onEvent(onWebSocketEvent);
    // Serial.println("WebSocket server started");


    wsCarInput.onEvent(onAsyncWebSocketEvent);
    server.addHandler(&wsCarInput);
    server.begin();
    Serial.println("Async WebSocket server started");


    pinMode(IN10, INPUT);
    server2.on("/voltage", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(200, "application/json", getVoltageJson());
    });
    

    server2.begin(); // Запускаем сервер
    // xTaskCreatePinnedToCore(streamTask, "StreamTask", 8192, NULL, 2, NULL, 1);
}

void loop() {
    // webSocket.loop();
    taskYIELD();
}