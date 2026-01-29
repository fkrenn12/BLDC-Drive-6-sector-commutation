 /**
 * PINS Generated Driver Source File 
 * 
 * @file      pins.c
 *            
 * @ingroup   pinsdriver
 *            
 * @brief     This is the generated driver source file for PINS driver.
 *
 * @skipline @version   Firmware Driver Version 1.0.2
 *
 * @skipline @version   PLIB Version 1.4.1
 *
 * @skipline  Device : dsPIC33CK64MC105
*/

/*
© [2026] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

// Section: Includes
#include <xc.h>
#include <stddef.h>
#include "../pins.h"

// Section: File specific functions
static void (*QEI3_InterruptHandler)(void) = NULL;
static void (*QEI2_InterruptHandler)(void) = NULL;
static void (*QEI1_InterruptHandler)(void) = NULL;

// Section: Driver Interface Function Definitions
void PINS_Initialize(void)
{
    /****************************************************************************
     * Setting the Output Latch SFR(s)
     ***************************************************************************/
    LATA = 0x0000U;
    LATB = 0x0000U;
    LATC = 0x0000U;
    LATD = 0x0000U;

    /****************************************************************************
     * Setting the GPIO Direction SFR(s)
     ***************************************************************************/
    TRISA = 0x001FU;
    TRISB = 0xFFEEU;
    TRISC = 0x3EFFU;
    TRISD = 0x0102U;


    /****************************************************************************
     * Setting the Weak Pull Up and Weak Pull Down SFR(s)
     ***************************************************************************/
    CNPUA = 0x0000U;
    CNPUB = 0x0000U;
    CNPUC = 0x00F0U;
    CNPUD = 0x0000U;
    CNPDA = 0x0000U;
    CNPDB = 0x0000U;
    CNPDC = 0x0000U;
    CNPDD = 0x0000U;


    /****************************************************************************
     * Setting the Open Drain SFR(s)
     ***************************************************************************/
    ODCA = 0x0000U;
    ODCB = 0x0000U;
    ODCC = 0x0000U;
    ODCD = 0x0000U;


    /****************************************************************************
     * Setting the Analog/Digital Configuration SFR(s)
     ***************************************************************************/
    ANSELA = 0x001FU;
    ANSELB = 0x0386U;
    ANSELC = 0x000FU;
    ANSELD = 0x0000U;
    /*******************************************************************************
    * Interrupt On Change: any
    *******************************************************************************/
    CNEN0Cbits.CNEN0C5 = 1; //Pin : RC5U; 
    CNEN1Cbits.CNEN1C5 = 1; //Pin : RC5U; 
    /*******************************************************************************
    * Interrupt On Change: any
    *******************************************************************************/
    CNEN0Cbits.CNEN0C6 = 1; //Pin : RC6U; 
    CNEN1Cbits.CNEN1C6 = 1; //Pin : RC6U; 
    /*******************************************************************************
    * Interrupt On Change: any
    *******************************************************************************/
    CNEN0Cbits.CNEN0C7 = 1; //Pin : RC7U; 
    CNEN1Cbits.CNEN1C7 = 1; //Pin : RC7U; 

    /****************************************************************************
     * Interrupt On Change: flag
     ***************************************************************************/
    CNFCbits.CNFC5 = 0;    //Pin : QEI3
    CNFCbits.CNFC6 = 0;    //Pin : QEI2
    CNFCbits.CNFC7 = 0;    //Pin : QEI1

    /****************************************************************************
     * Interrupt On Change: config
     ***************************************************************************/
    CNCONCbits.CNSTYLE = 1; //Config for PORTC
    CNCONCbits.ON = 1; //Config for PORTC

    /* Initialize IOC Interrupt Handler*/
    QEI3_SetInterruptHandler(&QEI3_CallBack);
    QEI2_SetInterruptHandler(&QEI2_CallBack);
    QEI1_SetInterruptHandler(&QEI1_CallBack);

    /****************************************************************************
     * Interrupt On Change: Interrupt Enable
     ***************************************************************************/
    IFS1bits.CNCIF = 0; //Clear CNCI interrupt flag
    IEC1bits.CNCIE = 1; //Enable CNCI interrupt
}

void __attribute__ ((weak)) QEI3_CallBack(void)
{

}

void __attribute__ ((weak)) QEI2_CallBack(void)
{

}

void __attribute__ ((weak)) QEI1_CallBack(void)
{

}

void QEI3_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC1bits.CNCIE = 0; //Disable CNCI interrupt
    QEI3_InterruptHandler = InterruptHandler; 
    IEC1bits.CNCIE = 1; //Enable CNCI interrupt
}

void QEI2_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC1bits.CNCIE = 0; //Disable CNCI interrupt
    QEI2_InterruptHandler = InterruptHandler; 
    IEC1bits.CNCIE = 1; //Enable CNCI interrupt
}

void QEI1_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC1bits.CNCIE = 0; //Disable CNCI interrupt
    QEI1_InterruptHandler = InterruptHandler; 
    IEC1bits.CNCIE = 1; //Enable CNCI interrupt
}

/* Interrupt service function for the CNCI interrupt. */
/* cppcheck-suppress misra-c2012-8.4
*
* (Rule 8.4) REQUIRED: A compatible declaration shall be visible when an object or 
* function with external linkage is defined
*
* Reasoning: Interrupt declaration are provided by compiler and are available
* outside the driver folder
*/
void __attribute__ (( interrupt, no_auto_psv )) _CNCInterrupt (void)
{
    if(CNFCbits.CNFC5 == 1)
    {
        if(QEI3_InterruptHandler != NULL) 
        { 
            QEI3_InterruptHandler(); 
        }
        
        CNFCbits.CNFC5 = 0;  //Clear flag for Pin - QEI3
    }
    
    if(CNFCbits.CNFC6 == 1)
    {
        if(QEI2_InterruptHandler != NULL) 
        { 
            QEI2_InterruptHandler(); 
        }
        
        CNFCbits.CNFC6 = 0;  //Clear flag for Pin - QEI2
    }
    
    if(CNFCbits.CNFC7 == 1)
    {
        if(QEI1_InterruptHandler != NULL) 
        { 
            QEI1_InterruptHandler(); 
        }
        
        CNFCbits.CNFC7 = 0;  //Clear flag for Pin - QEI1
    }
    
    // Clear the flag
    IFS1bits.CNCIF = 0;
}

