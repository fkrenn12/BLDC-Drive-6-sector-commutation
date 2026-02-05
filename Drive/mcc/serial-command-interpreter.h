#pragma once

#include "main.h"
#include "global.h"
#include "peripheral/uart1.h"
#include "peripheral/uart2.h"
#include "mcc_generated_files/system/pins.h"  
#include "drive.h"
void SerialCommandRxService(void);
void SerialCommandTxService(void);