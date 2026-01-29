#include "main.h"
#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/system/pins.h"
#include "mcc_generated_files/adc/adc1.h"
#include "pwm-sector-detection-commutation-speed-measurement.h"
#include "peripheral/uart1.h"
#include "peripheral/uart2.h"
#include "peripheral/timer1.h"
#include "peripheral/pwm.h"
#include "lib/pi-controller.h"
#include "global.h"
#include "debug.h"
#include "configuration.h"
#include "gui.h"
#include "adc-current-measurement-and-control.h"
#include "lib/ramp.h"
// #include <libpic30.h>
#include "serial-command-interpreter.h"

extern TGlobal g;
// ########################################################################
//              Timer1 Interrupt Service Routine ( 250µs Callback ) 
// ########################################################################
void __attribute__ ((interrupt, no_auto_psv)) _T1Interrupt(void)
{
    // ***************************************
    // speed measurement and speed controller
    // ***************************************
    volatile static int8_t state = 0;
    volatile static uint16_t speed_control_timer = 0;
    volatile static uint16_t precounter = 0;
    volatile static uint64_t  previous_millis = 0;

    #define INTERVAL_BETWEEN_MEASUREMENTS_MS (uint16_t)(1000UL/SPEED_MEASUREMENTS_PER_SECOND)
    
        // This section ist done every 250µs
    precounter++;
    switch (precounter){
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            precounter = 0;
            g.millis++;
            break;
        default:
            precounter = 0;
            break;
    }

    
    if (previous_millis == g.millis) {
        IFS0bits.T1IF = 0;
        return;
    }
    
    previous_millis = g.millis;
    g.speed.ref = ramp(&g.speed.ramp); 
    if (++speed_control_timer == INTERVAL_BETWEEN_MEASUREMENTS_MS)
    {      
        speed_control_timer = 0;
        // speed caculation
        g.speed.value = g.speed.sectors_counted * (60 * SPEED_MEASUREMENTS_PER_SECOND / HALL_PULSES_PER_ROTATION);   // rpm 10*60/PULSES_PER_ROTATION = 100
        g.speed.sectors_counted = 0;
        switch (state){
            // delaying start of speedcontroller to wait for valid speed measurement values
            case 0: 
            case 1: state++; 
                    break;
            case 2:
                if (g.speed.controller_activated) {
                    // not a good solution yet
                    /*
                    if (g.speed.value > g.speed.ref + 200)
                    {
                        PIController_Synthetise_ki(&g.speed.controller, double_to_fixed32(0)); // double_to_fixed32(SPEED_CONTROLLER_KI),
                        PIController_Reset_integrator(&g.speed.controller);
                    }
                    else  PIController_Synthetise_ki(&g.speed.controller, double_to_fixed32(SPEED_CONTROLLER_KI));
                    */
                    PIController_Synthetise_ki(&g.speed.controller, double_to_fixed32(SPEED_CONTROLLER_KI));
                    g.speed.out  = PIController_Compute(&g.speed.controller, g.speed.ref, g.speed.value); 
                    g.current.ref = g.speed.out;
                }
                break;
        }
    } 
    IFS0bits.T1IF = 0;  
    
}

// ########################################################################
//                  Main Function
// ########################################################################
int main(void){
    SYSTEM_Initialize(); 
    TMR1_Initialize();
    UART1_Initialize();
    UART2_Initialize();
    PWM_Initialize();
    GLOBAL_Init();

    #ifdef FLETUINO 
        fletuino_init(UART2_RxBufferedAvailable, UART2_RxBufferedReadByte, UART2_WriteBlockingByte, start_page); 
    #endif
    ADC1_ChannelCallbackRegister(ADC_Callback);
    
    while (1){   
        static uint16_t eventTimer1 = 0;
        static uint16_t eventTimer2 = 0;
        static uint16_t eventTimer3 = 0;

        static int32_t irefs[10]={100,150,200,250,250,250,200,150,100,0};
        static int32_t speedrefs[10]={100,150,300,400,600,500,200,150,100,0};
        static uint8_t iref_index = 0;
        static uint64_t previous_millis = 0;
        uint64_t actual_millis = millis();
        /*
          Service calls - preferably in the main loop
          because using printf (sprintf) is not allowed in interrupts
        */
        SerialCommandRxService(); 
        SerialCommandTxService();
        UART2_RXBufferedService();
        UART2_TxBufferedService(); 
        
        #ifdef FLETUINO  
           fletuino_loop();
        #endif       
        // IO_LED_Toggle();

        if (actual_millis > previous_millis)
        {   
            previous_millis = actual_millis; 
            eventTimer1++;
            eventTimer2++;
            eventTimer3++;

            if (eventTimer1 == 100){  // every 100 milliseconds 
                eventTimer1 = 0;         
                ADC_SoftwareTriggerChannelSequencing(); // Sequencially start software triggered ADC channels
                IO_LED_Toggle();
            }
                                                                               
            if (eventTimer2 == 5000){   // every 5sec
                eventTimer2 = 0;
                static uint16_t iref = 0; 
                if (iref==1000) iref = 200;
                else iref=1000;
                //g.current.ref = ref;
                g.speed.controller_activated=1;
                drive_set_speed_rpm(800);
                iref_index++;
                iref_index %= 10; 
                /*
                if (iref_index==0)
                {
                    if (g.direction == CLOCKWISE) g.direction = ANTICLOCKWISE;
                    else g.direction = CLOCKWISE;
                }                
                g.current.ref = irefs[iref_index];
                */
                // g.speed_ref = speedrefs[iref_index] * 2;
                
            }
            if (eventTimer3 == 50){  // every 50 milliseconds  
                eventTimer3 = 0; 
                // IO_LED_Toggle();
                // if (g.speed.controller.saturated == 1) IO_LED_SetLow();
                // else IO_LED_SetHigh();
                g.vlink = ADC_Result(_VLINK);
                g.speed.max = (SPEED_AT_NOMINAL_VOLTAGE / VLINK_NOMINAL_VOLTAGE) * g.vlink * ADC_FACTOR_VLINK;
                g.speed.ramp.target = (g.speed.ramp.target > g.speed.max)? g.speed.max : g.speed.ramp.target;
                
                #ifdef FLETUINO
                    gui_update();
                #endif
            }
        }
    }
}
