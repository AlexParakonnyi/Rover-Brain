#include "servers.h"

#include <ArduinoJson.h>
#include <battery.h>

#include "global.h"
#include "motor_control.h"

// Асинхронный сервер и WebSocket для управления моторами
AsyncWebServer serverMotorController(8082);
AsyncWebSocket wsControlMotors("/RoverControl");

// Асинхронный http сервер для изменения настроек камены и получения данных от
// зарядного устройства
AsyncWebServer serverControlBattery(8083);

// Глобальные переменные (объявление)
volatile int8_t controlClientId = -1;
uint8_t speed = 200;
StaticJsonDocument<512> doc;

void handleMessage(AsyncWebSocketClient *client, uint8_t *payload,
                   size_t length) {
  doc.clear();  // Очистка документа перед использованием
  // Парсинг JSON
  DeserializationError error = deserializeJson(doc, payload, length);

  if (error) {
    Serial.printf("JSON parsing failed: %s\n", error.c_str());
    return;
  }

  if (doc["type"] == "move") {
    JsonObject motors = doc["motors"];

    // Извлечение данных
    setMotor(1, motors["motor1"]["direction"], motors["motor1"]["speed"]);
    setMotor(2, motors["motor2"]["direction"], motors["motor2"]["speed"]);
    setMotor(3, motors["motor3"]["direction"], motors["motor3"]["speed"]);
    setMotor(4, motors["motor4"]["direction"], motors["motor4"]["speed"]);
  }
}

void onAsyncWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
                           AwsEventType type, void *arg, uint8_t *data,
                           size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("Client %u connected to %s\n", client->id(), server->url());
      if (strcmp(server->url(), "/RoverControl") == 0) {
        controlClientId = client->id();
      }
      break;

    case WS_EVT_DISCONNECT:
      Serial.printf("Client %u disconnected from %s\n", client->id(),
                    server->url());
      if (controlClientId == client->id()) {
        controlClientId = -1;
      }
      break;

    case WS_EVT_DATA: {
      AwsFrameInfo *info = (AwsFrameInfo *)arg;
      if (info->final && info->index == 0 && info->len == len) {
        if (info->opcode == WS_TEXT) {
          Serial.printf("Received text data from client %u: ", client->id());
          handleMessage(client, data, len);
        }
      }
      break;
    }

    case WS_EVT_ERROR:
      Serial.printf("WebSocket error for client %u\n", client->id());
      break;

    case WS_EVT_PONG:
      Serial.printf("Received PONG from client %u\n", client->id());
      break;

    default:
      Serial.printf("Unknown WebSocket event: %d\n", type);
      break;
  }
}

void handleVoltageRequest(AsyncWebServerRequest *request) {
  AsyncWebServerResponse *response =
      request->beginResponse(200, "application/json", getVoltageJson());
  response->addHeader("Access-Control-Allow-Origin",
                      "*");  // Разрешает запросы от всех источников
  response->addHeader("Access-Control-Allow-Methods",
                      "GET, OPTIONS");  // Разрешенные методы
  response->addHeader("Access-Control-Allow-Headers",
                      "Content-Type");  // Разрешенные заголовки
  request->send(response);
}

void handleChargingRequest(AsyncWebServerRequest *request) {
  AsyncWebServerResponse *response =
      request->beginResponse(200, "application/json", getChargingJson());
  response->addHeader("Access-Control-Allow-Origin",
                      "*");  // Разрешает запросы от всех источников
  response->addHeader("Access-Control-Allow-Methods",
                      "GET, OPTIONS");  // Разрешенные методы
  response->addHeader("Access-Control-Allow-Headers",
                      "Content-Type");  // Разрешенные заголовки
  request->send(response);
}

void initServers() {
  // Инициализация серверов
  wsControlMotors.onEvent(onAsyncWebSocketEvent);
  serverMotorController.addHandler(&wsControlMotors);
  serverMotorController.begin();
  Serial.println("Async WebSocket server started");

  // xTaskCreatePinnedToCore(streamTask, "StreamTask", 8192, NULL, 2, NULL,
  // 1);

  serverControlBattery.on("/battery", HTTP_GET, handleVoltageRequest);
  // Обработчик OPTIONS для CORS
  serverControlBattery.on(
      "/battery", HTTP_OPTIONS, [](AsyncWebServerRequest *request) {
        // Serial.println("Получен запрос OPTIONS");  // Отладочное сообщение
        AsyncWebServerResponse *response = request->beginResponse(200);
        response->addHeader("Access-Control-Allow-Origin", "*");
        response->addHeader("Access-Control-Allow-Methods", "GET, OPTIONS");
        response->addHeader("Access-Control-Allow-Headers", "Content-Type");
        request->send(response);
      });

  serverControlBattery.on("/charging", HTTP_GET, handleChargingRequest);
  serverControlBattery.on(
      "/charging", HTTP_OPTIONS, [](AsyncWebServerRequest *request) {
        // Serial.println("Получен запрос OPTIONS");  // Отладочное сообщение
        AsyncWebServerResponse *response = request->beginResponse(200);
        response->addHeader("Access-Control-Allow-Origin", "*");
        response->addHeader("Access-Control-Allow-Methods", "GET, OPTIONS");
        response->addHeader("Access-Control-Allow-Headers", "Content-Type");
        request->send(response);
      });

  serverControlBattery.begin();
}
