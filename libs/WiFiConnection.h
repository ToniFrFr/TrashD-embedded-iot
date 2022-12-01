#ifndef _WIFI_CONNECTION_H
#define _WIFI_CONNECTION_H

#include <stdio.h>
#include <string>
#include <time.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"


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
    void sendTCPRequest();
    int latestErrorCode();
    ~WiFiConnection();
};




#endif