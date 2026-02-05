#pragma once

#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <inttypes.h>
#include <stdbool.h>

#define FCY 100000000UL  //100Mhz
#define MYADDRESS "#4"      // define the address, in the future with dip-switches


#define SMART_POWERLAB_HARDWARE   // comment if using the real hardware drive

// #define DEBUG                // only one option (DEBUG or FLETUINO) is allowed
// #define DEBUG_SERIAL_COMMAND_HANDLING 
#define FLETUINO                // DEBUG or FLETUINO or nothing
// #define FLETUINO_APPLICATION_DEMO
#define FLETUINO_PI_CONTROLLER_SETTINGS


#if defined(DEBUG) && defined(FLETUINO)
#undef FLETUINO
#endif  



