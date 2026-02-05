#include "main.h"
/*
Mostly used configuration defines are listet first
*/
// IMPORTANT: all defines marked with * are not implemented yet

#define VLINK_NOMINAL_VOLTAGE 50
#define VLINK_CUTOFF_VOLTAGE_LOW 10                 // * switching to error state reaching this value !
#define VLINK_CUTOFF_VOLTAGE_HIGH 60                // * switching to error state reaching this value !   
#define SPEED_AT_NOMINAL_VOLTAGE 5000               // rpm at nominal voltage
#define SPEED_MEASUREMENTS_PER_SECOND 4             // number of measurements per second and calls of speed controller per second (1...1000)
#define SPEED_THRESHOLD_FOR_DIRECTION_CHANGE 100    // rpm threshold for direction change, must be below this value to change current direction


#define USE_SPEED_RAMP_FUNCTION 1                   // 0: not used 1: used 
#define USE_CURRENT_RAMP_FUNCTION 0                 // 0: not used 1: used 
/*
ADC conversion factors
*/
#define ADC_FACTOR_VLINK 0.01622                    // factor - hardware dependend 3.3V * 20 / 4095 = 0.01622
#ifdef SMART_POWERLAB_HARDWARE
    #define ADC_FACTOR_CURRENT 0.0030525            // factor - SmartPowerLab Hardware
#else
    #define ADC_FACTOR_CURRENT 0.03021              // factor - Drive Hardware 3.3V / 4095 / 26.67mV/A = 0.03021
#endif

#define ADC_FACTOR_TEMPERATURE -0.001               // * factor - hardware dependend - not yet determined  
#define ADC_FACTOR_OFFSET 25.0                      // * factor - hardware dependend - not yet determined   
/*
Cotroller kp and ki factors
*/
#define SPEED_CONTROLLER_KP 0.44
#define SPEED_CONTROLLER_KI 0.008
#define CURRENT_CONTROLLER_KP 0.7
#define CURRENT_CONTROLLER_KI 0.02
#define CURRENT_USAGE_OF_MAX_CURRENT 0.95           // 1.0 FULL Scale usage - 0.5 HALF Scale usage

#define PWM_PERIOD 8190                             // FPG1_clk/(PGxPER + 1)= 200Mhz/(8190+1) = 24.417kHz
#define PWM_MAX_DUTY PWM_PERIOD+1                   // do not edit !

// #define ADC_RESOLUTION 4095
#define MOTOR_NUMBER_OF_POLPAIRS 1                  // *
#define MOTOR_MAX_CURRENT_AMPERE_PEAK 0.99          // *
#define MOTOR_MAX_CURRENT_AMPERE_PEAK_TIME_MS 1000  // *
#define MOTOR_MAX_SPEED_RPM 5500                    // *
#define MOTOR_DIRECTION_INVERTED 0                  // 0: normal 1: inverted
#define MOTOR_DIRECTION_CLOCKWISE_PERMITTED 1       // *
#define MOTOR_DIRECTION_ANTICLOCKWISE_PERMITTED 1   // *

#define HALL_PULSES_PER_ROTATION 6
