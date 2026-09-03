// This is a guard condition so that contents of this file are not included
// more than once.  
#pragma once

#include <stdint.h>
#include "math.h" 
#include <libq.h>

// https://chummersone.github.io/qformat.html

#define FIXED_POINT16_FRACTIONAL_BITS 12
// Signed int16 fixed-point format with 12 fractional bits
// Value range: -8 to 7.999755859375
// Resolution: 0.000244140625

#define FIXED_POINT32_FRACTIONAL_BITS 11
// Signed int32 fixed-point format with 11 fractional bits
// Value range: -1048576 to 1048575.99951171875
// Resolution: 0.00048828125

typedef int16_t fixed16_point_t;
typedef int32_t fixed32_point_t;

float fixed16_to_float(fixed16_point_t input);
fixed16_point_t float_to_fixed16(float input);
double fixed32_to_double(fixed32_point_t input);
fixed32_point_t double_to_fixed32(double input);

