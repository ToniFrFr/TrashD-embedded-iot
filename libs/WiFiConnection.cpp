#include "WiFiConnection.h"

WiFiConnection::WiFiConnection(std::string ssid, std::string password) : ssid(ssid), password(password), errorCode(0), connected(false) 
{
    int errorCode = cyw43_arch_init();
    if(errorCode != 0) {
        this->errorCode = errorCode;
    }
}

WiFiConnection::~WiFiConnection()
{
    cyw43_arch_deinit();
}
void WiFiConnection::connectToAP() {
    cyw43_arch_enable_sta_mode();

    int errorCode = cyw43_arch_wifi_connect_timeout_ms(this->ssid.c_str(), this->password.c_str(), CYW43_AUTH_WPA2_AES_PSK, 30000);

    if(errorCode != 0) {
        this->connected = false;
        this->errorCode = errorCode;
    } else {
        this->connected = true;
        this->errorCode = errorCode;
    }
}
bool WiFiConnection::isConnected() {
    return this->connected;
}
int WiFiConnection::latestErrorCode() {
    return this->errorCode;
}