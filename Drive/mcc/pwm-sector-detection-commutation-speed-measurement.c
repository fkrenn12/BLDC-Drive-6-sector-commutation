#include "pwm-sector-detection-commutation-speed-measurement.h"

#define COMMUTATING 
#define PWM 0x0000      // NO override, PWM is on Pins
#define FLOAT 0x3000    // Override both (PWML and PWM_H) with 0 
#define CLAMP 0x3400    // Override PWM_H with 0 and PWM_L with 1
// Sectorindex 0 and sector 7 are error state and lets CLAMP all PWM's

uint8_t ENERGIZED_VECTOR_CLOCKWISE[7] = {0,4,5,6,1,2,3};
uint8_t ENERGIZED_VECTOR_ANTICLOCKWISE[7] = {0,1,2,3,4,5,6};

uint8_t SECTOR_FROM_HALLPATTERN[8] = {0,6,4,5,2,1,3,0};

// Vector               U           V           W     
// 1                   CLAMP       PWM         FLOAT
// 2                   PWM         CLAMP       FLOAT

uint16_t PWM_U[8] = {CLAMP, CLAMP, CLAMP, FLOAT, PWM,   PWM,   FLOAT, FLOAT};
uint16_t PWM_V[8] = {CLAMP, PWM,   FLOAT, CLAMP, CLAMP, FLOAT, PWM,   FLOAT};
uint16_t PWM_W[8] = {CLAMP, FLOAT, PWM,   PWM,   FLOAT, CLAMP, CLAMP, FLOAT};

// override pwm with PWM_U, PWM_V or PWM_W depending on sector
void PWM_override(uint8_t vector){
    PG1IOCONL = PWM_U[vector];
    PG2IOCONL = PWM_V[vector];
    PG3IOCONL = PWM_W[vector];
}

// ########################################################################
//                  PWM1 EOC Interrupt Service Routine 
// ########################################################################
void __attribute__ ( ( interrupt, no_auto_psv ) ) _PWM1Interrupt ( void )
{
    // time consuming of this ISR: 800nsec@100Mhz 
    // priority 4 (highest)
    // ********************************************************************
    // sector detection, commutation and counting for speed measurement
    // ********************************************************************
    volatile static uint8_t previous_position_sector = 0;
    volatile uint8_t actual_position_sector =  SECTOR_FROM_HALLPATTERN[ (PORTC & 0xE0)>>5];
    // commutating
    g.energized_vector = (g.direction_of_rotation==CLOCKWISE) ? ENERGIZED_VECTOR_CLOCKWISE[actual_position_sector]: ENERGIZED_VECTOR_ANTICLOCKWISE[actual_position_sector];
    g.energized_vector = (g.mode_selector==MODE_SELECTOR_ZERO_MOTOR_FLOATING)? 7 : g.energized_vector;
    g.energized_vector = (g.mode_selector==MODE_SELECTOR_ZERO_MOTOR_BLOCKED)? 0 : g.energized_vector;
    #ifdef COMMUTATING      
        g.energized_vector = (MDC < 100)?0:g.energized_vector;
        PWM_override(g.energized_vector);
    #endif
    // counting sectors for speed measurement
    g.speed.sectors_counted = (previous_position_sector != actual_position_sector)? g.speed.sectors_counted+1 : g.speed.sectors_counted;
    previous_position_sector = (previous_position_sector != actual_position_sector)? actual_position_sector : previous_position_sector;
    IFS4bits.PWM1IF = 0;
}

// for debugging and machine adaption
uint8_t get_actual_sector(void){
    return (PORTC & 0xE0) >> 5;
}