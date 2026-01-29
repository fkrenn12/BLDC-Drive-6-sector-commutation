#include "drive.h"

extern TGlobal g;
// extern PIController pi_current;
// extern PIController pi_speed;

void drive_init(void){

}

void drive_run(void){

}

int32_t drive_set_speed_rpm(int32_t rpm){
     g.speed.ramp.target = (g.speed.ramp.target > g.speed.max)? g.speed.max : rpm;
     return g.speed.ramp.target;
}

void drive_stop(uint8_t clamp){
    g.current.ref = 0;
    g.speed.ref = 0;
    g.speed.ramp.target = 0;
    g.speed.controller_activated = 0;
}

uint8_t drive_state(){
    return 0;
}

void set_current_limit(){
}

void reset_current_limit(){

}

void set_cruise_mode(){

}

void set_momentum_mode(){

}

