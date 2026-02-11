#include "adc-current-measurement-and-control.h"

#define CHANNEL_TEMPERATURE 1
#define CHANNEL_MOMENTUM 6
#define CHANNEL_VLINK 12

const int SOFTWARE_TRIGGERED_CHANNELS[]= {CHANNEL_TEMPERATURE, CHANNEL_MOMENTUM, CHANNEL_VLINK};
extern TGlobal g;

uint16_t ADC_GetSoftwareTriggeredChannel(uint8_t index)
{
    return SOFTWARE_TRIGGERED_CHANNELS[index];
}

void ADC_SoftwareTriggerConversion(uint16_t channel)
{
    ADCON3L = ADCON3L & ~0x1F;              //make sure that there are only numbers between 0 and 31 possible
    ADCON3L = ADCON3L | (channel & 0x1F);   //select the channel
    ADCON3Lbits.CNVRTCH = 1;                //trigger the conversion for an analog input
}

uint16_t ADC_Result(enum ADC_CHANNEL channel)
{
    ADC1_IndividualChannelInterruptDisable(channel);        //temporarely disable the interrupt from the channel
    uint16_t result = ADC1_ConversionResultGet(channel);    //save the result of the buffer in the variable "result"
    ADC1_IndividualChannelInterruptEnable(channel);         //enable the interrupt from the channel
    return result;
}

void ADC_SoftwareTriggerChannelSequencing(void)
{
    static uint8_t index = 0;
    index = (index+1) % sizeof(SOFTWARE_TRIGGERED_CHANNELS);
    ADC_SoftwareTriggerConversion(ADC_GetSoftwareTriggeredChannel(index)); 
}

void current_controller(void){
    volatile static uint32_t count=0;
    if (++count % 10 == 0){  // every 10th calling
        // DEBUG_1_SetHigh();
        count = 0;
        // current limiter 
        g.current.ref = (g.current.ref > g.current.limit)? g.current.limit : g.current.ref;
        g.current.ref = (g.current.ref < -g.current.limit)? -g.current.limit : g.current.ref;
        int16_t duty_cycle = PIController_Compute(&g.current.controller, g.current.ref, g.current.value);
        g.direction_of_rotation = (duty_cycle >= 0)? CLOCKWISE : ANTICLOCKWISE;
        duty_cycle = abs(duty_cycle);
        MDC = (duty_cycle > PWM_MAX_DUTY)? PWM_MAX_DUTY : duty_cycle; // limit duty cycle to 100%
        // adjust adc interrupt trigger time
        PG1TRIGA = (MDC > 8000)? MDC-100 : MDC+100;
        PG1TRIGB = PG1TRIGA;
        PG1STATbits.UPDREQ = 1; 
        // DEBUG_1_SetLow();
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
    // DEBUG_0_SetHigh();
    switch(channel){
        case _I1:
            g.current.value = abs((PG1IOCONL == CLAMP)? ((int32_t)adcVal - 2048) : g.current.value);
            g.current.value = (g.direction_of_rotation == ANTICLOCKWISE)? -g.current.value : g.current.value;
            break;

    #ifdef SMART_POWERLAB_HARDWARE
        case _I2_PowerLab:
    #else
        case _I2:
    #endif
            g.current.value  = abs((PG2IOCONL == CLAMP)? ((int32_t)adcVal - 2048) : g.current.value);
            g.current.value = (g.direction_of_rotation == ANTICLOCKWISE)? -g.current.value : g.current.value;
            if (CURRENT_CONTROL == 1) current_controller(); // channel I2 is the last channel to be read, so, the best place to call the current controller is here.
            break;
        case _I3:
            g.current.value = abs((PG3IOCONL == CLAMP)? ((int32_t)adcVal - 2048) : g.current.value);
            g.current.value = (g.direction_of_rotation == ANTICLOCKWISE)? -g.current.value : g.current.value;           
            break;
        default:
            break;
    }   
    // DEBUG_0_SetLow();
}
