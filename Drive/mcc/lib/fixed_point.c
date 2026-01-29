# include "fixed_point.h"

/*
 * Converts a fixed-point number to its floating-point representation.
 *
 * @param input The fixed-point number to convert.
 * @return The corresponding floating-point value.
 */
float fixed16_to_float(fixed16_point_t input)
{
    return ((float)input / (float)(1 << FIXED_POINT16_FRACTIONAL_BITS));
}

/*
 * Converts a floating-point number to its fixed-point representation.
 *
 * @param input The floating-point number to convert.
 * @return The corresponding fixed-point value.
 */
fixed16_point_t float_to_fixed16(float input)
{
    return (fixed16_point_t) (round(input * (1 << FIXED_POINT16_FRACTIONAL_BITS)));
}

/*
 * Converts a 32-bit fixed-point number to its floating-point representation.
 *
 * @param input The 32-bit fixed-point number to be converted.
 * @return The floating-point representation of the input 32-bit fixed-point value.
 */
double fixed32_to_double(fixed32_point_t input)
{
    // Use double for division to keep precision, then cast to float
    return (double)input / (double)((int32_t)1 << FIXED_POINT32_FRACTIONAL_BITS);
}

/*
 * Converts a floating-point number to a 32-bit fixed-point representation.
 *
 * @param input The floating-point number to be converted.
 * @return The corresponding 32-bit fixed-point representation of the input.
 */
fixed32_point_t double_to_fixed32(double input)
{
    double scaled = input * (double)((int32_t)1 << FIXED_POINT32_FRACTIONAL_BITS);
    return (fixed32_point_t)llround(scaled);
}


