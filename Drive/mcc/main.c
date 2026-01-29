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
#include <libpic30.h>
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
    // f = 1/250µs = 40000Hz
    #define TIMER1_CALLBACK_FREQUENCY (uint16_t)1000UL
    #define INTERVAL_BETWEEN_MEASUREMENTS_MS (uint16_t)(TIMER1_CALLBACK_FREQUENCY/SPEED_MEASUREMENTS_PER_SECOND)
    #define SEONDS_OF_ONE_MINUTE 60
    
    
    // This section ist done every 250µs
    precounter++;
    switch (precounter){
        case 1:
            // SerialCommandRxService();  
            break;
        case 2:
            // SerialCommandTxService();
            break;
        case 3:
            // UART2_RXBufferedService();
            break;
        case 4:
            // UART2_TxBufferedService(); 
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
        g.speed.value = g.speed.sectors_counted * (SEONDS_OF_ONE_MINUTE * SPEED_MEASUREMENTS_PER_SECOND / HALL_PULSES_PER_ROTATION);   // rpm 10*60/PULSES_PER_ROTATION = 100
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
    // PIController_Init(&pi_current, g.KP_CURRENT, g.KI_CURRENT, g.MIN_OUTPUT_CURRENT, g.MAX_OUTPUT_CURRENT);
    // PIController_Init(&pi_speed, g.KP_SPEED, g.KI_SPEED, g.MIN_OUTPUT_SPEED, g.MAX_OUTPUT_SPEED);
    // fletuino_init(UART2_IsRxReady, UART2_Read, UART2_Write, start_page); 
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
                // debug data output
                // printf("speed stpt=%li,error=%li,intgrtr=%li,strtd=%li,output=%li\n\r",(int32_t)g.speed.controller.setpoint,(int32_t)g.speed.controller.error,g.speed.controller.integrator, (int32_t)g.speed.controller.saturated,(int32_t)g.speed.controller.output);
                // printf("current stpt=%li,error=%li,intgrtr=%li,strtd=%li,output=%li\n\r",(int32_t)g.current.controller.setpoint,(int32_t)g.current.controller.error,g.current.controller.integrator, (int32_t)g.current.controller.saturated,(int32_t)g.current.controller.output);
                // printf("speed target=%li,speed value=%li,speed ref=%li\n\r",(int32_t)g.speed.ramp.target,(int32_t)g.speed.ramp.value,(int32_t)g.speed.ref);
                // printf("%li,%li,%li\n\r",(int32_t)g.speed.ramp.target,(int32_t)g.speed.ramp.value,(int32_t)g.speed.ref);
             
                // uint8_t hallpattern = (PORTC & 0xE0)>>5;
                // uint8_t sector = SECTOR[hallpattern];
                // snprintf(temp, sizeof(temp), "Position Sector %d\n\r",sector);
                //UART1_WriteString(temp);
                /*
                uint8_t energized_sector = _ANTICLOCKWISE[sector];
                hallpattern = (PORTC & 0xE0)>>5;
                sector = SECTOR[hallpattern];
                
                energized_sector = sector;
                energized_sector = (energized_sector>6)? 1 : energized_sector;
                energized_sector = (energized_sector<1)? 6 : energized_sector;
                PG1IOCONL = PWMState1[energized_sector];
                PG2IOCONL = PWMState2[energized_sector];
                PG3IOCONL = PWMState3[energized_sector];
                */
            }
        }
    }
}
