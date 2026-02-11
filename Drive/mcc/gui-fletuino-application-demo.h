#pragma once 
#include "fletuino/fletuino.h"
#include "global.h"
#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <inttypes.h>
#include "lib/pi-controller.h"
#include "drive.h"
#include "adc-current-measurement-and-control.h"
extern TGlobal g;
void start_page(void);
void gui_update(void);