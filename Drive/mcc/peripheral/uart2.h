#pragma once 

#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <inttypes.h>
#include <stdbool.h>
#include "../lib/ringbuffer.h"
#include <xc.h>

void UART2_Initialize(void);
void UART2_WriteNoneBlockingString(const char* str);
void UART2_Service_loop(void);
void UART2_WriteBlockingByte(uint8_t txData);
void UART2_WriteBlockingString(const char* str);
void UART2_WriteNonBlockingByte(const uint8_t txData);
void UART2_WriteNoneBlockingString(const char* str);
uint8_t UART2_RxBufferedReadByte(void);
bool UART2_RxBufferedAvailable(void);
void UART2_RXBufferedService(void);
void UART2_TxBufferedService(void);
bool UART2_IsTxDone(void);
bool UART2_IsTxReady(void);