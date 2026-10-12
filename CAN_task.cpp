#include "challenge.h"
#include "MSM_CAN.hpp"
#include <cmath>

// CAN is already initialised for you. Just use the packing and sending helpers.
// Read the MSM_CAN API comments here:
// https://github.com/ManchesterStingerMotorsports/johnson-node/blob/main/libraries/MSM_CAN/MSM_CAN.hpp
// Pack temperature_c multiplied by 10, then send the message.
void can_send_temperature(float temperature_c)
{
    MSM_CAN::TxFrame frame{};
    frame.id = 0x600;
    frame.length = 2;
    
    //integer 10x value of temperature_c, will always round down at 1dp (25.19 -> 251), level of precision is acceptable.
    int16_t temperature_10x = temperature_c * 10;
    //Convert to 16 bit signed, split into 2 bytes for payload prep. Frame uses unsigned int, convert signed directly and CAN will change it back?

    //Packing helper for int16_t -> uint8_t (from MSM_CAN.hpp):
    //inline void pack_i16(uint8_t data[8], uint8_t index, int16_t value)                 //helper function to pack uint8_t data[8] with a big-endian encoded int16_t
    //{
    //    pack_u16(data, index, static_cast<uint16_t>(value));
    //}
    MSM_CAN::pack_i16(frame.data, 0, temperature_10x);
    MSM_CAN::send_msg(frame);
    // Could be better to use schedule() instead of send_msg() as this would allow us to send the data periodically every second (as the brief states)
    //Schedule unecessary as framework calls function per ADC sample.


    // your code goes here:
    // 1. Use the MSM_CAN packing helper to fill frame.data.
    // 2. Send the message using MSM_CAN::send_msg(frame).
}
