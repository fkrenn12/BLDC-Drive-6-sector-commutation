#include "pi-controller.h"
#include <stdbool.h>
//Structure defining PI controller parameters



//Function to initialize the PI controller
void PIController_Init(PIController* controller, fixed32_point_t kp, fixed32_point_t ki, fixed32_point_t kaw, fixed32_point_t limit_min, fixed32_point_t limit_max)
{
    controller->kp = kp;
    controller->ki = ki;
    controller->kaw = kaw;
    controller->limit_max = limit_max;
    controller->limit_min = limit_min;
    controller->proportional = 0;
    controller->integrator = 0;
    controller->saturated = 0;
    controller->output = 0;
    controller->error = 0;
    controller->setpoint = 0;
    controller->measured_value = 0;
    controller->integrator_intermediate = 0;
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
    controller->measured_value = measured_value;
    controller->setpoint = setpoint;
    controller->error = setpoint - measured_value;
    controller->proportional = (controller->kp * controller->error) >> FIXED_POINT32_FRACTIONAL_BITS;
    controller->integrator_intermediate = ((controller->ki  * controller->error)) >> FIXED_POINT32_FRACTIONAL_BITS;

    fixed32_point_t integrator_candidate = controller->integrator + controller->integrator_intermediate;
    fixed32_point_t y = controller->proportional + integrator_candidate;
    controller->output = (y > controller->limit_max) ? controller->limit_max : (y < controller->limit_min ? controller->limit_min : y);
    controller->saturated = (controller->output != y) ? 1 : 0;

    controller->integrator = integrator_candidate +
        ((controller->kaw * (controller->output - y)) >> FIXED_POINT32_FRACTIONAL_BITS);

    return controller->output ;
}

int16_t PIController_Compute_16(PIController *controller, int16_t setpoint, int16_t measured_value)
{
    int16_t error = setpoint - measured_value;
    int32_t proportional = ((int32_t)(int16_t)controller->kp * error) >> FIXED_POINT32_FRACTIONAL_BITS;
    int32_t integrator_intermediate = ((int32_t)(int16_t)controller->ki * error) >> FIXED_POINT32_FRACTIONAL_BITS;
    int32_t integrator_candidate = controller->integrator + integrator_intermediate;
    int32_t output_unlimited = proportional + integrator_candidate;
    int32_t output = (output_unlimited > controller->limit_max) ? controller->limit_max :
                     (output_unlimited < controller->limit_min ? controller->limit_min : output_unlimited);

    controller->setpoint = setpoint;
    controller->measured_value = measured_value;
    controller->error = error;
    controller->proportional = proportional;
    controller->integrator_intermediate = integrator_intermediate;
    controller->output = output;
    controller->saturated = (output != output_unlimited) ? 1 : 0;

    controller->integrator = integrator_candidate +
        ((controller->kaw * (output - output_unlimited)) >> FIXED_POINT32_FRACTIONAL_BITS);

    return (int16_t)controller->output;
}
