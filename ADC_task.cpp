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
    //RCon - Resistance of constant resistance comparator resistor in the voltage divider against the thermistor.
    //RThe - Datasheet resistance of thermistor at a temperature of 25 degrees celsius.
    //BetaCon - Datasheet beta constant of thermistor between temps of 25 to 85 degrees celsius, used to calculate temperature from resistance.
    //ThermistorBaselineTemp - Temperature in kelvin (required for temp calculations) at which thermistor reaches its baseline resistance.
    const float RConstant = 10000.0, RThermistor25Baseline = 10000.0;
    const float BetaConstant2585 = 3940.0;
    const float ThermistorBaselineTempKelvin = 25.0 + 273.15;
    
    
    //float VOut = 5.25 * raw_adc / 4095.0, simplified steps from flowchart diagram but line here for clarity.
    float RThermistor = RConstant * 4095 / raw_adc - RConstant;
    //Temperature calculated using Beta Parameter Model (sufficient for this application over Steinhart-Hart model as lacking required measurements and level of precision not required.)
    //cmath power and natural log used, optimised math performance by performing minimal amount of resource intensive exponentiation and logarithms.
    temperature_c = pow(((log(RThermistor / RThermistor25Baseline) / BetaConstant2585) + pow(ThermistorBaselineTempKelvin, -1)), -1) - 273.15;

    //Maths code tested seperately outputs expected value for different raw_adcs.

    return std::isfinite(temperature_c);
}

//File size is 2KB, should fit easily within Microcontroller's ROM capacity of 384KB.