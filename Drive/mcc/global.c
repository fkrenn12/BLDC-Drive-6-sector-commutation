#include "global.h"
#include <errno.h>
TGlobal g={
    .millis = 0,
    .mode_selector = MODE_SELECTOR_IREF,
    .vlink = 0,
    .direction = 0,
    .position_sector = 0,
    .energized_sector = 0,
    .current.value = 0,
    .current.ref = 0,
    .speed.value = 0,
    .speed.sectors_counted = 0,
    .speed.ramp.value = 0,
    .speed.ramp.target = 0,
    .MIN_OUTPUT_SPEED = 0,
    .MAX_OUTPUT_SPEED = 0
};


void GLOBAL_Init(void){
    g.speed.ramp.counter = 0; // 50ms ramp interval
    g.speed.ramp.interval = 100; // 50ms ramp interval
    g.speed.ramp.upstep = 20;
    g.speed.ramp.downstep = 200;

    PIController_Init(&g.current.controller,
        double_to_fixed32(CURRENT_CONTROLLER_KP), 
        double_to_fixed32(CURRENT_CONTROLLER_KI),
        double_to_fixed32(-3.99951171875),
        double_to_fixed32(3.99951171875));
    
    PIController_Init(&g.speed.controller,
        double_to_fixed32(SPEED_CONTROLLER_KP), 
        double_to_fixed32(SPEED_CONTROLLER_KI),
        double_to_fixed32(-1.0 * CURRENT_USAGE_OF_MAX_CURRENT),
        double_to_fixed32(0.99951171875 * CURRENT_USAGE_OF_MAX_CURRENT));

    g.current.limit = double_to_fixed32(0.5);
    

    INTCON1bits.NSTDIS = 0; // 0 = Interrupt nesting is enabled 
    IPC16bits.PWM1IP = 4;
}

uint64_t millis(void)
{
    uint64_t res;
    IEC0bits.T1IE = 0; // disable Interrupt
    res = g.millis;
    IEC0bits.T1IE = 1; // Enable Interrupt
    return res;
}

int __attribute__((__section__(".libc.write"))) write(int handle, void *buffer, unsigned int len)
{
    switch (handle) {
        case 1: // stdout
            UART2_WriteBlockingString(buffer);
            return (int)len;           

        case 2: // stderr
             UART2_WriteNoneBlockingString(buffer);
             return (int)len;

        default:
            errno = EBADF;
            return -1;
    }
}
