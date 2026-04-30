#include "pwm.h"
#include <xc.h>

void SPI_Initialize(void){
    
    // SPIEN disabled; DISSDO disabled; MCLKEN FOSC/2; CKP Idle:Low, Active:High; SSEN disabled; MSTEN Host; MODE16 disabled; SMP Middle; DISSCK disabled; SPIFE Frame Sync pulse precedes; CKE Idle to Active; MODE32 disabled; SPISIDL disabled; ENHBUF enabled; DISSDI disabled; 
    SPI1CON1L = 0;
    // SPIBRGL 0; 
    SPI1BRGL = 0x0U;
    // AUDEN disabled; FRMEN disabled; AUDMOD I2S; FRMSYPW One clock wide; AUDMONO stereo; FRMCNT 0x0; MSSEN disabled; FRMPOL disabled; IGNROV disabled; SPISGNEXT not sign-extended; FRMSYNC disabled; URDTEN disabled; IGNTUR disabled; 
    SPI1CON1H = 0x0U;
    // WLENGTH 24bit; 
    SPI1CON2L = 0b10111;
    // SPIROV disabled; FRMERR disabled; 
    SPI1STATL = 0x0U;
    // SPIURDTL 0; 
    SPI1URDTL = 0x0U;
    // SPIURDTH 0; 
    SPI1URDTH = 0x0U;
    // SPI1CON1
    
    SPI1CON1Lbits.MODE16 = 1;
    SPI1CON1Lbits.MODE32 = 1;
    SPI1CON1Lbits.SMP = 0;      // Input data are sampled at the middle of data output time
    SPI1CON1Lbits.CKE = 1;      // Transmit happens on transition from Active Clock state to Idle Clock state
    SPI1CON1Lbits.CKP = 1;      // Idle state for clock is a high level; active state is a low level
    SPI1CON1Lbits.MSTEN = 1;    // Host mode 
    SPI1CON1Lbits.DISSDI = 1;   // SDIx pin is not used by the module; pin is controlled by port function
    SPI1CON1Lbits.ENHBUF = 1;   // Enhanced Buffer mode is enabled 
    SPI1CON1Lbits.SPIEN = 1;
}

void SPI1_ByteWrite(uint8_t byteData)
{
    while(1U == SPI1STATLbits.SPITBF);
    SPI1BUFL = byteData;
    SPI1BUFH = 0; // byteData;
}