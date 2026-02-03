#include "drive.h"

extern TGlobal g;

void Drive_Init(void){
    ramp_init(&g.speed.ramp, 0, 0, 20, 200, 100); // up 20rpm/100ms;down 200rpm/100ms
    PIController_Init(&g.current.controller,
        double_to_fixed32(CURRENT_CONTROLLER_KP),  // defined in configuration.h 
        double_to_fixed32(CURRENT_CONTROLLER_KI),  // defined in configuration.h
        double_to_fixed32(-3.99951171875),  // -8191 = ANTICLOWISE and max. duty cycle
        double_to_fixed32(3.99951171875));  //  8191 = CLOWISE and max. duty cycle
    
    PIController_Init(&g.speed.controller,
        double_to_fixed32(SPEED_CONTROLLER_KP), // defined in configuration.h 
        double_to_fixed32(SPEED_CONTROLLER_KI), // defined in configuration.h
        double_to_fixed32(-1.0 * CURRENT_USAGE_OF_MAX_CURRENT),  // -2048 max neg current (ANTICLOCKWISE)
        double_to_fixed32(0.99951171875 * CURRENT_USAGE_OF_MAX_CURRENT));  // 2047 max pos current (CLOCKWISE)

    g.current.limit = double_to_fixed32(CURRENT_USAGE_OF_MAX_CURRENT);
    g.mode_selector = MODE_SELECTOR_ZERO_MOTOR_BLOCKED;
}

void Drive_RunForward(void){
    g.direction_request = CLOCKWISE;
}

void Drive_RunBackward(void){
    g.direction_request = ANTICLOCKWISE;
}

void Drive_RunModeMomentum(void){
    g.mode_selector = MODE_SELECTOR_MOMENTUM;
}

void Drive_RunModeCruiser(void){
    g.mode_selector = MODE_SELECTOR_SPEEDCONTROLLER;
}

int16_t Drive_setSpeedRpm(int16_t rpm){
    rpm = (rpm < 0)? 0 : rpm;  // speed can't be negative 
    g.speed.ramp.in = (g.speed.ramp.in > g.speed.max)? g.speed.max : rpm;
    return g.speed.ramp.in;
}

int16_t Drive_getSpeedRpm(void){
    return (g.direction_of_rotation==CLOCKWISE)?g.speed.value: -g.speed.value;
}

void Drive_setSoftwareCurrentRef(int16_t currentRef){
    currentRef = (currentRef > 2047)? 2047 : currentRef;
    currentRef = (currentRef < -2048)? -2048 : currentRef;
    g.current.ref = currentRef;
}

void Drive_Stop(void){
    g.mode_selector = MODE_SELECTOR_ZERO_MOTOR_BLOCKED;
}

uint16_t Drive_getState(void){
    return 0;
}

void Drive_SetCurrentLimit(int16_t currentLimitPercent){
    g.current.limit = double_to_fixed32(currentLimitPercent * 0.01 * CURRENT_USAGE_OF_MAX_CURRENT);
}

void Drive_ResetCurrentLimit(void){
    g.current.limit = double_to_fixed32(CURRENT_USAGE_OF_MAX_CURRENT);
}


