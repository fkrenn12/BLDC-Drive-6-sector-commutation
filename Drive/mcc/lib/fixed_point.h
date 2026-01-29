// This is a guard condition so that contents of this file are not included
// more than once.  
#pragma once

#include <stdint.h>
#include "math.h" 
#include <libq.h>

// https://chummersone.github.io/qformat.html


// Signed Q4.12
// Fractional value range: -8 to 7.999755859375
// Resolution: 0.000244140625

// Signed Q8.23
// Fractional value range: -256 to 255.9999998807907
// Resolution: 0.0000001192092895


#define FIXED_POINT16_FRACTIONAL_BITS 12
#define FIXED_POINT32_FRACTIONAL_BITS 11

typedef int16_t fixed16_point_t;
typedef int32_t fixed32_point_t;

float fixed16_to_float(fixed16_point_t input);
fixed16_point_t float_to_fixed16(float input);
double fixed32_to_double(fixed32_point_t input);
fixed32_point_t double_to_fixed32(double input);

