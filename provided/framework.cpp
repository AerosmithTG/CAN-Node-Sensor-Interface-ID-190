// Supplied infrastructure. Candidate work belongs in ADC_task.cpp and CAN_task.cpp.
#include "framework.h"
#include "challenge.h"
#include <cmath>
#include <string.h>
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "MSM_CAN.hpp"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

static const char *TAG = "Challenge";
static QueueHandle_t samples;
static SemaphoreHandle_t spi_mutex;
static spi_device_handle_t adc_devices[2];

// J17: CS C = GPIO8, CS D = GPIO7. Keep onboard ADCs deselected.
static void initialise_spi(void)
{
    const gpio_num_t cs_pins[] = {GPIO_NUM_10, GPIO_NUM_9,
                                 GPIO_NUM_8, GPIO_NUM_7};
    for (size_t i = 0; i < sizeof(cs_pins) / sizeof(cs_pins[0]); ++i) {
        ESP_ERROR_CHECK(gpio_set_level(cs_pins[i], 1));
        ESP_ERROR_CHECK(gpio_set_direction(cs_pins[i], GPIO_MODE_OUTPUT));
    }
    spi_bus_config_t bus{};
    bus.mosi_io_num = GPIO_NUM_11;
    bus.miso_io_num = GPIO_NUM_13;
    bus.sclk_io_num = GPIO_NUM_12;
    bus.quadwp_io_num = -1;
    bus.quadhd_io_num = -1;
    bus.max_transfer_sz = 2;
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &bus, SPI_DMA_DISABLED));
    for (int i = 0; i < 2; ++i) {
        spi_device_interface_config_t device{};
        device.mode = 0;
        device.clock_speed_hz = 4000000;
        device.spics_io_num = (i == ADC_CS_C) ? GPIO_NUM_8 : GPIO_NUM_7;
        device.queue_size = 1;
        ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &device, &adc_devices[i]));
    }
}

esp_err_t adc_spi_transfer(adc_chip_select_t cs,
                           const uint8_t tx[2], uint8_t rx[2])
{
    if ((cs != ADC_CS_C && cs != ADC_CS_D) || tx == NULL || rx == NULL)
        return ESP_ERR_INVALID_ARG;
    if (spi_mutex == NULL || adc_devices[cs] == NULL)
        return ESP_ERR_INVALID_STATE;
    if (xSemaphoreTake(spi_mutex, pdMS_TO_TICKS(100)) != pdTRUE)
        return ESP_ERR_TIMEOUT;
    spi_transaction_t transfer{};
    transfer.flags = SPI_TRANS_USE_TXDATA | SPI_TRANS_USE_RXDATA;
    transfer.length = 16;
    memcpy(transfer.tx_data, tx, 2);
    esp_err_t err = spi_device_transmit(adc_devices[cs], &transfer);
    if (err == ESP_OK) memcpy(rx, transfer.rx_data, 2);
    xSemaphoreGive(spi_mutex);
    return err;
}

static void initialise_can(void)
{
    ESP_ERROR_CHECK(MSM_CAN::init(GPIO_NUM_2, GPIO_NUM_1));
}

static void adc_task(void *arg)
{
    (void)arg;
    TickType_t previous_wake = xTaskGetTickCount();
    for (;;) {
        float temperature_c = NAN;
        if (adc_read(temperature_c) && std::isfinite(temperature_c)) {
            // Pass the latest reading safely to CAN.
            xQueueOverwrite(samples, &temperature_c);
        }
        // Avoid rapid catch-up readings if ADC work takes too long.
        if (xTaskDelayUntil(&previous_wake, pdMS_TO_TICKS(1000)) == pdFALSE) {
            ESP_LOGW(TAG, "ADC work exceeded one second");
            previous_wake = xTaskGetTickCount();
            vTaskDelay(pdMS_TO_TICKS(1000));
            previous_wake = xTaskGetTickCount();
        }
    }
}

static void can_task(void *arg)
{
    (void)arg;
    for (;;) {
        float temperature_c;
        if (xQueueReceive(samples, &temperature_c, portMAX_DELAY) != pdTRUE) continue;
        can_send_temperature(temperature_c);
    }
}

void challenge_start(void)
{
    // Set up shared resources before starting tasks.
    samples = xQueueCreate(1, sizeof(float));
    spi_mutex = xSemaphoreCreateMutex();
    ESP_ERROR_CHECK(samples != NULL && spi_mutex != NULL ? ESP_OK : ESP_ERR_NO_MEM);
    initialise_spi();
    initialise_can();
    ESP_ERROR_CHECK(xTaskCreate(can_task, "CAN_task", 4096, NULL, 3, NULL)
                    == pdPASS ? ESP_OK : ESP_ERR_NO_MEM);
    ESP_ERROR_CHECK(xTaskCreate(adc_task, "ADC_task", 4096, NULL, 2, NULL)
                    == pdPASS ? ESP_OK : ESP_ERR_NO_MEM);
    ESP_LOGI(TAG, "Template ready: fill in the two task files to produce messages");
}
