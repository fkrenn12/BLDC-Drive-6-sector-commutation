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

void handle_mode_selector_changing(void){
    static uint16_t previous_mode_selector = 0; //g.mode_selector;
    if (g.mode_selector == previous_mode_selector) return;
    // changed
    
    if ((g.mode_selector == MODE_SELECTOR_ZERO_MOTOR_BLOCKED) || (g.mode_selector == MODE_SELECTOR_ZERO_MOTOR_FLOATING)){
        
    }
    else if (g.mode_selector == MODE_SELECTOR_IREF){

    }
    else if ((g.mode_selector == MODE_SELECTOR_SPEEDCONTROLLER) && (previous_mode_selector == MODE_SELECTOR_MOMENTUM)){
        // changed from speedcontroller to momentum (gas)
    }
    else if ((g.mode_selector == MODE_SELECTOR_MOMENTUM) && (previous_mode_selector == MODE_SELECTOR_SPEEDCONTROLLER )){
        // changed from momentum (gas)to speedcontroller 
    }
    previous_mode_selector = g.mode_selector;
}

void current_controller(void){
    volatile static uint32_t count=0;
    
    int32_t iref;
    if (++count % 10 == 0){  // every 10th calling
        // DEBUG_1_SetHigh();
        count = 0;
        switch(g.mode_selector){
            case MODE_SELECTOR_ZERO_MOTOR_BLOCKED:
            case MODE_SELECTOR_ZERO_MOTOR_FLOATING:
                                        iref = 0;
                                        break; 
            case MODE_SELECTOR_SPEEDCONTROLLER:
                                        iref = g.speed.out;
                                        break;
            case MODE_SELECTOR_MOMENTUM:iref = g.current.momentum; // reading momentum (gas)
                                        break;
            case MODE_SELECTOR_IREF:    iref = g.current.ref;
                                        break;
        }
        
        // dynamic current limiter 
        // iref = g.current.ref;
        iref = (iref > g.current.limit)? g.current.limit : iref;
        iref = (iref  < -g.current.limit)? g.current.limit : iref;
        int16_t duty_cycle = PIController_Compute(&g.current.controller, iref, g.current.value);
        g.direction = (duty_cycle < 0)? ANTICLOCKWISE : CLOCKWISE;
        duty_cycle = abs(duty_cycle);
        duty_cycle = (duty_cycle > PWM_MAX_DUTY)? PWM_MAX_DUTY : duty_cycle; // limit duty cycle to 100%
        MDC = abs(duty_cycle);
        // adjust adc interrupt trigger time
        PG1TRIGA = (MDC > 8000)? MDC-100 : MDC+100;
        PG1TRIGB = PG1TRIGA;
        PG1STATbits.UPDREQ = 1; 
        // DEBUG_1_SetLow();
    }
}

// ########################################################################
//                  ADC Interrupt Service Routine
// ########################################################################
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
            g.current.value = abs((g.energized_sector==1 || g.energized_sector==2)? ((int32_t)adcVal - 2048) : g.current.value);
            g.current.value = (g.direction == ANTICLOCKWISE)? -g.current.value : g.current.value;
            handle_mode_selector_changing();
            break;

    #ifdef POWERLAB_HARDWARE
        case _I2_PowerLab:
    #else
        case _I2:
    #endif
            g.current.value  = abs((g.energized_sector==3 || g.energized_sector==4)? ((int32_t)adcVal - 2048) : g.current.value);
            g.current.value = (g.direction == ANTICLOCKWISE)? -g.current.value : g.current.value;
            current_controller(); // channel I2 is the last channel to be read, so, the best place to call the current controller is here.
            break;
        case _I3:
            g.current.value = abs((g.energized_sector==5 || g.energized_sector==6)? ((int32_t)adcVal - 2048) : g.current.value);
            g.current.value = (g.direction == ANTICLOCKWISE)? -g.current.value : g.current.value;           
            break;
        default:
            break;
    }   
    // DEBUG_0_SetLow();
}