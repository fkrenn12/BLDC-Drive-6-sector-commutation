#include "debug.h"

void pulse_on_pin_rb1(void) 
{
    DEBUG_1_SetLow();
    DEBUG_1_SetHigh();
    DEBUG_1_SetHigh();
    DEBUG_1_SetHigh();
    DEBUG_1_SetHigh();
    DEBUG_1_SetLow();
}

void pulse_on_pin_rb0(void) 
{
    DEBUG_0_SetLow();
    DEBUG_0_SetHigh();
    DEBUG_0_SetHigh();
    DEBUG_0_SetHigh();
    DEBUG_0_SetHigh();
    DEBUG_0_SetLow();
}