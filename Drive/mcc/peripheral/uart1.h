#pragma once 

#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <inttypes.h>
#include <stdbool.h>
#include "../lib/ringbuffer.h"
#include <xc.h>

void UART1_Initialize(void);
void UART1_WriteBlockingByte(uint8_t txData);
void UART1_WriteBlockingString(const char* str);
void UART1_WriteNoneBlockingString(const char* str);
bool UART1_RxBufferedAvailable(void);
uint8_t UART1_RxBufferedReadByte(void);
void UART1_TxBufferedService(void);
bool UART1_TxBufferedAvailable(void);
uint8_t UART1_TxReadBufferedByte(void);
bool UART1_IsTxDone(void);
bool UART1_IsTxReady(void);