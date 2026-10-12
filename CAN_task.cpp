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
    


    // your code goes here:
    // 1. Use the MSM_CAN packing helper to fill frame.data.
    // 2. Send the message using MSM_CAN::send_msg(frame).
}
