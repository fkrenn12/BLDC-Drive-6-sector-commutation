#pragma once

#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <inttypes.h>
#include <stdbool.h>

#define FCY 100000000UL  //100Mhz
//#define MYADDRESS "#4"              // define the address hardcoded - uncomment if using the hardware with SW400

#define SMART_POWERLAB_HARDWARE     // comment if using the real hardware drive

#define CURRENT_CONTROL 1           // 0 = manual control, 1 = automatic control
#define COMMUTATE 1                 // 0 = no commutation, 1 = commutation
#define STATEMACHINE 1              // 0 = no state machine, 1 = state machine


// #define DEBUG                       // only one option (DEBUG or any FLETUINO) is allowed
// #define DEBUG_SERIAL_COMMAND_HANDLING 

// #define FLETUINO_APPLICATION_DEMO
#define FLETUINO_PI_CONTROLLER_SETTINGS
// #define FLETUINO_MANUAL_CONTROL


// NO NEED TO CHANGE ANYTHING BELOW THIS LINE
#if defined(FLETUINO_APPLICATION_DEMO)
    #define FLETUINO               
#endif

#if defined(FLETUINO_PI_CONTROLLER_SETTINGS)
    #define FLETUINO  
    #undef STATEMACHINE
    #define STATEMACHINE 0
#endif

#if defined(FLETUINO_MANUAL_CONTROL)
    #define FLETUINO  
    #undef STATEMACHINE
    #define STATEMACHINE 0
    #undef CURRENT_CONTROL
    #define CURRENT_CONTROL 0
    #undef COMMUTATE
    #define COMMUTATE 0
#endif

#if defined(DEBUG) && defined(FLETUINO)
#undef FLETUINO
#endif  



