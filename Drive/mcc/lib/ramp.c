#include "ramp.h"

void ramp_init(TRamp* ramp, int16_t out, int16_t in, int16_t upstep, int16_t downstep, int16_t interval){
    ramp->out = out;
    ramp->in = in;
    ramp->upstep = upstep;
    ramp->downstep = downstep;
    ramp->interval = interval;
    ramp->counter = 0;
}

void ramp_reset(TRamp* ramp){
    ramp->out = ramp->in;
    ramp->counter = 0;
}

int16_t ramp_calculate(TRamp* ramp){
    if (++ramp->counter % ramp->interval == 0){  
        ramp->counter = 0;
        if (ramp->in > ramp->out){
            ramp->out += ramp->upstep;
            ramp->out = (ramp->out > ramp->in)? ramp->in : ramp->out; // upper border
        }
        else if (ramp->in < ramp->out){
            ramp->out -= ramp->downstep;
            ramp->out = (ramp->out < ramp->in)? ramp->in : ramp->out; // lower border
        }
    }
    return ramp->out;
}
