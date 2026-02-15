#include "drive.h"

extern TGlobal g;

void Drive_init(void){
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

void Drive_setDirection(uint8_t direction){
    // g.input.direction = direction;
}

int16_t Drive_setSpeedRpm(int16_t rpm){   
    IEC0bits.T1IE = 0;
    rpm = (rpm > g.speed.max)? g.speed.max : rpm;
    rpm = (rpm < -g.speed.max)? -g.speed.max : rpm;
    g.input.speedRpm = rpm;
    g.input.speedRpm_timestamp = g.millis;
    IEC0bits.T1IE = 1;
    
    return rpm;
}

int16_t Drive_getSpeedRpm(void){
    IEC0bits.T1IE = 0;
    int16_t rpm  = (g.direction_of_rotation==CLOCKWISE)?g.speed.value: -g.speed.value;
    IEC0bits.T1IE = 1;
    return rpm;
}   

void Drive_stop(void){
    IEC0bits.T1IE = 0;
    g.mode_selector = MODE_MOTOR_BLOCKED;
    IEC0bits.T1IE = 1;
}

uint16_t Drive_getState(void){
    return 0;
}

void Drive_setCurrentLimit(uint16_t currentLimitmA){
    currentLimitmA = (currentLimitmA>CURRENT_MAX_VALUE_MA)?CURRENT_MAX_VALUE_MA:currentLimitmA;
    int32_t limit = double_to_fixed32((double)currentLimitmA/CURRENT_MAX_VALUE_MA);
    IEC0bits.T1IE = 0;
    IEC5bits.ADCAN4IE = 0;
    IEC6bits.ADCAN11IE = 0;
    g.current.limit = limit;
    IEC5bits.ADCAN4IE = 1;
    IEC6bits.ADCAN11IE = 1;
    IEC0bits.T1IE = 1;
}

void Drive_resetCurrentLimit(void){
    int32_t limit = double_to_fixed32(CURRENT_USAGE_OF_MAX_CURRENT);
    IEC0bits.T1IE = 0;
    IEC5bits.ADCAN4IE = 0;
    IEC6bits.ADCAN11IE = 0;
    g.current.limit = limit;
    IEC5bits.ADCAN4IE = 1;
    IEC6bits.ADCAN11IE = 1;
    IEC0bits.T1IE = 1;
}


