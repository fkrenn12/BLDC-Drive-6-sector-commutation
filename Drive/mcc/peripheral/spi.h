#pragma once

#include "../mcc_generated_files/system/pins.h"

void SPI_DAC_MCP48CMB24_Initialize(void);
void SPI_IOEXPANDER_MCP23S18_Initialize(void);
void MCP23S18_WriteRegister(uint8_t reg_addr, uint8_t byteData);
uint8_t MCP23S18_ReadRegister1(uint8_t reg_addr); // implementation not tested
void SPI1_WordWrite(uint16_t wordData);
void SPI1_ByteWrite(uint8_t byteData);

static inline void SPI_DAC_MCP48CMB24_Ch0(uint16_t data16bit)
{
    // approx. 300ns for unblocked transfer
    while(SPI1STATLbits.SPITBF == 1);
    SPI1BUFL = data16bit << 8;                  
    SPI1BUFH = (((data16bit >> 8) & 0x000F) | 0x0000); 
}

static inline void SPI_DAC_MCP48CMB24_Ch1(uint16_t data16bit)
{
    // approx. 300ns for unblocked transfer
    while(SPI1STATLbits.SPITBF == 1);
    SPI1BUFL = data16bit << 8;                 
    SPI1BUFH = (((data16bit >> 8) & 0x000F) | 0x0800); 
}

static inline void SPI_DAC_MCP48CMB24_Ch2(uint16_t data16bit)
{
    // approx. 300ns for unblocked transfer
    while(SPI1STATLbits.SPITBF == 1);
    SPI1BUFL = data16bit << 8;            
    SPI1BUFH = (((data16bit >> 8) & 0x000F) | 0x1000); 
}

static inline void SPI_DAC_MCP48CMB24_Ch3(uint16_t data16bit)
{
    // approx. 300ns for unblocked transfer
    while(SPI1STATLbits.SPITBF == 1);
    SPI1BUFL = data16bit << 8;             
    SPI1BUFH = (((data16bit >> 8) & 0x000F) | 0x1800);
}

static inline void MCP23S18_CS_SetLow(void)
{
    // Implementation for setting MCP23S18 CS pin low
}

static inline void MCP23S18_CS_SetHigh(void)
{
    // Implementation for setting MCP23S18 CS pin high
}

static inline void MCP48CMB24_CS_SetLow(void)
{
    // Implementation for setting MCP48CMB24 CS pin low
}

static inline void MCP48CMB24_CS_SetHigh(void)
{
    // Implementation for setting MCP48CMB24 CS pin high
}