#pragma once

#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <inttypes.h>
#include <stdbool.h>

#define FCY 100000000UL  //100Mhz
#define MYADDRESS "#4"      // define the address, in the future with dip-switches

#define HARDWARE_TEST
// #define DEBUG_SERIAL_COMMAND_HANDLING 
#define DEBUG               // only one option is allowed
// #define FLETUINO            // DEBUG or FLETUINO or nothing


#if defined(DEBUG) && defined(FLETUINO)
#undef FLETUINO
#endif  



