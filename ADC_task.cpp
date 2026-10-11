#include "challenge.h"
#include <cmath>

// Runs once a second. read_adc() gives you a number from 0 to 4095.
bool adc_read(float &temperature_c)
{
    const float raw_adc = read_adc();
    if (!std::isfinite(raw_adc)) return false;
    temperature_c = NAN;

    // your calculation goes here: convert raw_adc into temperature_c.

    return std::isfinite(temperature_c);
}
