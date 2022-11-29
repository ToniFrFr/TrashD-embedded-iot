#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "libs/DigitalGPIO.h"
#include <FreeRTOS.h>
#include <task.h>

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments

#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

void vBlinkLedTask(void * pvParameters) {
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    bool ledState = false;
    DigitalGPIO ledPin(LED_PIN, false, false);

    lorawan_init_otaa(&sx1276_settings, LORAWAN_REGION, &otaa_settings);

    while(true) {
        ledState = !ledState;

        ledPin.write(ledState);

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
