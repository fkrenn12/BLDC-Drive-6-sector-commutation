#pragma once
#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <inttypes.h>
#include "main.h"
#include "mcc_generated_files/system/system.h"
#include "lib/fixed_point.h"
#include "configuration.h"
#include "lib/pi-controller.h"
#include "lib/ramp.h"
#include "lib/ringbuffer.h"
#include "peripheral/uart1.h"
#include "peripheral/uart2.h"
#include <libpic30.h> 

#define CLOCKWISE 1
#define ANTICLOCKWISE 0
#define MODE_MOTOR_BLOCKED 0
#define MODE_MOTOR_FLOATING 1
#define MODE_SPEEDCONTROLLER 2
#define MODE_SPEEDCONTROLLER_ZERO_CURRENT 3
#define MODE_MOMENTUM 4
#define MODE_MOMENTUM_ZERO_CURRENT 5


// console output colors
#define COL_RED "\033[31m"
#define COL_GREEN "\033[32m"
#define COL_YELLOW "\033[33m"
#define COL_VIOLETT "\033[35m"
#define COL_WHITE "\033[0m"

typedef struct _input{
    volatile int16_t gas;      // gas pedal
    volatile int16_t speedRpm; // speed
    volatile uint8_t f_r;       // forwars/backward pin
    volatile uint8_t a_m;       // automatic/momentum pin
}TInput;

typedef struct _speed{
    volatile uint16_t sectors_counted;
    volatile int16_t max;
    volatile int16_t ref_ramped;
    volatile int16_t value;
    volatile int16_t out;
    TRamp ramp;
    PIController controller;  // not yet used
}TSpeed;

typedef struct _current{
    volatile int16_t ref;
    volatile int32_t value;
    volatile int32_t limit;
    volatile int16_t momentum;
    PIController controller;
}TCurrent;

typedef struct _global{
    volatile uint8_t demo;
    volatile uint8_t myaddress[5];
    volatile uint64_t millis;    
    volatile uint8_t direction;
    volatile uint8_t direction_of_rotation;
    volatile uint8_t mode_selector;
    volatile uint8_t position_sector;
    volatile uint8_t energized_vector;
    volatile int32_t vlink;
    volatile int16_t temperature; // deci-degC
    volatile uint8_t state;  // for debugging only
    TSpeed speed;
    TCurrent current; 
    TInput input;
    fixed32_point_t MIN_OUTPUT_SPEED;
    fixed32_point_t MAX_OUTPUT_SPEED;
}TGlobal;

extern TGlobal g;
void GLOBAL_Init(void);
uint64_t millis(void);