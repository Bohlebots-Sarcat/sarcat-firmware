#include "espOTA.h"

espOTA::espOTA(const char* ssid, const char* password) : _ssid(ssid), _password(password) {}

void espOTA::begin() {
    WiFi.begin(_ssid, _password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    ArduinoOTA.begin();
}

void espOTA::handle() {
    ArduinoOTA.handle();
}
