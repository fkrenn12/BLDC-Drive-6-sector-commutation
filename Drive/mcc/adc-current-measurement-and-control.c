#include "adc-current-measurement-and-control.h"

extern TGlobal g;

uint16_t ADC_Result(enum ADC_CHANNEL channel)
{
    ADC1_IndividualChannelInterruptDisable(channel);        //temporarely disable the interrupt from the channel
    uint16_t result = ADC1_ConversionResultGet(channel);    //save the result of the buffer in the variable "result"
    ADC1_IndividualChannelInterruptEnable(channel);         //enable the interrupt from the channel
    return result;
}

void current_controller(void){
    volatile static uint32_t count=0;
    if (++count % 10 == 0){  // every 10th calling
        count = 0;
        // current limiter 
        g.current.ref = (g.current.ref > g.current.limit)? g.current.limit : g.current.ref;
        g.current.ref = (g.current.ref < -g.current.limit)? -g.current.limit : g.current.ref;
        int16_t duty_cycle = PIController_Compute(&g.current.controller, g.current.ref, g.current.value);
        g.direction_of_rotation = (duty_cycle >= 0)? CLOCKWISE : ANTICLOCKWISE;
        duty_cycle = abs(duty_cycle);
        MDC = (duty_cycle > PWM_MAX_DUTY)? PWM_MAX_DUTY : duty_cycle; // limit duty cycle to 100%
        // adjust adc interrupt trigger time
        PG1TRIGA = (MDC > 8000)? MDC-100 : MDC+100; //PWM_Generator1_ADC_Trigger1
        PG1TRIGB = (MDC > 8000)? 0 : 8000; //PWM_Generator1_ADC_Trigger2
        PG1STATbits.UPDREQ = 1; 
    }
}

void ADC_Callback(enum ADC_CHANNEL channel, uint16_t adcVal)
{   
    // ********************************************************************
    // adc readings for current and current controller
    // ********************************************************************
    // DIG 4095     +A          ┌────┐     
    // DIG 2048      0     ┌────┘    └────┐   
    // DIG 0        -A ────┘              └───

    // DEBUG1_SetHigh();
    // requirement: channel I2 must be the last channel to be readed 
    // results of _I1 and _I3 are already measured 
    
    #ifdef SMART_POWERLAB_HARDWARE
        if (channel !=_I2_PowerLab) {/*DEBUG1_SetLow();*/return;}
    #else
        if (channel != _I2) {/*DEBUG1_SetLow();*/return;}
    #endif
    g.current.value  = abs((PG2IOCONL == PWM)? ((int32_t)adcVal - 2048) : g.current.value);
    g.current.value  = abs((PG1IOCONL == PWM)? ((int32_t)ADC_Result(_I1) - 2048) : g.current.value);
    g.current.value  = abs((PG3IOCONL == PWM)? ((int32_t)ADC_Result(_I3) - 2048) : g.current.value);
    g.current.value = (g.direction_of_rotation == ANTICLOCKWISE)? -g.current.value : g.current.value;
    if (CURRENT_CONTROL == 1) current_controller(); 
    // DEBUG1_SetLow();
}
