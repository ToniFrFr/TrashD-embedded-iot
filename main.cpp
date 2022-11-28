#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "libs/DigitalGPIO.h"
#include <FreeRTOS.h>
#include <task.h>
#include "lorawan-lib/include/pico/lorawan.h"
#include "lorawan-lib/loraconfig.h"

const struct lorawan_sx1276_settings sx1276_settings = {
    .spi = {
        .inst = PICO_DEFAULT_SPI_INSTANCE,
        .mosi = PICO_DEFAULT_SPI_TX_PIN,
        .miso = PICO_DEFAULT_SPI_RX_PIN,
        .sck  = PICO_DEFAULT_SPI_SCK_PIN,
        .nss  = 8
    },
    .reset = 9,
    .dio0  = 7,
    .dio1  = 10
};

const struct lorawan_otaa_settings otaa_settings = {
    .device_eui   = LORAWAN_DEVICE_EUI,
    .app_eui      = LORAWAN_APP_EUI,
    .app_key      = LORAWAN_APP_KEY,
    .channel_mask = LORAWAN_CHANNEL_MASK
};

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
