#include "pwm-sector-detection-commutation-speed-measurement.h"

/*
Vector   U       V       W   
------------------------------
  1     PWM     CLAMP   FLOAT
  2     PWM     FLOAT   CLAMP
  3     FLOAT   PWM     CLAMP
  4     CLAMP   PWM     FLOAT
  5     CLAMP   FLOAT   PWM
  6     FLOAT   CLAMP   PWM
*/

static const uint16_t PWM_U[8] = {CLAMP, PWM,   PWM,   FLOAT, CLAMP, CLAMP, FLOAT,  FLOAT};
static const uint16_t PWM_V[8] = {CLAMP, CLAMP, FLOAT, PWM,   PWM,   FLOAT, CLAMP,  FLOAT};
static const uint16_t PWM_W[8] = {CLAMP, FLOAT, CLAMP, CLAMP, FLOAT, PWM,   PWM,    FLOAT};

// override pwm with PWM_U, PWM_V or PWM_W depending on sector
void PWM_override(uint8_t vector){
    PG1IOCONL = PWM_U[vector];
    PG2IOCONL = PWM_V[vector];
    PG3IOCONL = PWM_W[vector];
}

// ********************************************************************
// sector detection, commutation and counting for speed measurement
// ********************************************************************
void commutation_and_sector_counting(void){
    static const uint8_t ENERGIZED_VECTOR_CLOCKWISE[7] = {0,2,4,3,6,1,5};
    static const uint8_t ENERGIZED_VECTOR_ANTICLOCKWISE[7] = {0,5,1,6,3,4,2};
    volatile static uint8_t previous_position_sector = 0;
    static const uint8_t SWAP_B0_B3[8] = {0,0b100,0b010,0b110,0b001,0b101,0b011,0};  
    g.position_sector =  SWAP_B0_B3[((PORTC & 0xE0)>>5)]; // we need to correct wiring of sensor signal to get correct sector
    // commutating
    g.energized_vector = (g.direction_of_rotation==((MOTOR_DIRECTION_INVERTED)? ANTICLOCKWISE: CLOCKWISE)) ? ENERGIZED_VECTOR_CLOCKWISE[g.position_sector]: ENERGIZED_VECTOR_ANTICLOCKWISE[g.position_sector];
    g.energized_vector = (g.mode_selector==MODE_MOTOR_FLOATING)? 7 : g.energized_vector;
    g.energized_vector = (g.mode_selector==MODE_MOTOR_BLOCKED)? 0 : g.energized_vector;
    if (COMMUTATE == 1) PWM_override(g.energized_vector);
    // counting sectors for speed measurement
    g.speed.sectors_counted = (previous_position_sector != g.position_sector)? g.speed.sectors_counted+1 : g.speed.sectors_counted;
    previous_position_sector = (previous_position_sector != g.position_sector)? g.position_sector : previous_position_sector;
}
// ########################################################################
//                  PWM1 EOC Interrupt Service Routine 
// ########################################################################
void __attribute__ ( ( interrupt, no_auto_psv ) ) _PWM1Interrupt ( void )
{
    // time consuming of this ISR: 800nsec@100Mhz 
    // priority 4 (highest)
    commutation_and_sector_counting();
    IFS4bits.PWM1IF = 0;
}
