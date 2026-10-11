#include "challenge.h"
#include <cmath>

//Desmos analogy of maths and inputs required for temperature calculations.
//https://www.desmos.com/calculator/gdqgglfel8

// Runs once a second. read_adc() gives you a number from 0 to 4095.
bool adc_read(float &temperature_c)
{
    const float raw_adc = read_adc();
    if (!std::isfinite(raw_adc)) return false;
    temperature_c = NAN;

    //Circuit & Thermistor Constants
    const float RConstant = 10000.0, RThermistor25Baseline = 10000.0;
    const float BetaConstant2585 = 3940.0;
    const float ThermistorBaselineTempKelvin = 25.0 + 273.15;
    
    
    //float VOut = 5.25 * raw_adc / 4095.0
    float RThermistor = RConstant * 4095 / raw_adc - RConstant;
    temperature_c = pow(((log(RThermistor / RThermistor25Baseline) / BetaConstant2585) + pow(ThermistorBaselineTempKelvin, -1)), -1) - 273.15;

    //Maths code tested seperately outputs expected value for different raw_adcs.

    return std::isfinite(temperature_c);
}