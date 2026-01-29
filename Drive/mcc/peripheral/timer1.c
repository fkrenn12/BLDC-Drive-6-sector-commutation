#include "timer1.h"
#include <xc.h>

void TMR1_Initialize (void)
{
    //TCS FOSC/2; TSYNC disabled; TCKPS 1:1; TGATE disabled; TECS FOSC/2; PRWIP Write complete; TMWIP Write complete; TMWDIS disabled; TSIDL disabled; TON disabled; 
    T1CON = 0x0U;
    //TMR 0x0; 
    TMR1 = 0x0U;
    //Period 0.25 ms; Frequency 100,000,000 Hz; PR 24999; 
    PR1 = 0x61A7U;
    //Clear interrupt flag
    IFS0bits.T1IF = 0;
    //Enable the interrupt
    IEC0bits.T1IE = 1;
    // Start the Timer 
    T1CONbits.TON = 1;
}
