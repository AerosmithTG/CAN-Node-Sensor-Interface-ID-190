#include "challenge.h"
#include <cmath>

// Match these to the daughterboard: CS C, input IN1 by default.
static constexpr auto CHIP_SELECT = ADC_CS_C;
static constexpr uint8_t CHANNEL = 0;

float read_adc()
{
    const uint8_t tx[2] = {static_cast<uint8_t>(CHANNEL << 3), 0};
    uint8_t rx[2] = {};
    if (adc_spi_transfer(CHIP_SELECT, tx, rx) != ESP_OK) return NAN;
    if (adc_spi_transfer(CHIP_SELECT, tx, rx) != ESP_OK) return NAN;
    return ((static_cast<uint16_t>(rx[0]) << 8) | rx[1]) & 0x0FFF;
}
