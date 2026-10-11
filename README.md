# Start here

Setup, timing and reading the sensor are already done for you.

- `ADC_task.cpp`: convert `raw_adc` (0 to 4095) into `temperature_c` using your circuit and thermistor data.
- `CAN_task.cpp` is where you encode and send the temperature using MSM_CAN.

CAN is already initialised. Use the packing helpers and `send_msg()` described
in the [MSM_CAN API](https://github.com/ManchesterStingerMotorsports/johnson-node/blob/main/libraries/MSM_CAN/MSM_CAN.hpp).
For temperature, multiply degrees C by 10: **25.0 C -> 250**.
Complete the marked section in each task.

You do not need to understand SPI, FreeRTOS or mutexes.
Optional build instructions are in `provided/BUILD.md`.

Organiser: the ADC helper uses CS C and input IN1. Adjust `provided/adc.cpp`
if the board uses a different connection.
