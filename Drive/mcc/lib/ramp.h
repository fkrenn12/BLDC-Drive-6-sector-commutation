#pragma once

#include <stdlib.h> 
#include <inttypes.h>

typedef struct _ramp{
    int16_t out;
    int16_t in;
    int16_t upstep;
    int16_t downstep;
    int16_t interval;
    int16_t counter;
}TRamp;

void ramp_init(TRamp* ramp, int16_t out, int16_t in, int16_t upstep, int16_t downstep, int16_t interval);
void ramp_reset(TRamp* ramp);
int16_t ramp_calculate(TRamp* ramp);
