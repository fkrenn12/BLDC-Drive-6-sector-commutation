/**
 * PINS Generated Driver Header File 
 * 
 * @file      pins.h
 *            
 * @defgroup  pinsdriver Pins Driver
 *            
 * @brief     The Pin Driver directs the operation and function of 
 *            the selected device pins using dsPIC MCUs.
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

#ifndef PINS_H
#define PINS_H
// Section: Includes
#include <xc.h>

// Section: Device Pin Macros

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RA1 GPIO Pin which has a custom name of LED2 to High
 * @pre      The RA1 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define LED2_SetHigh()          (_LATA1 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RA1 GPIO Pin which has a custom name of LED2 to Low
 * @pre      The RA1 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED2_SetLow()           (_LATA1 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RA1 GPIO Pin which has a custom name of LED2
 * @pre      The RA1 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED2_Toggle()           (_LATA1 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RA1 GPIO Pin which has a custom name of LED2
 * @param    none
 * @return   none  
 */
#define LED2_GetValue()         _RA1

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RA1 GPIO Pin which has a custom name of LED2 as Input
 * @param    none
 * @return   none  
 */
#define LED2_SetDigitalInput()  (_TRISA1 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RA1 GPIO Pin which has a custom name of LED2 as Output
 * @param    none
 * @return   none  
 */
#define LED2_SetDigitalOutput() (_TRISA1 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB0 GPIO Pin which has a custom name of DIRECTION to High
 * @pre      The RB0 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define DIRECTION_SetHigh()          (_LATB0 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB0 GPIO Pin which has a custom name of DIRECTION to Low
 * @pre      The RB0 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define DIRECTION_SetLow()           (_LATB0 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RB0 GPIO Pin which has a custom name of DIRECTION
 * @pre      The RB0 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define DIRECTION_Toggle()           (_LATB0 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RB0 GPIO Pin which has a custom name of DIRECTION
 * @param    none
 * @return   none  
 */
#define DIRECTION_GetValue()         _RB0

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB0 GPIO Pin which has a custom name of DIRECTION as Input
 * @param    none
 * @return   none  
 */
#define DIRECTION_SetDigitalInput()  (_TRISB0 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB0 GPIO Pin which has a custom name of DIRECTION as Output
 * @param    none
 * @return   none  
 */
#define DIRECTION_SetDigitalOutput() (_TRISB0 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB3 GPIO Pin which has a custom name of RxD to High
 * @pre      The RB3 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define RxD_SetHigh()          (_LATB3 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB3 GPIO Pin which has a custom name of RxD to Low
 * @pre      The RB3 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define RxD_SetLow()           (_LATB3 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RB3 GPIO Pin which has a custom name of RxD
 * @pre      The RB3 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define RxD_Toggle()           (_LATB3 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RB3 GPIO Pin which has a custom name of RxD
 * @param    none
 * @return   none  
 */
#define RxD_GetValue()         _RB3

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB3 GPIO Pin which has a custom name of RxD as Input
 * @param    none
 * @return   none  
 */
#define RxD_SetDigitalInput()  (_TRISB3 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB3 GPIO Pin which has a custom name of RxD as Output
 * @param    none
 * @return   none  
 */
#define RxD_SetDigitalOutput() (_TRISB3 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB4 GPIO Pin which has a custom name of TxD to High
 * @pre      The RB4 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define TxD_SetHigh()          (_LATB4 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB4 GPIO Pin which has a custom name of TxD to Low
 * @pre      The RB4 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define TxD_SetLow()           (_LATB4 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RB4 GPIO Pin which has a custom name of TxD
 * @pre      The RB4 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define TxD_Toggle()           (_LATB4 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RB4 GPIO Pin which has a custom name of TxD
 * @param    none
 * @return   none  
 */
#define TxD_GetValue()         _RB4

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB4 GPIO Pin which has a custom name of TxD as Input
 * @param    none
 * @return   none  
 */
#define TxD_SetDigitalInput()  (_TRISB4 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB4 GPIO Pin which has a custom name of TxD as Output
 * @param    none
 * @return   none  
 */
#define TxD_SetDigitalOutput() (_TRISB4 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC4 GPIO Pin which has a custom name of QEI4 to High
 * @pre      The RC4 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define QEI4_SetHigh()          (_LATC4 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC4 GPIO Pin which has a custom name of QEI4 to Low
 * @pre      The RC4 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define QEI4_SetLow()           (_LATC4 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RC4 GPIO Pin which has a custom name of QEI4
 * @pre      The RC4 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define QEI4_Toggle()           (_LATC4 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RC4 GPIO Pin which has a custom name of QEI4
 * @param    none
 * @return   none  
 */
#define QEI4_GetValue()         _RC4

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC4 GPIO Pin which has a custom name of QEI4 as Input
 * @param    none
 * @return   none  
 */
#define QEI4_SetDigitalInput()  (_TRISC4 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC4 GPIO Pin which has a custom name of QEI4 as Output
 * @param    none
 * @return   none  
 */
#define QEI4_SetDigitalOutput() (_TRISC4 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC5 GPIO Pin which has a custom name of QEI3 to High
 * @pre      The RC5 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define QEI3_SetHigh()          (_LATC5 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC5 GPIO Pin which has a custom name of QEI3 to Low
 * @pre      The RC5 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define QEI3_SetLow()           (_LATC5 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RC5 GPIO Pin which has a custom name of QEI3
 * @pre      The RC5 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define QEI3_Toggle()           (_LATC5 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RC5 GPIO Pin which has a custom name of QEI3
 * @param    none
 * @return   none  
 */
#define QEI3_GetValue()         _RC5

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC5 GPIO Pin which has a custom name of QEI3 as Input
 * @param    none
 * @return   none  
 */
#define QEI3_SetDigitalInput()  (_TRISC5 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC5 GPIO Pin which has a custom name of QEI3 as Output
 * @param    none
 * @return   none  
 */
#define QEI3_SetDigitalOutput() (_TRISC5 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC6 GPIO Pin which has a custom name of QEI2 to High
 * @pre      The RC6 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define QEI2_SetHigh()          (_LATC6 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC6 GPIO Pin which has a custom name of QEI2 to Low
 * @pre      The RC6 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define QEI2_SetLow()           (_LATC6 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RC6 GPIO Pin which has a custom name of QEI2
 * @pre      The RC6 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define QEI2_Toggle()           (_LATC6 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RC6 GPIO Pin which has a custom name of QEI2
 * @param    none
 * @return   none  
 */
#define QEI2_GetValue()         _RC6

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC6 GPIO Pin which has a custom name of QEI2 as Input
 * @param    none
 * @return   none  
 */
#define QEI2_SetDigitalInput()  (_TRISC6 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC6 GPIO Pin which has a custom name of QEI2 as Output
 * @param    none
 * @return   none  
 */
#define QEI2_SetDigitalOutput() (_TRISC6 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC7 GPIO Pin which has a custom name of QEI1 to High
 * @pre      The RC7 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define QEI1_SetHigh()          (_LATC7 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC7 GPIO Pin which has a custom name of QEI1 to Low
 * @pre      The RC7 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define QEI1_SetLow()           (_LATC7 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RC7 GPIO Pin which has a custom name of QEI1
 * @pre      The RC7 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define QEI1_Toggle()           (_LATC7 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RC7 GPIO Pin which has a custom name of QEI1
 * @param    none
 * @return   none  
 */
#define QEI1_GetValue()         _RC7

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC7 GPIO Pin which has a custom name of QEI1 as Input
 * @param    none
 * @return   none  
 */
#define QEI1_SetDigitalInput()  (_TRISC7 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC7 GPIO Pin which has a custom name of QEI1 as Output
 * @param    none
 * @return   none  
 */
#define QEI1_SetDigitalOutput() (_TRISC7 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC8 GPIO Pin which has a custom name of TXENABLE to High
 * @pre      The RC8 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define TXENABLE_SetHigh()          (_LATC8 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC8 GPIO Pin which has a custom name of TXENABLE to Low
 * @pre      The RC8 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define TXENABLE_SetLow()           (_LATC8 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RC8 GPIO Pin which has a custom name of TXENABLE
 * @pre      The RC8 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define TXENABLE_Toggle()           (_LATC8 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RC8 GPIO Pin which has a custom name of TXENABLE
 * @param    none
 * @return   none  
 */
#define TXENABLE_GetValue()         _RC8

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC8 GPIO Pin which has a custom name of TXENABLE as Input
 * @param    none
 * @return   none  
 */
#define TXENABLE_SetDigitalInput()  (_TRISC8 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC8 GPIO Pin which has a custom name of TXENABLE as Output
 * @param    none
 * @return   none  
 */
#define TXENABLE_SetDigitalOutput() (_TRISC8 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC10 GPIO Pin which has a custom name of D0 to High
 * @pre      The RC10 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define D0_SetHigh()          (_LATC10 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC10 GPIO Pin which has a custom name of D0 to Low
 * @pre      The RC10 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define D0_SetLow()           (_LATC10 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RC10 GPIO Pin which has a custom name of D0
 * @pre      The RC10 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define D0_Toggle()           (_LATC10 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RC10 GPIO Pin which has a custom name of D0
 * @param    none
 * @return   none  
 */
#define D0_GetValue()         _RC10

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC10 GPIO Pin which has a custom name of D0 as Input
 * @param    none
 * @return   none  
 */
#define D0_SetDigitalInput()  (_TRISC10 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC10 GPIO Pin which has a custom name of D0 as Output
 * @param    none
 * @return   none  
 */
#define D0_SetDigitalOutput() (_TRISC10 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC11 GPIO Pin which has a custom name of D1 to High
 * @pre      The RC11 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define D1_SetHigh()          (_LATC11 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC11 GPIO Pin which has a custom name of D1 to Low
 * @pre      The RC11 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define D1_SetLow()           (_LATC11 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RC11 GPIO Pin which has a custom name of D1
 * @pre      The RC11 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define D1_Toggle()           (_LATC11 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RC11 GPIO Pin which has a custom name of D1
 * @param    none
 * @return   none  
 */
#define D1_GetValue()         _RC11

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC11 GPIO Pin which has a custom name of D1 as Input
 * @param    none
 * @return   none  
 */
#define D1_SetDigitalInput()  (_TRISC11 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC11 GPIO Pin which has a custom name of D1 as Output
 * @param    none
 * @return   none  
 */
#define D1_SetDigitalOutput() (_TRISC11 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC12 GPIO Pin which has a custom name of D2 to High
 * @pre      The RC12 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define D2_SetHigh()          (_LATC12 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC12 GPIO Pin which has a custom name of D2 to Low
 * @pre      The RC12 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define D2_SetLow()           (_LATC12 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RC12 GPIO Pin which has a custom name of D2
 * @pre      The RC12 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define D2_Toggle()           (_LATC12 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RC12 GPIO Pin which has a custom name of D2
 * @param    none
 * @return   none  
 */
#define D2_GetValue()         _RC12

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC12 GPIO Pin which has a custom name of D2 as Input
 * @param    none
 * @return   none  
 */
#define D2_SetDigitalInput()  (_TRISC12 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC12 GPIO Pin which has a custom name of D2 as Output
 * @param    none
 * @return   none  
 */
#define D2_SetDigitalOutput() (_TRISC12 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC13 GPIO Pin which has a custom name of D3 to High
 * @pre      The RC13 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define D3_SetHigh()          (_LATC13 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC13 GPIO Pin which has a custom name of D3 to Low
 * @pre      The RC13 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define D3_SetLow()           (_LATC13 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RC13 GPIO Pin which has a custom name of D3
 * @pre      The RC13 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define D3_Toggle()           (_LATC13 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RC13 GPIO Pin which has a custom name of D3
 * @param    none
 * @return   none  
 */
#define D3_GetValue()         _RC13

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC13 GPIO Pin which has a custom name of D3 as Input
 * @param    none
 * @return   none  
 */
#define D3_SetDigitalInput()  (_TRISC13 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC13 GPIO Pin which has a custom name of D3 as Output
 * @param    none
 * @return   none  
 */
#define D3_SetDigitalOutput() (_TRISC13 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD10 GPIO Pin which has a custom name of IO_LED to High
 * @pre      The RD10 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define IO_LED_SetHigh()          (_LATD10 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD10 GPIO Pin which has a custom name of IO_LED to Low
 * @pre      The RD10 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_LED_SetLow()           (_LATD10 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RD10 GPIO Pin which has a custom name of IO_LED
 * @pre      The RD10 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_LED_Toggle()           (_LATD10 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RD10 GPIO Pin which has a custom name of IO_LED
 * @param    none
 * @return   none  
 */
#define IO_LED_GetValue()         _RD10

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD10 GPIO Pin which has a custom name of IO_LED as Input
 * @param    none
 * @return   none  
 */
#define IO_LED_SetDigitalInput()  (_TRISD10 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD10 GPIO Pin which has a custom name of IO_LED as Output
 * @param    none
 * @return   none  
 */
#define IO_LED_SetDigitalOutput() (_TRISD10 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD13 GPIO Pin which has a custom name of LED1 to High
 * @pre      The RD13 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define LED1_SetHigh()          (_LATD13 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD13 GPIO Pin which has a custom name of LED1 to Low
 * @pre      The RD13 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED1_SetLow()           (_LATD13 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RD13 GPIO Pin which has a custom name of LED1
 * @pre      The RD13 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED1_Toggle()           (_LATD13 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RD13 GPIO Pin which has a custom name of LED1
 * @param    none
 * @return   none  
 */
#define LED1_GetValue()         _RD13

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD13 GPIO Pin which has a custom name of LED1 as Input
 * @param    none
 * @return   none  
 */
#define LED1_SetDigitalInput()  (_TRISD13 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD13 GPIO Pin which has a custom name of LED1 as Output
 * @param    none
 * @return   none  
 */
#define LED1_SetDigitalOutput() (_TRISD13 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Initializes the PINS module
 * @param    none
 * @return   none  
 */
void PINS_Initialize(void);

/**
 * @ingroup  pinsdriver
 * @brief    This function is callback for QEI3 Pin
 * @param    none
 * @return   none   
 */
void QEI3_CallBack(void);

/**
 * @ingroup  pinsdriver
 * @brief    This function is callback for QEI2 Pin
 * @param    none
 * @return   none   
 */
void QEI2_CallBack(void);

/**
 * @ingroup  pinsdriver
 * @brief    This function is callback for QEI1 Pin
 * @param    none
 * @return   none   
 */
void QEI1_CallBack(void);


/**
 * @ingroup    pinsdriver
 * @brief      This function assigns a function pointer with a callback address
 * @param[in]  InterruptHandler - Address of the callback function 
 * @return     none  
 */
void QEI3_SetInterruptHandler(void (* InterruptHandler)(void));

/**
 * @ingroup    pinsdriver
 * @brief      This function assigns a function pointer with a callback address
 * @param[in]  InterruptHandler - Address of the callback function 
 * @return     none  
 */
void QEI2_SetInterruptHandler(void (* InterruptHandler)(void));

/**
 * @ingroup    pinsdriver
 * @brief      This function assigns a function pointer with a callback address
 * @param[in]  InterruptHandler - Address of the callback function 
 * @return     none  
 */
void QEI1_SetInterruptHandler(void (* InterruptHandler)(void));


#endif
