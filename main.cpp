#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <time.h>
#include "pico/stdlib.h"
#include "hardware/rtc.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "libs/DigitalGPIO.h"
#include "libs/WiFiConnection.h"
#include "pico/cyw43_arch.h"
#include "lwip/tcp.h"
#include <FreeRTOS.h>
#include <task.h>

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments

static char ssid[] = "OPMIKAEL";
static char wifipassword[] = "pellemiljoona";

static TaskHandle_t xConnectionTaskHandle = NULL;
static TaskHandle_t xButtonInputTaskHandle = NULL;
static TaskHandle_t xSleepTaskHandle = NULL;

static void vConnenctionTestTask(void *pvParameters)
{
    WiFiConnection connection(ssid, wifipassword);
    static tcp_pcb taskPcb;
    POSTRequestData reqData;
    std::string jsonData;
    int sample_nr = 0;
    int boot_id;
    int device_id = 1;
    const int adc_pin = 28;
    uint16_t adc_read_result;
    static float battVoltage;
    static const float resistorVal = 100000.0f;

    reqData.ipAddress = "192.168.65.42";
    reqData.port = 8080;

    srand(time(NULL));
    boot_id = rand() % 1000 + 1;

    adc_init();
    adc_gpio_init(adc_pin);
    adc_select_input(2);

    while (true)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        if (!connection.isConnected())
        {
            connection.connectToAP();
            printf("Connected: %d\n", connection.isConnected());
        }
        else
        {
            printf("Already connected\n");
        }

        adc_read_result = adc_read();

        battVoltage = (adc_read_result / (resistorVal + resistorVal)) * resistorVal;

        printf("Battery Voltage: %f \n", battVoltage);
        printf("Boot_id: %i\n", boot_id);

        jsonData = connection.generatePostJson(device_id, boot_id,sample_nr, 50, battVoltage);

        printf("JSONDATA: %s\n", jsonData.c_str());

        reqData.bodyString = jsonData;

        printf("BODYSTRING: %s\n", reqData.bodyString.c_str());
        printf("IP ADDR: %s\n", reqData.ipAddress);
        printf("IP PORT: %i\n", reqData.port);

        printf("Starting send...\n");

        connection.sendPostRequest(&taskPcb, &reqData);

        sleep_ms(2000);

        sample_nr++;
    }
}

static void vButtonInputTask(void *pvParameters)
{
    const uint switchPin = 14;

    DigitalGPIO switchGPIO(switchPin, true, true, true);

    while (true)
    {
        if (switchGPIO.read())
        {
            printf("Read true \n");
            xTaskNotifyGive(xConnectionTaskHandle);
        }
        else
        {
            printf("Read false \n");
        }
        vTaskDelay(15);
    }
}
static void sleep_callback(void) {
    printf("RTC woke us up\n");
}


extern "C" {
    static void irq_callback(uint gpio, uint32_t events) {
    BaseType_t xHigherPriorityTaskWoken;

    xHigherPriorityTaskWoken = pdFALSE;

    vTaskNotifyGiveFromISR(xConnectionTaskHandle, &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
}


static void vSleepTask(void *pvParameters)
{
    while (true)
    {
        printf("Sleeping......\n");

        sleep_ms(2000);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

static void vIRQInit(void) {
    gpio_set_irq_enabled_with_callback(14, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &irq_callback);
}

int main()
{
    stdio_init_all();

    //xTaskCreate(vButtonInputTask, "vButtonInputTask", 256, NULL, tskIDLE_PRIORITY + 1, &xButtonInputTaskHandle);
    xTaskCreate(vConnenctionTestTask, "vConnTask", 2048, NULL, tskIDLE_PRIORITY + 1, &xConnectionTaskHandle);
    xTaskCreate(vSleepTask, "vSleepTask", 256, NULL, tskIDLE_PRIORITY + 1, &xSleepTaskHandle);
    vIRQInit();

    vTaskStartScheduler();

    while (1)
    {
    };

    return 0;
}
