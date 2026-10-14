#include "challenge.h"
#include "MSM_CAN.hpp"
#include <cmath>

void can_send_temperature(float temperature_c)
{
    MSM_CAN::TxFrame frame{};
    frame.id = 0x600;
    frame.length = 2;

    //Error checking to ensure results out of range are not sent
    if ((temperature_c < 0.0) || (temperature_c > 100.0)) {
        //Code to send an SCS error here
    }
    
    //integer 10x value of temperature_c, will always round down at 1dp (25.19 -> 251), level of precision is acceptable.
    int16_t temperature_10x = temperature_c * 10;
    //Convert to 16 bit signed, split into 2 bytes for payload prep. Frame uses unsigned int, convert signed directly and CAN will change it back?

    uint16_t test = 23;
    MSM_CAN::pack_u16(frame.data, 2, test)
    
    //Packing helper for int16_t -> uint8_t (from MSM_CAN.hpp):
    //inline void pack_i16(uint8_t data[8], uint8_t index, int16_t value)
    //{
    //    pack_u16(data, index, static_cast<uint16_t>(value));
    //}
    MSM_CAN::pack_i16(frame.data, 0, temperature_10x);
    //Packing from index 0 to ensure both bytes of data are included.
    MSM_CAN::send_msg(frame);
    //Schedule unecessary as framework calls function per ADC sample.
}