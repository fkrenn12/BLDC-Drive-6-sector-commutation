#pragma once

#include <stdlib.h> 
#include <inttypes.h>

typedef struct _ramp{
    int32_t value;
    int32_t target;
    int32_t upstep;
    int32_t downstep;
    int32_t interval;
    int32_t counter;
}TRamp;

void ramp_init(TRamp* ramp, int32_t value, int32_t target, int32_t upstep, int32_t downstep, int32_t interval);
void ramp_reset(TRamp* ramp);
int32_t ramp(TRamp* ramp);
