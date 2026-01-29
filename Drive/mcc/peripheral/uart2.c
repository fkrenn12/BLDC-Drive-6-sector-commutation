#include "uart2.h"

static volatile RingBuffer txBuffer;
static volatile RingBuffer rxBuffer;

static void UART2_RxEnableInterrupt(void)
{
    IEC1bits.U2RXIE = 1; // RX-Interrupt enable
}

static void UART2_RxDisableInterrupt(void)
{
    IEC1bits.U2RXIE = 0; // RX-Interrupt disable
}

void UART2_Initialize(void)
{
    RingBuffer_Init(&txBuffer);
    RingBuffer_Init(&rxBuffer);
    RingBuffer_SetWriteLock(&rxBuffer, UART2_RxDisableInterrupt);
    RingBuffer_SetWriteUnlock(&rxBuffer, UART2_RxEnableInterrupt);
/*    
     Set the UART2 module to the options selected in the user interface.
     Make sure to set LAT bit corresponding to TxPin as high before UART initialization
*/
    // Disable Analog Function on PortC RC10 and RC11
    ANSELC &= ~((1u << 10) | (1u << 11));
    TRISCbits.TRISC11 = 1; // RX input
    TRISCbits.TRISC10 = 0; // TX output
    LATCbits.LATC10 = 1; // Set high the TxPin
    /****************************************************************************
     * Set the PPS
     ***************************************************************************/
     
     __builtin_write_RPCON(0x0000); // unlock PPS
        RPINR19bits.U2RXR = 0x003BU; //RC11->UART2:U2RX;
        RPOR13bits.RP58R = 0x0003U;  //RC10->UART2:U2TX;
     __builtin_write_RPCON(0x0800); // lock PPS
     
    // URXEN ; RXBIMD ; UARTEN disabled; MOD Asynchronous 8-bit UART; UTXBRK ; BRKOVR ; UTXEN ; USIDL ; WAKE ; ABAUD ; BRGH ; 
    U2MODE = 0x0U;
    // STSEL 1 Stop bit sent, 1 checked at RX; BCLKMOD enabled; SLPEN ; FLO ; BCLKSEL FOSC/2; C0EN ; RUNOVF ; UTXINV ; URXINV ; HALFDPLX ; 
    U2MODEH = 0x800U;
    // OERIE ; RXBKIF ; RXBKIE ; ABDOVF ; OERR ; TXCIE ; TXCIF ; FERIE ; TXMTIE ; ABDOVE ; CERIE ; CERIF ; PERIE ; 
    U2STA = 0x00U;
    // URXISEL ; UTXBE ; UTXISEL ; URXBE ; STPMD ; TXWRE ; 
    U2STAH = 0x2EU;
    // BaudRate 115207.37; Frequency 100000000 Hz; BRG 868; 
    U2BRG = 0x364U;
    // BRG 0; 
    U2BRGH = 0x0U;
    
    U2MODEHbits.RUNOVF = 1;  // When an Overflow Error (OERR) condition is detected, the RX shifter continues to run
    // Enable RX interrupt 
    IFS1bits.U2RXIF = 0;       // clear RX-Flag 
    IEC1bits.U2RXIE = 1;       // RX-Interrupt enabled
    // Enable UART1 with receiver and transmitter
    U2MODEbits.URXEN = 1;
    U2MODEbits.UTXEN = 1;
    U2MODEbits.UARTEN = 1;   // enabling UART ON bit
}



void UART2_Deinitialize(void)
{
    U2MODE = 0x0U;
    U2MODEH = 0x0U;
    U2STA = 0x80U;
    U2STAH = 0x2EU;
    U2BRG = 0x0U;
    U2BRGH = 0x0U;
}

void UART2_WriteBlockingByte(uint8_t txData)
{
    while(U2STAHbits.UTXBF == 1);  // wait until TX buffer not full
    U2TXREG = txData;    // Write the data byte to the USART.
}

void UART2_WriteBlockingString(const char* str){
    while(*str) UART2_WriteBlockingByte(*str++);
}

void UART2_WriteNonBlockingByte(const uint8_t txData){
    RingBuffer_Write(&txBuffer, (char)txData);
}

void UART2_WriteNoneBlockingString(const char* str){
    RingBuffer_WriteString(&txBuffer, str);
}

bool UART2_IsTxReady(void)
{
    return ((!U2STAHbits.UTXBF) && U2MODEbits.UTXEN);
}

bool UART2_IsTxDone(void)
{
    return (bool)(U2STAbits.TRMT && U2STAHbits.UTXBE);
}


void UART2_TxBufferedService(void){
    while (!RingBuffer_IsEmpty(&txBuffer)){                  
        if (U2STAHbits.UTXBF) return;
        char c;
        RingBuffer_Read(&txBuffer,&c);
        if (c) U2TXREG = c;
    }//end while
}

bool UART2_RxBufferedAvailable(void){
    return (!RingBuffer_IsEmpty(&rxBuffer)); // RingBuffer has data
}

uint8_t UART2_RxBufferedReadByte(void){
    char c;
    RingBuffer_Read(&rxBuffer, &c);
    return (uint8_t)c;
}

void UART2_RXBufferedService(void){

}

void __attribute__ ((interrupt, no_auto_psv)) _U2RXInterrupt(void)
{
    if (U2STAbits.OERR) U2STAbits.OERR = 1;
    while (U2STAHbits.URXBE == 0) {
        if (!RingBuffer_IsFull(&rxBuffer))
            RingBuffer_Write(&rxBuffer, (char)U2RXREG);
    } 
    IFS1bits.U2RXIF = 0;
}