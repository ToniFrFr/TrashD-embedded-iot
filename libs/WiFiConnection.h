#ifndef _WIFI_CONNECTION_H
#define _WIFI_CONNECTION_H

#include <stdio.h>
#include <string>
#include <time.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "TCPRequest.h"
#include "lwip/tcp.h"

/**
 * @brief WiFiConnection class is used to as the main method of wireless communication. Class includes all that is required to do the required HTTP functionality of the embedded device
 * Class constructor takes the wireless access point ssid and password into constructor.
 * Private members also include the latest errorCode from the POST request and WiFi connection and the connenction status as boolean
 * Methods should be by naming convention quite self explanatory, but the class has methods for connecting to AP, disconnecting, getting connection status, sending post request and generating JSON object string for the POST request
 * 
 */
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
    std::string generatePostJson(int device_id, int boot_id, int samplenr, int distance, float batterylevel);
    void sendPostRequest(tcp_pcb *tpcb, POSTRequestData *data);
    int latestErrorCode();
    ~WiFiConnection();
};




#endif