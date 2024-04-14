//
// Created by Jan Schlegel on 14.04.24.
//

#ifndef ESP_NOW_NCSERIAL_H
#define ESP_NOW_NCSERIAL_H


#include <WiFi.h>

class ncSerial {
public:
    ncSerial(int port);
    void begin();
    void handle();
    template <typename T>
    void sendMessage(T message);
private:
    int _port;
    WiFiServer _server;
    WiFiClient _client;
};



#endif //ESP_NOW_NCSERIAL_H
