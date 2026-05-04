
#include "spi.h"

void SPI_DAC_MCP48CMB24_Initialize(void){
    /****************************************************************************
     * Set the PPS
     ***************************************************************************/
     __builtin_write_RPCON(0x0000); // unlock PPS
        RPOR4bits.RP40R = 0x0005U;  //RB8->SPI1:SDO1;
        // RPINR20bits.SCK1R = 0x0027U;  //RB7->SPI1:SCK1IN;
        RPOR3bits.RP39R = 0x0006U;  //RB7->SPI1:SCK1OUT;
     __builtin_write_RPCON(0x0800); // lock PPS

    
    // SPIEN disabled; DISSDO disabled; MCLKEN FOSC/2; CKP Idle:Low, Active:High; SSEN disabled; MSTEN Host; MODE16 disabled; SMP Middle; DISSCK disabled; SPIFE Frame Sync pulse precedes; CKE Idle to Active; MODE32 disabled; SPISIDL disabled; ENHBUF enabled; DISSDI disabled; 
    SPI1CON1L = 0;
    // SPIBRGL 0; 
    SPI1BRGL = 0x0U;  // 50Mhz Clock
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
    
    SPI1CON1Lbits.SMP = 0;      // Input data are sampled at the middle of data output time
    SPI1CON1Lbits.CKE = 0;      // Transmit happens on transition from Active Clock state to Idle Clock state
    SPI1CON1Lbits.CKP = 1;      // Idle state for clock is a high level; active state is a low level
    SPI1CON1Lbits.MSTEN = 1;    // Host mode 
    SPI1CON1Lbits.DISSDI = 1;   // SDIx pin is not used by the module; pin is controlled by port function
    SPI1CON1Lbits.ENHBUF = 1;   // Enhanced Buffer mode is enabled 
    SPI1CON1Lbits.SPIEN = 1;
}

void SPI_IOEXPANDER_MCP23S18_Initialize(void){
    SPI_DAC_MCP48CMB24_Initialize();
}


void SPI1_ByteWrite(uint8_t byteData)
{
    while(SPI1STATLbits.SPITBF == 1); // wait until available buffer
    SPI1CON2L = 0b0111; // // bitsize 8bit
    SPI1BUFL = byteData;
    SPI1BUFH = 0;
}

void MCP23S18_WriteRegister(uint8_t reg_addr, uint8_t byteData)
{
    //  _________________________________________________________________________________
    // |   Cmd WR  |     Register Addr       ||          Data           |                |
    // |___________ BUF HIGH_________________||________________BUF LOW___________________|
    // | 0100 0000 | A7 A6 A5 A4 A3 A2 A1 A0 || b7 b6 b5 b4 b3 b2 b1 b0 | 0 0 0 0 0 0 0 0|
    // |MSB________|______________________LSB||MSB______________________|_____________LSB|
    while(SPI1STATLbits.SPITBF == 1); // wait until available buffer
    SPI1CON2L = 0b10111;        // bitsize 24bit
    SPI1BUFL = byteData << 8;                 
    SPI1BUFH = (0x4000 | reg_addr); 
}

void MCP23S18_ReadRegister(uint8_t reg_addr)
{
    // not working -- not tested 
    //  __________________________________________________________________________
    // |   Cmd WR  |     Register Addr       ||         Data    |                |
    // |___________ BUF HIGH_________________||________BUF LOW___________________|
    // | 0100 0000 | A7 A6 A5 A4 A3 A2 A1 A0 || 0 0 0 0 0 0 0 0 | 0 0 0 0 0 0 0 0|
    // |MSB________|______________________LSB||MSB______________|_____________LSB|

    while(SPI1STATLbits.SPITBF == 1); // wait until available buffer
    SPI1CON2L = 0b10111;        // bitsize 24bit
    SPI1BUFL = 0x0000;                 
    SPI1BUFH = (0x4100 | reg_addr); 

    while(SPI1STATLbits.SPIRBF == 0);  // Warten bis RX Buffer Daten hat
}