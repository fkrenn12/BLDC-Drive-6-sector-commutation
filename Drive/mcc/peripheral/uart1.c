#include "uart1.h"
// #include "serial-command-interpreter.h"
static volatile RingBuffer txBuffer;
static volatile RingBuffer rxBuffer;

static void UART1_RxEnableInterrupt(void)
{
    IEC0bits.U1RXIE = 1; // RX-Interrupt enable
}

static void UART1_RxDisableInterrupt(void)
{
    IEC0bits.U1RXIE = 0; // RX-Interrupt disable
}

void UART1_Initialize(void)
{
    RingBuffer_Init(&txBuffer);
    RingBuffer_Init(&rxBuffer);
    RingBuffer_SetWriteLock(&rxBuffer, UART1_RxDisableInterrupt);
    RingBuffer_SetWriteUnlock(&rxBuffer, UART1_RxEnableInterrupt);
/*    
     Set the UART1 module to the options selected in the user interface.
     Make sure to set LAT bit corresponding to TxPin as high before UART initialization
*/
    // Disable Analog Function on PortB RB3 and RB4
    ANSELB &= ~((1u << 3) | (1u << 4));
    // Rx pin
    TRISBbits.TRISB3 = 1;
    CNPUBbits.CNPUB3 = 1;   // Weak Pullup the RXPin
    // Tx Pin
    TRISBbits.TRISB4 = 0;   
    LATBbits.LATB4 = 1; // Set high the TxPin
    
    /****************************************************************************
     * Set the PPS
     ***************************************************************************/  
     __builtin_write_RPCON(0x0000); // unlock PPS
    RPINR18bits.U1RXR = 0x0023U; //RB3->UART1:U1RX;
    RPOR2bits.RP36R = 0x0001U;  //RB4->UART1:U1TX;
     __builtin_write_RPCON(0x0800); // lock PPS
    
    // URXEN ; RXBIMD ; UARTEN disabled; MOD Asynchronous 8-bit UART; UTXBRK ; BRKOVR ; UTXEN ; USIDL ; WAKE ; ABAUD ; BRGH ; 
    U1MODE = 0x0U;
    // STSEL 1 Stop bit sent, 1 checked at RX; BCLKMOD enabled; SLPEN ; FLO ; BCLKSEL FOSC/2; C0EN ; RUNOVF ; UTXINV ; URXINV ; HALFDPLX ; 
    U1MODEH = 0x800U;
    // OERIE ; RXBKIF ; RXBKIE ; ABDOVF ; OERR ; TXCIE ; TXCIF ; FERIE ; TXMTIE ; ABDOVE ; CERIE ; CERIF ; PERIE ; 
    U1STA = 0x00U;
    // URXISEL ; UTXBE ; UTXISEL ; URXBE ; STPMD ; TXWRE ; 
    U1STAH = 0x2EU;
    // BaudRate 115207.37; Frequency 100000000 Hz; BRG 868; 
    U1BRG = 0x364U;
    // BRG 0; 
    U1BRGH = 0x0U;

    U1MODEHbits.RUNOVF = 1;  // When an Overflow Error (OERR) condition is detected, the RX shifter continues to run
    // Enable RX interrupt 
    IFS0bits.U1RXIF = 0;       // clear RX-Flag 
    IEC0bits.U1RXIE = 1;       // RX-Interrupt enabled
    // Enable UART1 with receiver and transmitter
    U1MODEbits.URXEN = 1;
    U1MODEbits.UTXEN = 1;
    U1MODEbits.UARTEN = 1;   // enabling UART ON bit
}

void UART1_Deinitialize(void)
{
    U1MODE = 0x0U;
    U1MODEH = 0x0U;
    U1STA = 0x80U;
    U1STAH = 0x2EU;
    U1BRG = 0x0U;
    U1BRGH = 0x0U;
}

void UART1_WriteBlockingByte(uint8_t txData)
{
    while(U2STAHbits.UTXBF == 1);
    U2TXREG = txData;    // Write the data byte to the USART.
}

void UART1_WriteBlockingString(const char* str){
    while(*str) UART1_WriteBlockingByte(*str++);
}

void UART1_WriteNoneBlockingString(const char* str){
    RingBuffer_WriteString(&txBuffer, str);
}

bool UART1_RxBufferedAvailable(void){
    return (!RingBuffer_IsEmpty(&rxBuffer)); // RingBuffer has data
}

bool UART1_TxBufferedAvailable(void){
    return (!RingBuffer_IsEmpty(&txBuffer));
}

uint8_t UART1_RxBufferedReadByte(void){
    char c;
    RingBuffer_Read(&rxBuffer, &c);
    return (uint8_t)c;
}

uint8_t UART1_TxReadBufferedByte(void){
    char c;
    RingBuffer_Read(&txBuffer, &c);
    return (uint8_t)c;
}

void UART1_TxBufferedService(void){
    while (!RingBuffer_IsEmpty(&txBuffer)){                  
        if (U1STAHbits.UTXBF) return;
        char c;
        RingBuffer_Read(&txBuffer,&c);
        if (c) U1TXREG = c;
    }//end while
}

bool UART1_IsTxReady(void)
{
    return ((!U1STAHbits.UTXBF) && U1MODEbits.UTXEN);
}

bool UART1_IsTxDone(void)
{
    return (bool)(U1STAbits.TRMT && U1STAHbits.UTXBE);
}


void __attribute__ ((interrupt, no_auto_psv)) _U1RXInterrupt(void)
{
    if (U1STAbits.OERR) U1STAbits.OERR = 1;
    while (U1STAHbits.URXBE == 0) {
        if (!RingBuffer_IsFull(&rxBuffer))
        {
            RingBuffer_Write(&rxBuffer, (char)U1RXREG);
        }
    } 
    IFS0bits.U1RXIF = 0; 
}
