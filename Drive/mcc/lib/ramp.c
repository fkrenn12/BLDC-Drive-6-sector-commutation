#include "ramp.h"

void ramp_init(TRamp* ramp, int32_t value, int32_t target, int32_t upstep, int32_t downstep, int32_t interval){
    ramp->value = value;
    ramp->target = target;
    ramp->upstep = upstep;
    ramp->downstep = downstep;
    ramp->interval = interval;
    ramp->counter = 0;
}

void ramp_reset(TRamp* ramp){
    ramp->value = ramp->target;
    ramp->counter = 0;
}

int32_t ramp(TRamp* ramp){
    if (++ramp->counter % ramp->interval == 0){  
        ramp->counter = 0;
        if (ramp->target > ramp->value){
            ramp->value += ramp->upstep;
            ramp->value = (ramp->value > ramp->target)? ramp->target : ramp->value; // upper border
        }
        else if (ramp->target < ramp->value){
            ramp->value -= ramp->downstep;
            ramp->value = (ramp->value < ramp->target)? ramp->target : ramp->value; // lower border
        }
    }
    return ramp->value;
}
