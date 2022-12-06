#ifndef _WIFI_CONNECTION_H
#define _WIFI_CONNECTION_H

#include <stdio.h>
#include <string>
#include <time.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "TCPRequest.h"
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
    void generatePostJson(int samplenr,int distance, int batteryLevel, POSTRequestData &postData);
    void sendPostRequest(tcp_pcb *tpcb, POSTRequestData *data);
    int latestErrorCode();
    ~WiFiConnection();
};




#endif