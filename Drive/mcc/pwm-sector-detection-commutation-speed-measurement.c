#include "pwm-sector-detection-commutation-speed-measurement.h"
#include "debug.h"

#define COMMUTATING 
#define PWM 0x0000      // NO override, PWM is on Pins
#define FLOAT 0x3000    // Override both (PWML and PWM_H) with 0 
#define CLAMP 0x3400    // Override PWM_H with 0 and PWM_L with 1
// Sector 0 and sector 7 are error state and lets CLAMP all PWM's

uint8_t ENERGIZED_SECTOR_CLOCKWISE[7] =     {0,3,1,2,5,4,6};  
uint8_t ENERGIZED_SECTOR_ANTICLOCKWISE[7] = {0,5,4,6,3,1,2};


uint16_t PWM_U[8] = {CLAMP, CLAMP, CLAMP, FLOAT, PWM,   PWM,   FLOAT, FLOAT};
uint16_t PWM_V[8] = {CLAMP, PWM,   FLOAT, CLAMP, CLAMP, FLOAT, PWM,   FLOAT};
uint16_t PWM_W[8] = {CLAMP, FLOAT, PWM,   PWM,   FLOAT, CLAMP, CLAMP, FLOAT};


// override pwm with PWM_U, PWM_V or PWM_W depending on sector
void PWM_override(uint8_t energized_sector){
    PG1IOCONL = PWM_U[energized_sector];
    PG2IOCONL = PWM_V[energized_sector];
    PG3IOCONL = PWM_W[energized_sector];
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
    volatile static uint8_t previous_sector = 0;
    // actual sector detection - swap b0 -b2 because of suboptimal hardware wiring
    volatile uint8_t actual_sector =  (PORTC & 0xE0)>>5;
    // commutating
    g.energized_sector = (g.direction==CLOCKWISE) ? ENERGIZED_SECTOR_CLOCKWISE[actual_sector]: ENERGIZED_SECTOR_ANTICLOCKWISE[actual_sector];
    #ifdef COMMUTATING
        if (g.drive_on)
            PWM_override(g.energized_sector);
        else PWM_override(0);
    #endif
    // counting sectors for speed measurement
    g.speed.sectors_counted = (previous_sector != actual_sector)? g.speed.sectors_counted+1 : g.speed.sectors_counted;
    previous_sector = (previous_sector != actual_sector)? actual_sector : previous_sector;
    IFS4bits.PWM1IF = 0;
}

// for debugging and machine adaption
uint8_t get_actual_sector(void){
    return (PORTC & 0xE0) >> 5;
}