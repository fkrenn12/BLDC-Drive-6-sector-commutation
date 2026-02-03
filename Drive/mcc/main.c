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
#include "configuration.h"
#include "adc-current-measurement-and-control.h"
#include "lib/ramp.h"
#include "serial-command-interpreter.h"
#ifdef FLETUINO_PI_CONTROLLER_SETTINGS
    #include "gui-fletuino-pi-controller-settings.h"
#endif
#ifdef FLETUINO_APPLICATION_DEMO
    #include "gui-fletuino-application-demo.h"
#endif

extern TGlobal g;


void handle_mode_selector(void){
    static uint16_t previous_mode_selector = 0; //g.mode_selector;
    if (g.mode_selector == previous_mode_selector) return;
    // changed
    
    if ((g.mode_selector == MODE_SELECTOR_ZERO_MOTOR_BLOCKED) || (g.mode_selector == MODE_SELECTOR_ZERO_MOTOR_FLOATING)){
        PIController_ResetIntegrator(&g.speed.controller);
        PIController_ResetIntegrator(&g.current.controller);
        g.speed.ramp.in = 0;
        ramp_reset(&g.speed.ramp);
    }
    else if (g.mode_selector == MODE_SELECTOR_IREF){

    }
    else if ((g.mode_selector == MODE_SELECTOR_SPEEDCONTROLLER) && (previous_mode_selector == MODE_SELECTOR_MOMENTUM)){
        // changed from momentum (gas)to speedcontroller 
        g.speed.controller.integrator = 0;
    }
    else if ((g.mode_selector == MODE_SELECTOR_MOMENTUM) && (previous_mode_selector == MODE_SELECTOR_SPEEDCONTROLLER )){
        // changed from speedcontroller to momentum (gas) 
        
    }
    previous_mode_selector = g.mode_selector;
}

void handle_direction_request(void){
    static uint8_t previous_direction_request = 0; 
    static uint8_t state = 0;
    if (g.direction_request != previous_direction_request) {
        previous_direction_request = g.direction_request;  
        state = 0;
    }
    IO_LED_SetHigh();
    switch (state){
        case 0: if (g.mode_selector == MODE_SELECTOR_MOMENTUM) state++;
                break;
        case 1: if (abs(g.speed.value) > SPEED_THRESHOLD_FOR_DIRECTION_CHANGE) break;
                // to take new direction the rotating speed must be below threshold
                IO_LED_SetLow();
                g.direction = g.direction_request;
                state++;
                break;
        default: break;
    }   
}

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
    volatile static uint16_t timer_50ms = 0;
    volatile static uint16_t timer_100ms = 0;
    volatile static uint16_t postdivider = 0;
    volatile static uint64_t  previous_millis = 0;

    #define INTERVAL_BETWEEN_MEASUREMENTS_MS (uint16_t)(1000UL/SPEED_MEASUREMENTS_PER_SECOND)
    
    // This section ist done every 250µs
    postdivider++;
    switch (postdivider){
        case 1:
            handle_mode_selector();
            break;
        case 2:
            handle_direction_request();
            break;
        case 3:
            break;
        case 4:
            postdivider = 0;
            g.millis++;
            break;
        default:
            postdivider = 0;
            break;
    }

    
    if (previous_millis == g.millis) {
        IFS0bits.T1IF = 0;
        return;
    }
    
    // here we are every ms
    previous_millis = g.millis;

    g.speed.ref = ramp_calculate(&g.speed.ramp); 

    if (++speed_control_timer == INTERVAL_BETWEEN_MEASUREMENTS_MS)
    {      
        speed_control_timer = 0;
        // speed caculation
        g.speed.value = (int16_t)((int32_t)g.speed.sectors_counted * (60 * SPEED_MEASUREMENTS_PER_SECOND / HALL_PULSES_PER_ROTATION));   // rpm 10*60/PULSES_PER_ROTATION = 100
        g.speed.sectors_counted = 0;
        g.speed.value = (g.direction_of_rotation == CLOCKWISE)? g.speed.value: -g.speed.value; 
        switch (state){
            // delaying start of speedcontroller to wait for valid speed measurement values
            case 0: state++; 
                    break;
            case 1:
                if (g.mode_selector == MODE_SELECTOR_SPEEDCONTROLLER) {
                    // not a good solution yet
                    /*
                    if (g.speed.value > g.speed.ref + 200)
                    {
                        PIController_Synthetise_ki(&g.speed.controller, double_to_fixed32(0)); // double_to_fixed32(SPEED_CONTROLLER_KI),
                        PIController_ResetIntegrator(&g.speed.controller);
                    }
                    else  PIController_Synthetise_ki(&g.speed.controller, double_to_fixed32(SPEED_CONTROLLER_KI));
                    */
                    PIController_Synthetise_ki(&g.speed.controller, double_to_fixed32(SPEED_CONTROLLER_KI));
                    if (!g.speed.overruled_off){
                        g.speed.out  = (int16_t)PIController_Compute(&g.speed.controller, g.speed.ref, g.speed.value); 
                    }
                }
                break;
        }
    } 
    // every 50ms
    if( ++timer_50ms == 50){
        timer_50ms = 0;
        g.vlink = ADC_Result(_VLINK);
        g.speed.max = (int16_t)((SPEED_AT_NOMINAL_VOLTAGE / VLINK_NOMINAL_VOLTAGE) * g.vlink * ADC_FACTOR_VLINK);
        g.speed.ramp.in = (g.speed.ramp.in > g.speed.max)? g.speed.max : g.speed.ramp.in;
        g.current.momentum = ADC_Result(_MOMENTUM)>1; // :2 =  0...2047
        g.current.momentum = (g.demo)? g.current.ref: g.current.momentum;
    }

    // every 100ms
    if( ++timer_100ms == 100){
        timer_100ms = 0;
        ADC_SoftwareTriggerChannelSequencing(); // Sequencially start software triggered ADC channels
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
    Drive_Init();

    #ifdef FLETUINO 
        fletuino_init(UART2_RxBufferedAvailable, UART2_RxBufferedReadByte, UART2_WriteBlockingByte, start_page); 
    #endif
    ADC1_ChannelCallbackRegister(ADC_Callback);
    
    while (1){   
        static uint16_t eventTimer1 = 0;
        static uint16_t eventTimer2 = 0;
        static uint16_t eventTimer3 = 0;
        static uint64_t previous_millis = 0;
        uint64_t actual_millis = millis();
        /*
          Service calls - preferably in the main loop
          because using printf (sprintf) is not a good advise in interrupts
        */
        SerialCommandRxService(); 
        SerialCommandTxService();
        UART2_RXBufferedService();
        UART2_TxBufferedService(); 
        
        #ifdef FLETUINO  
           fletuino_loop();
        #endif       

        if (actual_millis > previous_millis)
        {   
            previous_millis = actual_millis; 
            eventTimer1++;
            eventTimer2++;
            eventTimer3++;

            if (eventTimer1 == 100){  // every 100 milliseconds 
                eventTimer1 = 0;         
                // IO_LED_Toggle();
            }
                                                                               
            if (eventTimer2 == 2000){ 
                eventTimer2 = 0;
                // g.direction_of_rotation = ANTICLOCKWISE; //CLOCKWISE; //  ANTICLOCKWISE; 
                // MDC = 5000;
                // PG1STATbits.UPDREQ = 1; 
                //sprintf(buffer,"index %d - energized %d - actual %d\n\r", sector_index, sector, get_actual_sector());
                //UART2_WriteNoneBlockingString(buffer);
                // PWM_override(sector_index);
                // PWM_override(7);
                // Drive_setSpeedRpm(200);              
                 
            }
            if (eventTimer3 == 50){  // every 50 milliseconds  
                eventTimer3 = 0;                
                #ifdef FLETUINO
                    gui_update();
                #endif
            }
        }
    }
}
