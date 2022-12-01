#include "WiFiConnection.h"

WiFiConnection::WiFiConnection(std::string ssid, std::string password) : ssid(ssid), password(password), errorCode(0), connected(false) 
{

}

WiFiConnection::~WiFiConnection()
{
    
}
void WiFiConnection::connectToAP() {
    int errorCode = 0; 
    
    errorCode = cyw43_arch_init();
    if(errorCode != 0) {
        this->errorCode = errorCode;
    } else {
        cyw43_arch_enable_sta_mode();

        errorCode = cyw43_arch_wifi_connect_timeout_ms(this->ssid.c_str(), this->password.c_str(), CYW43_AUTH_WPA2_AES_PSK, 30000);

        if(errorCode != 0) {
            this->connected = false;
            this->errorCode = errorCode;
        } else {
            this->connected = true;
            this->errorCode = errorCode;
        }
    }
}
void WiFiConnection::disconnect() {
    cyw43_arch_deinit();

    this->connected = false;
}
bool WiFiConnection::isConnected() {
    return this->connected;
}
int WiFiConnection::latestErrorCode() {
    return this->errorCode;
}