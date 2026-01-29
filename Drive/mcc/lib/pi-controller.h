#pragma once

#include "fixed_point.h"

typedef struct _piController{
    fixed32_point_t kp;               //Proportional gain in fixed-point format
    fixed32_point_t ki;               //Integral gain in fixed-point format
    fixed32_point_t limit_max;        //Maximum output limit to prevent windup
    fixed32_point_t limit_min;        //Minimum output limit to prevent windup
    fixed32_point_t integrator;       //Accumulated integrator value
    fixed32_point_t error;            //Previous error value
    fixed32_point_t output;
    fixed32_point_t setpoint;
    fixed32_point_t proportional;
    uint8_t saturated;
} PIController;

void PIController_Init(PIController* controller, fixed32_point_t kp, fixed32_point_t ki, fixed32_point_t limit_min, fixed32_point_t limit_max);
void PIController_Synthetise_kp(PIController* controller, fixed32_point_t kp);
void PIController_Synthetise_ki(PIController* controller, fixed32_point_t ki);
void PIController_Reset_integrator(PIController* controller);
fixed32_point_t PIController_Compute(PIController *controller, fixed32_point_t setpoint, fixed32_point_t measured_value);
