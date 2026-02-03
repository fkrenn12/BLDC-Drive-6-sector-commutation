#include "pi-controller.h"

//Structure defining PI controller parameters



//Function to initialize the PI controller
void PIController_Init(PIController* controller, fixed32_point_t kp, fixed32_point_t ki, fixed32_point_t limit_min, fixed32_point_t limit_max)
{
    controller->kp = kp;
    controller->ki = ki;
    controller->limit_max = limit_max;
    controller->limit_min = limit_min;
    controller->proportional = 0;
    controller->integrator = 0;
    controller->saturated = 0;
    controller->output = 0;
    controller->error = 0;
    controller->setpoint = 0;
}

//Function to synthetise the PI controller (kr-value)
void PIController_Synthetise_kp(PIController* controller, fixed32_point_t kp)
    {controller->kp = kp;}

//Function to synthetise the PI controller (ki-value)
void PIController_Synthetise_ki(PIController* controller, fixed32_point_t ki)
    {controller->ki = ki;}

void PIController_ResetIntegrator(PIController* controller)
    {controller->integrator = 0;}

fixed32_point_t PIController_GetIntegrator(PIController* controller)
    {return controller->integrator;}

void PIController_SetIntegrator(PIController* controller, fixed32_point_t integrator)
    {controller->integrator = integrator;}

//Function to calculate the PI controller output
fixed32_point_t PIController_Compute(PIController *controller, fixed32_point_t setpoint, fixed32_point_t measured_value)
{
    controller->error = setpoint - measured_value;
    controller->setpoint = setpoint;
    controller->proportional = (controller->kp * controller->error) >> FIXED_POINT32_FRACTIONAL_BITS;
    controller->integrator += ((controller->ki  * controller->error)) >> FIXED_POINT32_FRACTIONAL_BITS;
    fixed32_point_t y = controller->proportional + controller->integrator;
    controller->output = (y > controller->limit_max) ? controller->limit_max : (y < controller->limit_min ? controller->limit_min : y);
    fixed32_point_t saturation = controller->output  -  y;
    controller->saturated = (saturation < 0) ? 1 : 0;
    controller->integrator += saturation;
    return controller->output ;
}