#include <ArduinoJson.h>
#include "servers.h"
#include "motor_control.h"
#include "global.h"

// Асинхронный сервер и WebSocket - полная версия для управления
AsyncWebServer server(82);
AsyncWebSocket wsCarInput("/RoverControl");

// Глобальные переменные (объявление)
volatile int8_t controlClientId = -1;
uint8_t speed = 100;
StaticJsonDocument<200> doc;


void handleWebSocketMessage(AsyncWebSocketClient *client, uint8_t *payload, size_t length) {
    
    // Парсинг JSON
    DeserializationError error = deserializeJson(doc, payload, length);

    if (error) {
        Serial.printf("JSON parsing failed: %s\n", error.c_str());
        return;
    }

    // Извлечение данных
    const char *key = doc["key"];
    const char *value = doc["value"];
    
    if (key && value) {
        Serial.printf("Received: key=%s, value=%s\n", key, value);

        if (strcmp(key, "MoveCar") == 0) {
            Serial.printf("Car moving %s\n", value);
            if (strcmp(value, "n") == 0) {
                goForward(speed);
                Serial.println("Car moving forward");
            } else if (strcmp(value, "nw") == 0) {
                goForwardLeft(speed);
                Serial.println("Car moving nw");
            } else if (strcmp(value, "ne") == 0) {
                goForwardRight(speed);
                Serial.println("Car moving ne");
            } else if (strcmp(value, "w") == 0) {
                goLeft(speed);
                Serial.println("Car moving left");
            } else if (strcmp(value, "e") == 0) {
                goRight(speed);
                Serial.println("Car moving right");
            } else if (strcmp(value, "s") == 0) {
                goBackward(speed);
                Serial.println("Car moving backward");
            } else if (strcmp(value, "sw") == 0) {
                goBackwardLeft(speed);
                Serial.println("Car moving sw");    
            } else if (strcmp(value, "se") == 0) {
                goBackwardRight(speed);
                Serial.println("Car moving se");
            } else if (strcmp(value, "stop") == 0) {
                stop();
                Serial.println("Car stopped");
            } else {
                Serial.println("Unknown direction");
            }
           // Отправка ответа клиенту
            // JsonDocument response = StaticJsonDocument<400>(); // Размер буфера 200 байт
            // // JsonDocument response;  // Статический буфер для ответа
            // response["status"] = "success";
            // response["key"] = key;
            // response["value"] = value;

            // String jsonResponse;
            // serializeJson(response, jsonResponse);
            // webSocket.sendTXT(num, jsonResponse); 
        }
    }
}


void onAsyncWebSocketEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) {
    switch (type) {
        case WS_EVT_CONNECT:
            Serial.printf("Client %u connected to %s\n", client->id(), server->url());
            if (strcmp(server->url(), "/RoverControl") == 0) {
                controlClientId = client->id(); // Устанавливаем ID клиента для камеры
            }
            break;
        case WS_EVT_DISCONNECT:
            Serial.printf("Client %u disconnected from %s\n", client->id(), server->url());
            if (controlClientId == client->id()) {
                controlClientId = -1;
            }
            break;
        case WS_EVT_DATA:
            handleWebSocketMessage(client, data, len);
            break;
        default:
            break;
    }
}

