#ifndef ESP_OTA_H
#define ESP_OTA_H

#include <WiFi.h>
#include <ArduinoOTA.h>

class espOTA {
public:
    espOTA(const char* ssid, const char* password);
    void begin();
    void handle();
private:
    const char* _ssid;
    const char* _password;
};

#endif // REMOTE_UPLOADER_H
