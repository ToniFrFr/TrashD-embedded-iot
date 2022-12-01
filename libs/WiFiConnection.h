#ifndef _WIFI_CONNECTION_H
#define _WIFI_CONNECTION_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include <string>

class WiFiConnection
{
private:
    std::string ssid;
    std::string password;
    int errorCode;
    bool connected;
public:
    WiFiConnection(std::string ssid, std::string password);
    bool isConnected();
    void connectToAP();
    void disconnect();
    int latestErrorCode();
    ~WiFiConnection();
};




#endif