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

void statemachine(void){
    enum {INIT, START, RUN_MOMENTUM, RUN_SPEEDCONTROLLER ,CHANGE_DIRECTION };
    static uint8_t  state = INIT;
    g.state = state;  // zum debuggen 
    switch (state){
        case INIT:      state = START; 
                        break;

        case START:     if (abs(g.speed.value) < 100)
                            g.mode_selector = MODE_MOTOR_BLOCKED;

                        if (g.input.f_r != g.direction){ 
                            state = CHANGE_DIRECTION;
                            g.mode_selector = MODE_MOMENTUM_ZERO_CURRENT;
                            break;
                        }
                        // wait until momentum required
                        if (g.current.momentum != 0){
                            g.mode_selector = MODE_MOMENTUM;
                            state = RUN_MOMENTUM; 
                            break;
                        }
                        // check speedcontrol
                        if ((g.input.a_m) && (g.input.speedRpm)){
                            g.mode_selector = MODE_SPEEDCONTROLLER;
                            g.speed.ramp.in = 0;
                            ramp_reset(&g.speed.ramp);
                            state = RUN_SPEEDCONTROLLER; 
                            break;
                        }
                        break;
        case RUN_MOMENTUM: 
                        if (g.current.ref == 0 && abs(g.speed.value) < 200){ 
                            // halted
                            g.mode_selector = MODE_MOTOR_BLOCKED; 
                            state = START;
                            break;
                        }
                        if (g.input.f_r != g.direction){ 
                            state = CHANGE_DIRECTION;
                            g.mode_selector = MODE_MOMENTUM_ZERO_CURRENT;
                            break;
                        }
                        break;
        case RUN_SPEEDCONTROLLER:    
                        g.speed.ramp.in = g.input.speedRpm;
                        if ((g.input.speedRpm == 0) || (!g.input.a_m)){
                            g.speed.ramp.in = 0;  // slow down
                            if (abs(g.speed.value) <100){
                                state = START;
                            }
                            break;
                        }
                        if (g.current.momentum != 0){
                            g.mode_selector = MODE_MOMENTUM;
                            state = RUN_MOMENTUM; 
                            break;
                        }
                        break;
        case CHANGE_DIRECTION:
                        // wait until speed goes below threshold
                        if (abs(g.speed.value) < 100){
                            g.direction = g.input.f_r;
                            state = START;
                        }
                        break;
        default:        state = INIT;
                        break;

    }
}

// select g.current.ref depending on mode_selector
void iref_selector(void){
    switch(g.mode_selector){
        case MODE_MOTOR_BLOCKED:
        case MODE_MOTOR_FLOATING:
        case MODE_SPEEDCONTROLLER_ZERO_CURRENT:
        case MODE_MOMENTUM_ZERO_CURRENT:
                                    g.current.ramp.in = 0;
                                    g.current.ref = g.current.ref_ramped;
                                    break; 
        case MODE_SPEEDCONTROLLER:  g.current.ref = g.speed.out;
                                    break;
        case MODE_MOMENTUM:         g.current.ramp.in = g.current.momentum;
                                    g.current.ref = g.current.ref_ramped; 
                                    g.current.ref = (g.direction == CLOCKWISE)? g.current.ref : -g.current.ref;
                                    break;
    }
}

// KI generated value clamped to range - positive values only
static inline uint16_t map_range_clamped(uint16_t in,
                                         uint16_t in_min, uint16_t in_max,
                                         uint16_t out_min, uint16_t out_max)
{
    if (in <= in_min) return out_min;
    if (in >= in_max) return out_max;
    uint32_t num = (uint32_t)(in - in_min) * (out_max - out_min);
    uint32_t den = (uint32_t)(in_max - in_min);
    return (uint16_t)(out_min + (num + (den >> 1)) / den);
}

// ########################################################################
//              Timer1 Interrupt Service Routine ( 250µs Callback ) 
// ########################################################################
void __attribute__ ((interrupt, no_auto_psv)) _T1Interrupt(void)
{
    // ***************************************
    // speed measurement and speed controller
    // ***************************************
    volatile static uint16_t speed_control_timer = 0;
    volatile static uint16_t timer_50ms = 0;
    volatile static uint16_t timer_100ms = 0;
    volatile static uint16_t sequencer = 0;
 
    #define INTERVAL_BETWEEN_MEASUREMENTS_MS (uint16_t)(1000UL/SPEED_MEASUREMENTS_PER_SECOND)
    IFS0bits.T1IF = 0;
    // This section ist done every 250µs
    switch (++sequencer){
        case 1:
            #ifndef FLETUINO_PI_CONTROLLER_SETTINGS
                statemachine();
            #endif
            return;
        case 2:
            iref_selector();
            return;
        case 3:
            // ramp exccution
            g.speed.ref_ramped = (USE_SPEED_RAMP_FUNCTION)?ramp_calculate(&g.speed.ramp):g.speed.ramp.in;
            g.current.ref_ramped = (USE_CURRENT_RAMP_FUNCTION)?ramp_calculate(&g.current.ramp):g.current.ramp.in;
            return;
        default:
            sequencer = 0;
            g.millis++;
            break;
    }

    if (++speed_control_timer == INTERVAL_BETWEEN_MEASUREMENTS_MS)
    {      
        speed_control_timer = 0;
        // speed caculation
        g.speed.value = (int16_t)((int32_t)g.speed.sectors_counted * (60 * SPEED_MEASUREMENTS_PER_SECOND / HALL_PULSES_PER_ROTATION));   // rpm 10*60/PULSES_PER_ROTATION = 100
        g.speed.sectors_counted = 0;
        g.speed.value = (g.direction_of_rotation == CLOCKWISE)? g.speed.value: -g.speed.value; 
        g.speed.out = (g.mode_selector == MODE_SPEEDCONTROLLER)?(int16_t)PIController_Compute(&g.speed.controller, g.speed.ref_ramped, g.speed.value):g.speed.out; 
    } 
    // every 50ms reading inputs
    if( ++timer_50ms == 50){
        timer_50ms = 0;
        g.vlink = ADC_Result(_VLINK);
        g.speed.max = (int16_t)((SPEED_AT_NOMINAL_VOLTAGE / VLINK_NOMINAL_VOLTAGE) * g.vlink * ADC_FACTOR_VLINK);
        g.speed.ramp.in = (g.speed.ramp.in > g.speed.max)? g.speed.max : g.speed.ramp.in;
        g.input.gas = (g.demo)? g.input.gas: ADC_Result(_MOMENTUM);
        g.input.f_r = (g.demo)? g.input.f_r: (uint8_t)(PORTB & 0x01); //RB0
        g.input.a_m = (g.demo)? g.input.a_m: (uint8_t)(PORTD & 0x400); //RD10
        #ifndef FLETUINO_PI_CONTROLLER_SETTINGS
            g.current.momentum = map_range_clamped(g.input.gas, 150, 4095, 0, 2047);
        #endif
    }

    // every 100ms
    if( ++timer_100ms == 100){
        timer_100ms = 0;
        ADC_SoftwareTriggerChannelSequencing(); // Sequencially start software triggered ADC channels
    }    
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
