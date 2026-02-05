

#pragma once

#include "mcc_generated_files/adc/adc1.h"
#include "mcc_generated_files/system/pins.h"
#include "global.h"
#include "pwm-sector-detection-commutation-speed-measurement.h"

uint16_t ADC_Result(enum ADC_CHANNEL channel);
void ADC_SoftwareTriggerChannelSequencing(void);
void ADC_Callback(enum ADC_CHANNEL channel, uint16_t adcVal);
