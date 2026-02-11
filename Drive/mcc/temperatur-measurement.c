#include "temperatur-measurement.h"

// KI generated Code

// ADC lookup for 10k NTC (B=3425 K) in series with 1.5k, Vcc=3.3V
// Node voltage measured by 12-bit ADC (0..4095). Values at 0..100°C in 5°C steps.
// Order: from cold (0°C, higher ADC) to hot (100°C, lower ADC).
static const struct { uint16_t adc; int16_t t_deci; } ntc_adclut_0_100C[] = {
    {3635,    0},   //  0.0°C
    {3548,   50},   //  5.0°C
    {3452,  100},   // 10.0°C
    {3353,  150},   // 15.0°C
    {3256,  200},   // 20.0°C
    {3165,  250},   // 25.0°C
    {3078,  300},   // 30.0°C
    {2996,  350},   // 35.0°C
    {2919,  400},   // 40.0°C
    {2847,  450},   // 45.0°C
    {2779,  500},   // 50.0°C
    {2716,  550},   // 55.0°C
    {2657,  600},   // 60.0°C
    {2601,  650},   // 65.0°C
    {2549,  700},   // 70.0°C
    {2499,  750},   // 75.0°C
    {2452,  800},   // 80.0°C
    {2407,  850},   // 85.0°C
    {2365,  900},   // 90.0°C
    {2325,  950},   // 95.0°C
    {2287, 1000},   // 100.0°C
};

#define NTC_LUT_N  (sizeof(ntc_adclut_0_100C)/sizeof(ntc_adclut_0_100C[0]))

// Returns temperature in 0.1°C (deci-degC) from ADC reading.
// Clamps below 0°C to 0.0°C and above 100°C to 100.0°C.
int16_t NTC_Temperature_FromADC(uint16_t adc)
{
    // Clamp outside range
    if (adc >= ntc_adclut_0_100C[0].adc)  return ntc_adclut_0_100C[0].t_deci;            // ≤ 0°C range
    if (adc <= ntc_adclut_0_100C[NTC_LUT_N-1].adc) return ntc_adclut_0_100C[NTC_LUT_N-1].t_deci; // ≥ 100°C

    // Find segment [i, i+1] with adc between them (table is strictly descending in adc)
    for (unsigned i = 0; i + 1 < NTC_LUT_N; ++i) {
        uint16_t a_hi = ntc_adclut_0_100C[i].adc;       // higher ADC, lower index (colder)
        uint16_t a_lo = ntc_adclut_0_100C[i+1].adc;     // lower ADC, higher index (hotter)
        if (adc <= a_hi && adc >= a_lo) {
            int16_t t0 = ntc_adclut_0_100C[i].t_deci;
            int16_t t1 = ntc_adclut_0_100C[i+1].t_deci;
            uint16_t da = (uint16_t)(a_hi - a_lo);
            uint16_t dx = (uint16_t)(a_hi - adc);
            // Linear interpolation: t = t0 + (t1 - t0) * (adc_hi - adc) / (adc_hi - adc_lo)
            return (int16_t)(t0 + (int32_t)(t1 - t0) * dx / da);
        }
    }
    // Should not reach here due to clamps; return upper bound as fallback
    return ntc_adclut_0_100C[NTC_LUT_N-1].t_deci;
}
