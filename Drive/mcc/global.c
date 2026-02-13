#include "global.h"
#include <errno.h>
char debugBuffer[256];
TGlobal g={
    .demo = 0,
    .millis = 0,
    .mode_selector = MODE_MOTOR_FLOATING,
    .vlink = 0,
    .direction = 0,
    .position_sector = 0,
    .energized_vector = 0,
    .current.value = 0,
    .current.ref = 0,
    .speed.value = 0,
    .speed.sectors_counted = 0,
    .speed.ramp.out = 0,
    .speed.ramp.in = 0,
    .speed.ref_ramped = 0,
    .input.speedRpm = 0,
    .input.gas = 0,
    .input.direction = CLOCKWISE,
    .MIN_OUTPUT_SPEED = 0,
    .MAX_OUTPUT_SPEED = 0
};

void GLOBAL_Init(void){   
    // read address from switch
    uint8_t address = (PORTC & 0x3C00)>>10; // RC10 to RC13
    sprintf((char*)g.myaddress,"#%d",address);
    #ifdef MYADDRESS
        strcpy((char*)g.myaddress,MYADDRESS);
    #endif
    INTCON1bits.NSTDIS = 0; // 0 = Interrupt nesting is enabled 
    IPC16bits.PWM1IP = 4; // PWM has highest priority
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
