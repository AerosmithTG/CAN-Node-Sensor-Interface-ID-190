# Start here

The main components for your design are listed below:

• Temperature sensor: NTCS0805E3103JHT SMD (0805) 10k NTC Thermistor, connected to
a thermal source ranging from 0°C to 100°C.
https://www.digikey.com/en/products/detail/vishay-beyschlag-draloric-bc-components/NTCS0805E3103JHT/2237370

• ADC: Texas Instruments ADC124S051CIMM/NOPB SPI ADC. Only one analogue input is
required.
https://www.ti.com/product/ADC124S051#params
Operating temperature range -40:85, sensor has to measure range 0-100, caution about temperature on ADC.

• Microcontroller: ESP32-S3-WROOM-1U & ISO1042 CAN Interface, already installed on
the CAN Data node.
https://documentation.espressif.com/esp32-s3-wroom-1_wroom-1u_datasheet_en.html

For this exercise, transmit temperature using the following CAN message format:

• Standard 11-bit CAN identifier: 0x600.

• Payload length: 2 bytes.

• Temperature encoding: Signed 16-bit integer in tenths of a degree Celsius.

• Byte order: Most significant byte first.

For example, 25.0°C should be represented by the integer 250, giving payload bytes 0x00 and 0xFA.
Additionally, you must use the Manchester Stinger Motorsports CAN Library for its packing and
sending functions. The required files can be found at this link:

https://github.com/ManchesterStingerMotorsports/johnsonnode/blob/main/libraries/MSM_CAN/MSM_CAN.hpp
-Files actually found in "https://github.com/ManchesterStingerMotorsports/johnson-node/tree/main/libraries/MSM_CAN"

Setup, timing and reading the sensor are already done for you.

- `ADC_task.cpp`: convert `raw_adc` (0 to 4095) into `temperature_c` using your circuit and thermistor data.
- `CAN_task.cpp` is where you encode and send the temperature using MSM_CAN.

CAN is already initialised. Use the packing helpers and `send_msg()` described
in the [MSM_CAN API](https://github.com/ManchesterStingerMotorsports/johnson-node/blob/main/libraries/MSM_CAN/MSM_CAN.hpp).
-Files actually found in "https://github.com/ManchesterStingerMotorsports/johnson-node/tree/main/libraries/MSM_CAN"
For temperature, multiply degrees C by 10: **25.0 C -> 250**.
Complete the marked section in each task.

You do not need to understand SPI, FreeRTOS or mutexes.
Optional build instructions are in `provided/BUILD.md`.

Organiser: the ADC helper uses CS C and input IN1. Adjust `provided/adc.cpp`
if the board uses a different connection.
