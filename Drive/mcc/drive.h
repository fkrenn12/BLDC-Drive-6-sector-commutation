#pragma once 

#include <stdlib.h> 
#include <inttypes.h>
#include "global.h"

void Drive_init(void);
void Drive_stop(void);
int16_t Drive_setSpeedRpm(int16_t rpm);
void Drive_setDirection(uint8_t direction);
int16_t Drive_getSpeedRpm(void);
void Drive_setCurrentLimit(uint16_t currentLimitmA);
void Drive_resetCurrentLimit();
uint16_t Drive_getState(void);

