#include "ncSerial.h"

ncSerial::ncSerial(int port) : _port(port), _server(port) {}

void ncSerial::begin() {
    Serial.begin(115200);
    WiFi.begin();

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    delay(100);
    Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());

    _server.begin();
}

void ncSerial::handle() {
    if (_server.hasClient()) {
        _client = _server.available();
        Serial.println("Client connected");
    }

    if (_client.connected()) {
        if (Serial.available() > 0) {
            String message = Serial.readStringUntil('\n');
            _client.println(message);
            Serial.print("Sent message: ");
            Serial.println(message);
        }
    } else {
        _client.stop();
        Serial.println("Client disconnected");
    }
}

template<>
void ncSerial::sendMessage<char const*>(char const* message) {
    if (_client.connected()) {
        _client.println(message);
        Serial.print("Sent message: ");
        Serial.println(message);
    } else {
        Serial.println("Client not connected");
    }
}

template<>
void ncSerial::sendMessage<StringSumHelper>(StringSumHelper message) {
    if (_client.connected()) {
        _client.println(message);
        Serial.print("Sent message: ");
        Serial.println(message);
    } else {
        Serial.println("Client not connected");
    }
}

