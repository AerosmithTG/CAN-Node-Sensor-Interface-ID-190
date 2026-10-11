# Optional build instructions

Use an ESP-IDF 5.5.x terminal with ESP32-S3 tools installed. From the project folder:

```
idf.py set-target esp32s3
idf.py build
```

To run on hardware: `idf.py -p COMx flash monitor` (replace COMx).
A successful build does not test your circuit or temperature calculation.

## Supplied infrastructure

Based on https://github.com/ManchesterStingerMotorsports/johnson-node at f204d34.
SPI: mode 0, 4 MHz, 16 bits, MSB first. GPIO11/12/13 = MOSI/SCLK/MISO.
CS C: GPIO8, J17 pin 1. CS D: GPIO7, J17 pin 2. Onboard ADCs stay deselected.
CAN: 1 Mbit/s, TX GPIO1, RX GPIO2. FreeRTOS scheduling, sample queue and SPI
mutex are provided, along with the repository's C++ MSM_CAN wrapper.

The wrapper allows ID 0x600 and variable payload lengths. Its packing helpers
use big-endian order: 25.0 C becomes 250, sent as 00 FA. Update the brief to
match this format before distributing the template.
Transmit buffers and completion objects are kept alive until the worker is done.
Receive support is unchanged and is not used by this exercise.

Check messages with a CAN analyser when testing hardware. This is a teaching
scaffold, not an implementation of a vehicle shutdown safety function.
