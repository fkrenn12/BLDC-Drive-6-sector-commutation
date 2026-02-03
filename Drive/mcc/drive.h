#pragma once 

#include <stdlib.h> 
#include <inttypes.h>
#include "global.h"

void Drive_Init(void);
void Drive_RunModeMomentum(void);
void Drive_RunForward(void);
void Drive_RunBackward(void);
void Drive_RunModeCruiser(void);
void Drive_Stop(void);
int16_t Drive_setSpeedRpm(int16_t rpm);
int16_t Drive_getSpeedRpm(void);
void Drive_setSoftwareCurrentRef(int16_t currentRef);
void Drive_SetCurrentLimit();
void Drive_ResetCurrentLimit();
uint16_t Drive_getState(void);

