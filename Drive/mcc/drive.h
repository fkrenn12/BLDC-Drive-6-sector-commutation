#pragma once 

#include <stdlib.h> 
#include <inttypes.h>
#include "global.h"

void drive_init(void);
void drive_run(void);
void drive_stop(uint8_t clamp);
uint8_t drive_state();
int32_t drive_set_speed_rpm(int32_t rpm);
void set_current_limit();
void reset_current_limit();
void set_cruise_mode();
void set_momentum_mode();
