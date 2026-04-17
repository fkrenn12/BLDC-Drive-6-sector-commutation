#include "adc-current-measurement-and-control.h"
#include <stdint.h>

extern TGlobal g;

#define PWM 0x0000      // NO override, PWM is on Pins
#define FLOAT 0x3000    // Override both (PWM_L and PWM_H) with 0 
#define CLAMP 0x3400    // Override PWM_H with 0 and PWM_L with 1

#define VECTOR_CLAMP 0
#define VECTOR_FLOAT 7
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
void commutation(void){
    #ifdef SMART_POWERLAB_HARDWARE
        static const uint8_t ENERGIZED_VECTOR_CLOCKWISE[7] = {0,2,4,3,6,1,5};
        static const uint8_t ENERGIZED_VECTOR_ANTICLOCKWISE[7] = {0,5,1,6,3,4,2};
    #else
        static const uint8_t ENERGIZED_VECTOR_CLOCKWISE[7] = {0,4,6,5,2,3,1};
        static const uint8_t ENERGIZED_VECTOR_ANTICLOCKWISE[7] = {0,1,3,2,5,6,4}; 
    #endif
    static const uint8_t SWAP_B0_B3[8] = {0,0b100,0b010,0b110,0b001,0b101,0b011,0};  
    // DEBUG2_SetHigh();
    g.position_sector =  SWAP_B0_B3[((PORTC & 0b11100000) >> 5)]; // we need to correct wiring of sensor signal to get correct sector
    g.energized_vector = (g.direction_of_rotation==((MOTOR_DIRECTION_INVERTED)? ANTICLOCKWISE: CLOCKWISE)) ? ENERGIZED_VECTOR_CLOCKWISE[g.position_sector]: ENERGIZED_VECTOR_ANTICLOCKWISE[g.position_sector];
    g.energized_vector = (g.mode_selector==MODE_MOTOR_FLOATING)? 7 : g.energized_vector;
    // g.energized_vector = (g.mode_selector==MODE_MOTOR_BLOCKED)? 0 : g.energized_vector;
    g.energized_vector = (g.mode_selector==MODE_MOTOR_BLOCKED)? 7 : g.energized_vector;
    if (COMMUTATE == 1) PWM_override(g.energized_vector); 
    // DEBUG2_SetLow();
}

void sector_counting(void){
   volatile static uint8_t previous_position_sector = 0;
   // counting sectors for speed measurement
   g.speed.sectors_counted = (previous_position_sector != g.position_sector)? g.speed.sectors_counted+1 : g.speed.sectors_counted;
   previous_position_sector = (previous_position_sector != g.position_sector)? g.position_sector : previous_position_sector; 
}
 
#define TICK_US                 PWM_PERIODE_MICROSECOND     // ISR-Periode in µs
#define NO_EDGE_TIMEOUT_TICKS   (uint16_t)(20000u / TICK_US) // ~20 ms Timeout

void PWMInputSampler(void)
{
    static uint8_t   prev_level = 0;
    static uint16_t  ticks_in_period = 0;
    static uint16_t  high_ticks_in_period = 0;

    uint8_t level = PWM_Momentum_GetValue();
    uint8_t rising_edge = ((prev_level==0) && (level==1)); 

    ticks_in_period = (ticks_in_period < UINT16_MAX)? ticks_in_period + 1 : ticks_in_period;  
    high_ticks_in_period = (level == 1) ? high_ticks_in_period + 1 : high_ticks_in_period;
    
    // rising edge?
    if (rising_edge){
        if (ticks_in_period > 0) {
            // new periode detected: calculate value
            uint16_t temp = (((2047 << 4) / ticks_in_period) * high_ticks_in_period) >> 4;
            g.input.momentum = (temp > 2047)? 2047 : temp;
        }
        // start new periode
        ticks_in_period = 0;
        high_ticks_in_period = 0;
    }
    else{
        // no rising edge: timeout detection
        if (ticks_in_period >= NO_EDGE_TIMEOUT_TICKS) {
            // timeout detected - no pwm periode signal: reset values
            ticks_in_period = 0;
            high_ticks_in_period = 0;
            g.input.momentum = 0;
        }
    }
    prev_level = level;
}

uint16_t ADC_Result(enum ADC_CHANNEL channel)
{
    ADC1_IndividualChannelInterruptDisable(channel);        //temporarely disable the interrupt from the channel
    uint16_t result = ADC1_ConversionResultGet(channel);    //save the result of the buffer in the variable "result"
    ADC1_IndividualChannelInterruptEnable(channel);         //enable the interrupt from the channel
    return result;
}

void current_controller(void){
    int16_t duty_cycle;
    // current limiter 
    g.current.ref = (g.current.ref > g.current.limit)? g.current.limit : g.current.ref;
    g.current.ref = (g.current.ref < -g.current.limit)? -g.current.limit : g.current.ref;
    if ((g.mode_selector==MODE_MOTOR_FLOATING) || (g.mode_selector==MODE_MOTOR_BLOCKED))
        duty_cycle = PIController_Compute(&g.current.controller, g.current.ref, 0);
    else 
        duty_cycle = PIController_Compute(&g.current.controller, g.current.ref, g.current.value);
    
    g.direction_of_rotation = (duty_cycle >= 0)? CLOCKWISE : ANTICLOCKWISE;

    // fixed duty for testing and debugging
    // duty_cycle = 4080;  // fixed duty
    // g.direction_of_rotation = 1;

    // for testing and debugging
    // if (g.direction_of_rotation == ANTICLOCKWISE) DEBUG2_SetHigh();
    // if (g.direction_of_rotation == CLOCKWISE) DEBUG2_SetLow();
    
    duty_cycle = abs(duty_cycle);
    MDC = (duty_cycle > PWM_MAX_DUTY)?PWM_MAX_DUTY :duty_cycle; // limit duty cycle to 100%
    MDC = ((g.mode_selector==MODE_MOTOR_FLOATING) || (g.mode_selector==MODE_MOTOR_BLOCKED))?0:MDC;
    // adjust adc interrupt trigger time
    PG1TRIGA = MDC >> 1; // PWM_Generator1_ADC_Trigger1 at half of the duty cycle
    PG1TRIGB = (MDC > 1000)?0:(PWM_MAX_DUTY-1000); // switching PWM_Generator1_ADC_Trigger2 to unused periode of duty 
    PG1STATbits.UPDREQ = 1; 
}

void ADC_Callback(enum ADC_CHANNEL channel, uint16_t adcVal)
{   
    // ********************************************************************
    // adc readings for current and current controller
    // ********************************************************************
    // DIG 4095     +A          ┌────┐     
    // DIG 2048      0     ┌────┘    └────┐   
    // DIG 0        -A ────┘              └───
    DEBUG1_SetHigh();
    if (channel == _I1) PWMInputSampler();
    if (channel == _I3) sector_counting();
    #ifdef SMART_POWERLAB_HARDWARE
        if (channel !=_I2_PowerLab) {DEBUG1_SetLow();return;}
    #else
        if (channel != _I2) {DEBUG1_SetLow();return;}
    #endif
    int32_t i1 = (int32_t)ADC_Result(_I1) - (2047 + OFFSET_CURRENT_U); 
    int32_t i2 = (int32_t)adcVal - (2047 + OFFSET_CURRENT_V);
    int32_t i3 = (int32_t)ADC_Result(_I3) - (2047 + OFFSET_CURRENT_W);
    g.current.value  = (PG1IOCONL == PWM)? i1 : g.current.value;
    g.current.value  = (PG2IOCONL == PWM)? i2 : g.current.value;
    g.current.value  = (PG3IOCONL == PWM)? i3 : g.current.value;

    uint8_t valid_current_value = ((PG1IOCONL == PWM) || (PG2IOCONL == PWM) ||(PG3IOCONL == PWM))?1:0; 
    if (!valid_current_value){
        // max value of the three channels is the current value
        g.current.value = (abs(i1) > abs(i2))?abs(i1):abs(i2);
        g.current.value = (abs(i3) > g.current.value)?abs(i3):g.current.value;
    }
    
    #ifdef SMART_POWERLAB_HARDWARE
        g.current.value = (g.direction_of_rotation == CLOCKWISE)? -g.current.value : g.current.value;
    #else
        g.current.value = (g.direction_of_rotation == CLOCKWISE)? g.current.value : -g.current.value;
    #endif

    commutation();
    g.current.value_peak = (g.current.value_peak > abs(g.current.value))? g.current.value_peak : abs(g.current.value);
    g.voltage.value_peak = (g.voltage.value_peak > (int32_t)ADC_Result(_VLINK))? g.voltage.value_peak : (int32_t)ADC_Result(_VLINK);
    if (abs(g.current.value) > abs(g.current.cutoff)) g.current.overflow = 1;
    if ((int32_t)ADC_Result(_VLINK) > g.voltage.cutoff) g.voltage.overflow = 1;
    if (g.current.overflow || g.voltage.overflow) {
        PWM_override(VECTOR_FLOAT);  // immediately off
        g.mode_selector = MODE_MOTOR_FLOATING;
    }
    if (CURRENT_CONTROL == 1) current_controller(); 
    
    DEBUG1_SetLow();
}
// ########################################################################
//                  PWM1 EOC Interrupt Service Routine 
// ########################################################################
void __attribute__ ( ( interrupt, no_auto_psv ) ) _PWM1Interrupt ( void )
{
    // only used to generate a proper trigger signal for ozilloskope measurements
    // DEBUG2_SetHigh();
    IFS4bits.PWM1IF = 0;
    // DEBUG2_SetLow();
}