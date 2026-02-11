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
    g.mode_selector = MODE_MOTOR_BLOCKED;
}

int16_t Drive_setSpeedRpm(int16_t rpm){
    rpm = (rpm < 0)? 0 : rpm;  // speed can't be negative 
    g.input.speedRpm = (rpm > g.speed.max)? g.speed.max : rpm;
    return g.input.speedRpm;
}

int16_t Drive_getSpeedRpm(void){
    return (g.direction_of_rotation==CLOCKWISE)?g.speed.value: -g.speed.value;
}   

void Drive_Stop(void){
    g.mode_selector = MODE_MOTOR_BLOCKED;
}

uint16_t Drive_getState(void){
    return 0;
}

void Drive_SetCurrentLimit(uint16_t currentLimitmA){
    currentLimitmA = (currentLimitmA>CURRENT_MAX_VALUE_MA)?CURRENT_MAX_VALUE_MA:currentLimitmA;
    g.current.limit = double_to_fixed32((double)currentLimitmA/CURRENT_MAX_VALUE_MA);
}

void Drive_ResetCurrentLimit(void){
    g.current.limit = double_to_fixed32(CURRENT_USAGE_OF_MAX_CURRENT);
}


