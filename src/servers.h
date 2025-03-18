#ifndef SERVERS_H
#define SERVERS_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

// Асинхронный сервер и WebSocket
extern AsyncWebServer server;
extern AsyncWebSocket wsCarInput;

// Функции
void handleWebSocketMessage(AsyncWebSocketClient *client, uint8_t *data, size_t len);
void onAsyncWebSocketEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);

#endif