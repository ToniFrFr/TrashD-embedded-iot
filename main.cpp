#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "libs/DigitalGPIO.h"
#include "pico/cyw43_arch.h"
#include <FreeRTOS.h>
#include <task.h>

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments

static char ssid[] = "wifi ssid"
static char wifipassword[] = "wifi password"

static void vBlinkLedTask(void * pvParameters) {
    const uint blinkingLed = CYW43_WL_GPIO_LED_PIN;
    bool ledState = false;
    DigitalGPIO ledPin(blinkingLed, false, false);
    while(true) {
        ledState = !ledState;

        ledPin.write(ledState);

        printf("Current led state: %d \n", ledState);

        vTaskDelay(200);
    }
}


int main()
{
    stdio_init_all();

    xTaskCreate(vBlinkLedTask, "vBlinkLedTask", 256, NULL, tskIDLE_PRIORITY + 1, NULL);
    
    vTaskStartScheduler();

    while(1){};
    
    return 0;
}
