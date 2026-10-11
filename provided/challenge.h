#pragma once
#include <cstddef>
#include <cstdint>
#include "esp_err.h"

// The template handles setup, timing and sharing data between tasks.
// These helpers let you focus on the sensor reading and CAN message.
typedef enum { ADC_CS_C = 0, ADC_CS_D = 1 } adc_chip_select_t;

// Internal SPI transfer: two bytes, mode 0, 4 MHz, MSB first.
// Reply is written to rx; return value is an ESP_OK/error status.
esp_err_t adc_spi_transfer(adc_chip_select_t cs,
                           const uint8_t tx[2], uint8_t rx[2]);

// Fill in these two task functions. ADC returns true for a valid reading.
bool adc_read(float &temperature_c);
void can_send_temperature(float temperature_c);

// Raw ADC count (0 to 4095), or NAN if the transfer fails.
float read_adc();
