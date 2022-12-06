#include <stdio.h>
#include <string>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "libs/DigitalGPIO.h"
#include "libs/WiFiConnection.h"
#include "pico/cyw43_arch.h"
#include "lwip/tcp.h"
#include <FreeRTOS.h>
#include <task.h>

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments

static char ssid[] = "ssid";
static char wifipassword[] = "password";

static void vConnenctionTestTask(void * pvParameters) {
    WiFiConnection connection(ssid, wifipassword);
    static tcp_pcb taskPcb;
    POSTRequestData reqData;
    int sample_nr = 0;

    reqData.ipAddress = "192.168.10.126";
    reqData.port = 8080;

    while(true) {
        if(!connection.isConnected()) {
            connection.connectToAP();
            printf("Connected: %d\n", connection.isConnected());
        } else {
            printf("Already connected\n");
        }
        
        connection.generatePostJson(sample_nr, 50, 50, reqData);

        connection.sendPostRequest(&taskPcb, &reqData);

        sleep_ms(2000);

        sample_nr++;
        
        vTaskDelay(1000);
    }
}

static void vBlinkLedTask(void * pvParameters) {
    const uint blinkingLed = CYW43_WL_GPIO_LED_PIN;
    bool ledState = false;
    DigitalGPIO ledPin(blinkingLed, false, false);
    while(true) {
        ledState = !ledState;

        ledPin.write(ledState);

        printf("Current led state: %d \n", ledState);

        vTaskDelay(1000);
    }
}


int main()
{
    stdio_init_all();

    xTaskCreate(vBlinkLedTask, "vBlinkLedTask", 256, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(vConnenctionTestTask, "vConnTask", 512, NULL, tskIDLE_PRIORITY + 1, NULL);
    
    vTaskStartScheduler();

    while(1){};
    
    return 0;
}
