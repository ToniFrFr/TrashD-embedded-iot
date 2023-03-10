#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <time.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "libs/WiFiConnection.h"
#include "libs/TCPRequest.h"
#include "libs/DigitalGPIO.h"
#include <FreeRTOS.h>
#include <task.h>

extern "C"
{
#include "libs/Drivers/VL53L1X/VL53L1X_UltraLowPower_Driver/VL53L1X_ULP_api.h"
#include "libs/Drivers/VL53L1X/VL53L1X_UltraLowPower_Platform/VL53L1X_ULP_platform.h"
}

// I2C reserves some addresses for special purposes. We exclude these from the scan.
// These are any addresses of the form 000 0xxx or 111 1xxx
bool reserved_addr(uint8_t addr)
{
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

//WiFi Access Point ssid and password
static char ssid[] = "wifissid";
static char wifipassword[] = "wifipassword";

//Task handles for task notifications
static TaskHandle_t xConnectionTaskHandle = NULL;
static TaskHandle_t xButtonInputTaskHandle = NULL;
static TaskHandle_t xSleepTaskHandle = NULL;
static TaskHandle_t xDistanceTaskHandle = NULL;

static int iDetectedObstacle = -1;

/**
 * @brief vConnectionTask, used to handle the connecting functionality, initalizes first the random boot id and then other variables, initializes also the adc pin
 * During the main loop first checks current connection status, attempts to connect if not connected (also has safeguard if in middle of attempting connection).
 * If successfully conneceted, first calculates the current battery level and then generates the post JSON Object string
 * Once POST string generated, send the post string to server and go to sleep
 * 
 * @param pvParameters not used for this task
 */
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
    uint32_t currentTickCount;
    uint32_t connectCallTime;

    //IP address and port for the server
    reqData.ipAddress = "192.168.65.47";
    reqData.port = 4000;

    //Random boot_id generation
    srand(time(NULL));
    boot_id = rand() % 1000 + 1;

    adc_init();
    adc_gpio_init(adc_pin);
    adc_select_input(2);

    while (true)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        currentTickCount = xTaskGetTickCount() * 1000;

        if(currentTickCount - connectCallTime > 32000 && !connection.isConnected()) {
            printf("Calling connect..\n");
            connection.connectToAP();
            connectCallTime = xTaskGetTickCount() * 1000;
        } else {
            printf("Not calling...\n");
        }

        if (connection.isConnected())
        {
            adc_read_result = adc_read();

            //Battery level calculation
            battVoltage = ((adc_read_result / 4095) * 3.3f)*2;

            //POST JSON string generation
            jsonData = connection.generatePostJson(device_id, boot_id, sample_nr, iDetectedObstacle, battVoltage);

            //JSON Object string and the route where to post
            reqData.bodyString = jsonData;
            reqData.addressRoute = "/api/samples";

            printf("Starting send...\n");

            //Sending POST request
            connection.sendPostRequest(&taskPcb, &reqData);

            //Wait for POST request results
            sleep_ms(2000);

            sample_nr++;
        }
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
//IRQ Handler Global Variables
uint32_t CurrentPressTick = 0;
uint32_t LastPressTick = 0;

extern "C"
{
    static void irq_callback(uint gpio, uint32_t events)
    {
        CurrentPressTick = xTaskGetTickCountFromISR();
        BaseType_t xHigherPriorityTaskWoken;

        xHigherPriorityTaskWoken = pdFALSE;

        bool debounce = CurrentPressTick - LastPressTick > 30;

        if(events == GPIO_IRQ_EDGE_FALL && debounce) {
            LastPressTick = xTaskGetTickCountFromISR();
            printf("Edge fall \n");
            vTaskNotifyGiveFromISR(xDistanceTaskHandle, &xHigherPriorityTaskWoken);

            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        } else if(events == GPIO_IRQ_EDGE_RISE && debounce) {
            LastPressTick = xTaskGetTickCountFromISR();
            printf("Edge rise \n");
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}
//Simple sleep task to force light sleep on device when not sending
static void vSleepTask(void *pvParameters)
{
    while (true)
    {
        printf("Sleeping......\n");

        sleep_ms(2000);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// VL53L1X defines

#define VL53L1X_I2C_ADDR 0x29
#define VL53L1X_DIST_TRESHOLD 500

void vGetDistanceTask(void *pvParameters)
{
    bool isI2CInit = false;
    bool isSensInit = false;

    i2c_inst_t *i2c = I2C_PORT;

    uint8_t status;
    uint8_t dataReady;
    uint16_t dev = VL53L1X_I2C_ADDR;

    // for DumpDebugData
    uint8_t measure_status;
    uint16_t est_distance_mm;
    uint16_t sigma_mm;
    uint16_t signal_kcps;
    uint16_t ambient_kcps;

    while (true)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        //Sleep added here for send detection
        sleep_ms(2000);
        // If not initialized, initialize I2C. 400kHz.
        if (!isI2CInit)
        {
            i2c_init(i2c, 400 * 1000);
            gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
            gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
            gpio_pull_up(I2C_SDA);
            gpio_pull_up(I2C_SCL);
            isI2CInit = true;
        }

        // If sensor has not been initialized
        if (!isSensInit)
        {
            sleep_ms(1000);

            status = VL53L1X_ULP_SensorInit(dev);
            if (status)
            {
                // Go here if SensorInit() was not succesful.
                // Shouldn't go here.
            }
            else
            {
                isSensInit = true;
            }

            // Sensor has already been initialized
        }
        else
        {

            // Start ranging
            status = VL53L1X_ULP_StartRanging(dev);
            if (status)
            {
                printf("Start ranging failed with status %u\n", status);
            }

            sleep_ms(50);

            do
            {
                // Check if data is ready
                status = VL53L1X_ULP_CheckForDataReady(dev, &dataReady);
                if (status)
                {
                    printf("Check data ready failed, status %u\n", status);
                }
                // Data is ready
                if (dataReady == 1)
                {

                    // Clear interrupt
                    status = VL53L1X_ULP_ClearInterrupt(dev);
                    if (status)
                    {
                        printf("Clear interrupt failed, status: %u\n", status);
                    }

                    // Read data
                    status = VL53L1X_ULP_DumpDebugData(dev, &measure_status, &est_distance_mm, &sigma_mm, &signal_kcps, &ambient_kcps);
                    if (status)
                    {
                        printf("Dump debug data failed, status: %u\n", status);
                    }
                    printf("Est_distance: %d\n", est_distance_mm);

                    if (est_distance_mm < VL53L1X_DIST_TRESHOLD)
                    {
                        // Obstacle detected, do something with this!
                        printf("\n\nObstacle detected!\n\n");
                        iDetectedObstacle = 1;
                    }
                    else
                    {
                        // No obstacle detected.
                        printf("\n\nNo obstacles detected\n\n");
                        iDetectedObstacle = 0;
                    }
                }
            } while (dataReady != 1);

            // Stop ranging
            status = VL53L1X_ULP_StopRanging(dev);
            if (status)
            {
                printf("Stop ranging failed, status: %u\n", status);
            }
        }

        xTaskNotifyGive(xConnectionTaskHandle);
    }
}

static void vIRQInit(void)
{
    gpio_set_irq_enabled_with_callback(14, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &irq_callback);
}

int main()
{
    stdio_init_all();

    // xTaskCreate(vButtonInputTask, "vButtonInputTask", 256, NULL, tskIDLE_PRIORITY + 1, &xButtonInputTaskHandle);
    xTaskCreate(vConnenctionTestTask, "vConnTask", 2048, NULL, tskIDLE_PRIORITY + 1, &xConnectionTaskHandle);
    xTaskCreate(vSleepTask, "vSleepTask", 256, NULL, tskIDLE_PRIORITY + 1, &xSleepTaskHandle);
    xTaskCreate(vGetDistanceTask, "vGetDistanceTask", 2048, NULL, tskIDLE_PRIORITY + 2, &xDistanceTaskHandle);
    vIRQInit();

    vTaskStartScheduler();

    while (1)
    {
    };

    return 0;
}