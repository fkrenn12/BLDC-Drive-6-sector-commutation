#include "pwm-sector-detection-commutation-speed-measurement.h"
#include "debug.h"

#define PWM 0x0000      // NO override, PWM is on Pins
#define FLOAT 0x3000    // Override both (PWML and PWM_H) with 0 
#define CLAMP 0x3400    // Override PWM_H with 0 and PWM_L with 1
// Sector 0 and sector 7 are error state and lets CLAMP all PWM's

uint8_t ENERGIZED_SECTOR_CLOCKWISE[7] =     {0,3,1,2,5,4,6};  

// uint8_t ENERGIZED_SECTOR_CLOCKWISE[7] =     {0,1,2,3,4,5,6};     
uint8_t ENERGIZED_SECTOR_ANTICLOCKWISE[7] = {0,5,4,6,3,1,2};

/*
    3 -> 1
    1 -> 2
    2 -> 3
    5 -> 4
    4 -> 5
    6 -> 6
*/
uint16_t PWM_U[8] = {CLAMP, CLAMP, CLAMP, FLOAT, PWM,   PWM,   FLOAT, FLOAT};
uint16_t PWM_V[8] = {CLAMP, PWM,   FLOAT, CLAMP, CLAMP, FLOAT, PWM,   FLOAT};
uint16_t PWM_W[8] = {CLAMP, FLOAT, PWM,   PWM,   FLOAT, CLAMP, CLAMP, FLOAT};

// override pwm with PWM_U, PWM_V or PWM_W depending on sector
void PWM_override(uint8_t energized_sector){
    PG1IOCONL = PWM_U[energized_sector];
    PG2IOCONL = PWM_V[energized_sector];
    PG3IOCONL = PWM_W[energized_sector];
}

// C
static const uint8_t swap_b0_b2_lut[8] = {
    0, // 000 -> 000
    4, // 001 -> 100
    2, // 010 -> 010
    6, // 011 -> 110
    1, // 100 -> 001
    5, // 101 -> 101
    3, // 110 -> 011
    7  // 111 -> 111
};

static inline uint8_t swap_b0_b2_lut3(uint8_t x)
{
    return x;
    return swap_b0_b2_lut[x & 0x07];
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
    DEBUG_0_SetHigh();
    volatile static uint8_t previous_sector = 0;
    // actual sector detection - swap b0 -b2 because of suboptimal hardware wiring
    volatile uint8_t actual_sector =  swap_b0_b2_lut3((PORTC & 0xE0)>>5);
    // commutating
    g.energized_sector = (g.direction==CLOCKWISE) ? ENERGIZED_SECTOR_CLOCKWISE[actual_sector]: ENERGIZED_SECTOR_ANTICLOCKWISE[actual_sector];
    PWM_override(g.energized_sector);
    // counting sectors for speed measurement
    g.speed.sectors_counted = (previous_sector != actual_sector)? g.speed.sectors_counted+1 : g.speed.sectors_counted;
    previous_sector = (previous_sector != actual_sector)? actual_sector : previous_sector;
    IFS4bits.PWM1IF = 0;
    DEBUG_0_SetLow();
}

