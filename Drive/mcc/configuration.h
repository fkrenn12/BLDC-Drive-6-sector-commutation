#include "main.h"
/*
Mostly used configuration defines are listet first
*/
// IMPORTANT: all defines marked with * are not implemented yet

#define VLINK_NOMINAL_VOLTAGE 50                    // for calculating max rpm speed only
#define VLINK_CUTOFF_VOLTAGE_LOW 10                 // * switching to error state reaching this low limit
#define VLINK_CUTOFF_VOLTAGE_HIGH 50                // switching to error state reaching this high limit
#define SPEED_AT_NOMINAL_VOLTAGE 5000               // rpm at nominal voltage
#define SPEED_MEASUREMENTS_PER_SECOND 4             // number of measurements per second and calls of speed controller per second (1...1000)
#define SPEED_THRESHOLD_FOR_DIRECTION_CHANGE 100    // rpm threshold for direction change, must be below this value to change current direction


#define SPEED_RAMP_USED 1                           // 0: not used 1: used 
#define SPEED_RAMP_UPSTEP 20                        // 20 rpm digits up per step
#define SPEED_RAMP_DOWNSTEP 200                     // 200 rpm digits down per step
#define SPEED_RAMP_INTERVAL_MS 20                   // calculation interval in ms

#define MOMENTUM_RAMP_USED 1                        // 0: not used 1: used
#define MOMENTUM_RAMP_UPSTEP 20                     // 20 momentum digits up per step
#define MOMENTUM_RAMP_DOWNSTEP 500                  // 500 momentum digits down per step
#define MOMENTUM_RAMP_INTERVAL_MS 20                // calculation interval in ms
/*
ADC conversion factors
*/
#ifdef SMART_POWERLAB_HARDWARE
    #define OFFSET_CURRENT_U 0
    #define OFFSET_CURRENT_V 0
    #define OFFSET_CURRENT_W 0
    #define ADC_FACTOR_VLINK 0.01622                // factor - hardware dependend 3.3V * 20 / 4095 = 0.01622 V/DIG
    #define ADC_FACTOR_VLINK_REZIPROK 62            // 1/ADC_FACTOR_VLINK =  62 DIG/V
    #define ADC_FACTOR_CURRENT 0.0030525            // factor - SmartPowerLab Hardware 3.3V / 4095 / 264mV/A = 0.0030525 A/DIG
    #define ADC_FACTOR_CURRENT_REZIPROK 327         // 1/ADC_FACTOR_CURRENT = 327.6 DIG/A
#else
    #define OFFSET_CURRENT_U 0
    #define OFFSET_CURRENT_V 0
    #define OFFSET_CURRENT_W 0
    #define ADC_FACTOR_VLINK 0.01934                // factor - hardware dependend 3.3V * 24 / 4095 = 0.01934 V/DIG
    #define ADC_FACTOR_VLINK_REZIPROK 52            // 1/ADC_FACTOR_VLINK =  52 DIG/V
    #define ADC_FACTOR_CURRENT 0.03021              // factor - Drive Hardware 3.3V / 4095 / 26.67mV/A = 0.03021 A/DIG
    #define ADC_FACTOR_CURRENT_REZIPROK 33          // 1/ADC_FACTOR_CURRENT = 33.1 DIG/A
#endif

#define ADC_FACTOR_TEMPERATURE -0.001               // * factor - hardware dependend - not yet determined  
#define ADC_FACTOR_OFFSET 25.0                      // * factor - hardware dependend - not yet determined   
/*
Cotroller kp and ki factors
*/
#define SPEED_CONTROLLER_KP 0.08
#define SPEED_CONTROLLER_KI 0.008
#ifdef SMART_POWERLAB_HARDWARE
    #define CURRENT_CONTROLLER_KP 1.30
    #define CURRENT_CONTROLLER_KI 0.023
#else
    #define CURRENT_CONTROLLER_KP 3.00
    #define CURRENT_CONTROLLER_KI 0.023
#endif
#define CURRENT_USAGE_OF_MAX_CURRENT 0.80           // 1.0 FULL Scale usage - 0.5 HALF Scale usage
#define CURRENT_MAX_VALUE_MA (uint16_t)(ADC_FACTOR_CURRENT*2048*CURRENT_USAGE_OF_MAX_CURRENT*1000)
#define CURRENT_CUTOFF_VALUE 0.98                   // 
#define CURRENT_CUTOFF_TIME_MS 10                   // *

#define PWM_PERIOD 8190                             // FPG1_clk/(PGxPER + 1)= 200Mhz/(8190+1) = 24.417kHz
#define PWM_MAX_DUTY PWM_PERIOD-1                   // do not edit !
#define PWM_PERIODE_MICROSECOND 41                  // do not edit ! 1000/24.417kHz = 41us

// #define ADC_RESOLUTION 4095
#ifdef SMART_POWERLAB_HARDWARE
    #define MOTOR_NUMBER_OF_POLPAIRS 1              // 
#else
    #define MOTOR_NUMBER_OF_POLPAIRS 4              // 
#endif

#define MOTOR_MAX_SPEED_RPM 5500                    // *
#define MOTOR_DIRECTION_INVERTED 0                  // 0: normal 1: inverted
#define MOTOR_DIRECTION_CLOCKWISE_PERMITTED 1       // *
#define MOTOR_DIRECTION_ANTICLOCKWISE_PERMITTED 1   // *


#define HALL_PULSES_PER_ROTATION (MOTOR_NUMBER_OF_POLPAIRS*6)
