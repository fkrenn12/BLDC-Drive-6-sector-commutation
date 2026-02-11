#pragma once 

#include <stdlib.h> 
#include <inttypes.h>
#include "global.h"

void Drive_Init(void);
void Drive_Stop(void);
int16_t Drive_setSpeedRpm(int16_t rpm);
int16_t Drive_getSpeedRpm(void);
void Drive_SetCurrentLimit(uint16_t currentLimitmA);
void Drive_ResetCurrentLimit();
uint16_t Drive_getState(void);

