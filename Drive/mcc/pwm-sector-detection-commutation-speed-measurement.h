#pragma once

#include <xc.h>
#include "global.h"


#define PWM 0x0000      // NO override, PWM is on Pins
#define FLOAT 0x3000    // Override both (PWML and PWM_H) with 0 
#define CLAMP 0x3400    // Override PWM_H with 0 and PWM_L with 1

void PWM_override(uint8_t energized_vector);
