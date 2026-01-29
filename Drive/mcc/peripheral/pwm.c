#include "pwm.h"
#include <xc.h>

#define DEAD_TIME 400   // 400*1/200Mhz = 2µs 

#define SOC 0b000
#define CLIENT_SOC 0b010 
#define IMMEDIATE 0b001
#define CLIENT_IMMEDIATE 0b011
#define LOCAL_EOC 0x0
#define TRG_BY_PG1_PG5 0x1
#define TRG_BY_PG2_PG6 0x2
#define TRG_BY_PG3_PG7 0x3

void PWM_Initialize(void){
    PCLKCONbits.MCLKSEL = 0b00;     // High Speed PLL Clock = 200Mhz
    PCLKCONbits.DIVSEL = 0b00;      // Divided by 2 (not for Master Clock)
    MPER = 8191;                    // 200Mhz/(9999+1) = 24.41kHz
    MDC = 0;                        // 0% for init

    // *****************************************************************************
    //                          PWM Generator 1
    //******************************************************************************
    PG1CONLbits.ON = 0;             // OFF during initialization
    
    PG1DC = 0;                      // unused
    PG1PER = 0;                     // unused
    PG1PHASE = 0;                   // unused
    
    PG1CONHbits.MDCSEL = 1;         // PWM Generator uses MDC Register 
    PG1CONHbits.MPERSEL = 1;        // PWM Generator uses MPER Register 
    PG1CONHbits.MPHSEL = 0;         // PWM Generator uses MPHASE Register 
    PG1CONHbits.MSTEN = 1;          // 0: PWM Generator does not broadcast UPDREQ status bit or EOC signal 1: does
    PG1CONHbits.UPDMOD = SOC;       // 0b000 SOC Update Data registers at start of next PWM cycle if UPDREQ = 1. The
                                    // UPDATE status bit will be cleared automatically after the update occurs.(1)
                                    // 0b001 Immediate Update Data registers immediately, or as soon as possible, if
                                    // UPDREQ = 1. The UPDATE status bit will be cleared automatically after the update occurs.
                                    // 0b010 Slaved SOC Update Data registers at start of next cycle if a master update request is
                                    // received. A master update request will be transmitted if MSTEN = 1 and UPDREQ = 1 for the requesting PWM Generator.
                                    // 0b011 Slaved Immediate Update Data registers immediately, or as soon as possible, when a master
                                    // update request is received. A master update request will be transmitted if MSTEN = 1 and UPDREQ = 1 for the requesting PWM Generator.
   
    PG1CONHbits.TRGMOD = 0;         // 0 Single Trigger Mode / 1 Retriggerable mode
    PG1CONHbits.SOCS = 0;           // Start of Cycle Selection 0x0 Local EOC

    
    PG1CONLbits.TRGCNT = 0b000;     // 1 Cycle after Trigger
    PG1CONLbits.CLKSEL = 0b01;      // PWM Generator uses Master clock by PCLKCONbits.MCLKSEL 
    PG1CONLbits.MODSEL = 0b000;     // 0b000 Independent Edge PWM Mode / 0b100 Center Aligned PWM mode

    PG1IOCONHbits.CAPSRC = 0b000;   // No hardware source for time base capture
    PG1IOCONHbits.DTCMPSEL = 0b0;   // Dead time compensation is controlled by PCI Sync logic
    PG1IOCONHbits.PMOD = 0b00;      // PWM Generator outputs operate in Complementary mode
    PG1IOCONHbits.PENH = 1;         // PWM Generator controls the PWMxH output pin
    PG1IOCONHbits.PENL = 1;         // PWM Generator controls the PWMxL output pin
    PG1IOCONHbits.POLH = 0;         // Output pin is active-high
    PG1IOCONHbits.POLL = 0;         // Output pin is active-high

    PG1IOCONLbits.CLMOD = 0;        // If PCI current limit is active, then the CLDAT<1:0> bits define the PWM output levels
    PG1IOCONLbits.SWAP = 0;         // PWMxH/L signals are mapped to their respective pins
    PG1IOCONLbits.OVRENH = 0;       // OVRDAT<1> provides data for output on the PWMxH pin
    PG1IOCONLbits.OVRENL = 0;       // OVRDAT<0> provides data for output on the PWMxL pin
    PG1IOCONLbits.OVRDAT = 0b00;    // OVRDAT<1> provides data for PWMxH. - OVRDAT<0> provides data for PWMxL     
    PG1IOCONLbits.OSYNC = 0b00;     // 0b00:User output overrides via the OVRENL/H and OVRDAT<1:0> bits are synchronized to the
                                    // local PWM time base (next Start-of-Cycle)
                                    // 0b01:User output overrides via the OVRENL/H and OVRDAT<1:0> bits occur immediately (as
                                    // soon as possible)
                                    // 0b10:User output overrides via the OVRENL/H and OVRDAT<1:0> bits occur when specified by
                                    // the UPDMOD<2:0> bits in the PGxCONH register
    PG1IOCONLbits.FLTDAT = 0b00;
    PG1IOCONLbits.CLDAT = 0b00;
    PG1IOCONLbits.FFDAT = 0b00;
    PG1IOCONLbits.DBDAT = 0b00;                                        

    PG1EVTLbits.ADTR1PS = 0b00000;  // ADC Trigger 1 Postscaler Selection bits 1:1
    PG1EVTLbits.ADTR1EN3 = 0;       // ADC Trigger 1 Source is PGxTRIGC Compare Event Enable bit
    PG1EVTLbits.ADTR1EN2 = 0;       // ADC Trigger 1 Source is PGxTRIGB Compare Event Enable bit 
    PG1EVTLbits.ADTR1EN1 = 1;       // ADC Trigger 1 Source is PGxTRIGA Compare Event Enable bit

    PG1EVTLbits.UPDTRG = 0b00;      // Update Trigger Select bits
                                    // 0b11 A write of the PGxTRIGA register automatically sets the UPDREQ bit
                                    // 0b10 A write of the PGxPHASE register automatically sets the UPDREQ bit
                                    // 0b01 A write of the PGxDC register automatically sets the UPDREQ bit
                                    // 0b00 User must set the UPDREQ bit (PGxSTAT<3>) manually

    PG1EVTHbits.IEVTSEL = 0;        // Interrupts CPU at EOC
    PG1EVTHbits.ADTR2EN3 = 0;       // ADC Trigger 2 Source is PGxTRIGC Compare Event Enable bit
    PG1EVTHbits.ADTR2EN2 = 1;       // ADC Trigger 2 Source is PGxTRIGB Compare Event Enable bit
    PG1EVTHbits.ADTR2EN1 = 0;       // ADC Trigger 2 Source is PGxTRIGA Compare Event Enable bit
    PG1EVTHbits.ADTR1OFS = 0b00000; // Trigger 1 Offset Selection bits

    PG1DTL = DEAD_TIME;    // Dead time Low 
    PG1DTH = DEAD_TIME;    // Dead time High
    PG1TRIGA = 0;
    PG1TRIGB = 0;
    PG1TRIGC = 0;

    // *****************************************************************************
    //                          PWM Generator 2
    //******************************************************************************
    PG2CONLbits.ON = 0;             // OFF during initialization
    PG2DC = 0;                      // unused
    PG2PER = 0;                     // unused
    PG2PHASE = 0;                   // unused
    PG2CONHbits.MDCSEL = 1;         // PWM Generator uses MDC Register 
    PG2CONHbits.MPERSEL = 1;        // PWM Generator uses MPER Register 
    PG2CONHbits.MPHSEL = 0;         // PWM Generator uses MPHASE Register 
    PG2CONHbits.MSTEN = 0;          // PWM Generator does not broadcast UPDREQ status bit or EOC signal 
    PG2CONHbits.UPDMOD = CLIENT_SOC;       // 0b000 SOC Update Data registers at start of next PWM cycle if UPDREQ = 1. The
                                    // UPDATE status bit will be cleared automatically after the update occurs.(1)
                                    // 0b001 Immediate Update Data registers immediately, or as soon as possible, if
                                    // UPDREQ = 1. The UPDATE status bit will be cleared automatically after the update occurs.
                                    // 010 Slaved SOC Update Data registers at start of next cycle if a master update request is
                                    // received. A master update request will be transmitted if MSTEN = 1 and UPDREQ = 1 for the requesting PWM Generator.
                                    // 0b011 Slaved Immediate Update Data registers immediately, or as soon as possible, when a master
                                    // update request is received. A master update request will be transmitted if MSTEN = 1 and UPDREQ = 1 for the requesting PWM Generator.
    PG2CONHbits.TRGMOD = 0;         // 0 Single Trigger Mode / 1 Retriggerable mode
    PG2CONHbits.SOCS = 0b0001;      // 0b0000 Start of Cycle Selection 0x0 Local EOC
                                    // 0b0001 Trigger output selected by PG1 PGTRGSEL<2:0> bits (PGxEVTL<2:0>) 

    PG2CONLbits.TRGCNT = 0b000;     // 1 Cycle after Trigger
    PG2CONLbits.CLKSEL = 0b01;      // PWM Generator uses Master clock by PCLKCONbits.MCLKSEL 
    PG2CONLbits.MODSEL = 0b000;     // 0b000 Independent Edge PWM Mode / 0b100 Center Aligned PWM mode

    PG2IOCONHbits.CAPSRC = 0b000;   // No hardware source for time base capture
    PG2IOCONHbits.DTCMPSEL = 0b0;   // Dead time compensation is controlled by PCI Sync logic
    PG2IOCONHbits.PMOD = 0b00;      // PWM Generator outputs operate in Complementary mode
    PG2IOCONHbits.PENH = 1;         // PWM Generator controls the PWMxH output pin
    PG2IOCONHbits.PENL = 1;         // PWM Generator controls the PWMxL output pin
    PG2IOCONHbits.POLH = 0;         // Output pin is active-high
    PG2IOCONHbits.POLL = 0;         // Output pin is active-high

    PG2IOCONLbits.CLMOD = 0;        // If PCI current limit is active, then the CLDAT<1:0> bits define the PWM output levels
    PG2IOCONLbits.SWAP = 0;         // PWMxH/L signals are mapped to their respective pins
    PG2IOCONLbits.OVRENH = 0;       // OVRDAT<1> provides data for output on the PWMxH pin
    PG2IOCONLbits.OVRENL = 0;       // OVRDAT<0> provides data for output on the PWMxL pin
    PG2IOCONLbits.OVRDAT = 0b00;    // OVRDAT<1> provides data for PWMxH. - OVRDAT<0> provides data for PWMxL     
    PG2IOCONLbits.OSYNC = 0b00;     // 0b00:User output overrides via the OVRENL/H and OVRDAT<1:0> bits are synchronized to the
                                    // local PWM time base (next Start-of-Cycle)
                                    // 0b01:User output overrides via the OVRENL/H and OVRDAT<1:0> bits occur immediately (as
                                    // soon as possible)
                                    // 0b10:User output overrides via the OVRENL/H and OVRDAT<1:0> bits occur when specified by
                                    // the UPDMOD<2:0> bits in the PGxCONH register
    PG2IOCONLbits.FLTDAT = 0b00;
    PG2IOCONLbits.CLDAT = 0b00;
    PG2IOCONLbits.FFDAT = 0b00;
    PG2IOCONLbits.DBDAT = 0b00;                                        

    PG2EVTLbits.ADTR1PS = 0b00000;  // ADC Trigger 1 Postscaler Selection bits 1:1
    PG2EVTLbits.ADTR1EN3 = 0;       // ADC Trigger 1 Source is PGxTRIGC Compare Event Enable bit
    PG2EVTLbits.ADTR1EN2 = 0;       // ADC Trigger 1 Source is PGxTRIGB Compare Event Enable bit 
    PG2EVTLbits.ADTR1EN1 = 1;       // ADC Trigger 1 Source is PGxTRIGA Compare Event Enable bit

    PG2EVTLbits.UPDTRG = 0b00;      // Update Trigger Select bits
                                    // 0b11 A write of the PGxTRIGA register automatically sets the UPDREQ bit
                                    // 0b10 A write of the PGxPHASE register automatically sets the UPDREQ bit
                                    // 0b01 A write of the PGxDC register automatically sets the UPDREQ bit
                                    // 0b00 User must set the UPDREQ bit (PGxSTAT<3>) manually
    
    PG2EVTHbits.IEVTSEL = 0;        // Interrupts CPU at EOC
    PG2EVTHbits.ADTR2EN3 = 0;       // ADC Trigger 2 Source is PGxTRIGC Compare Event Enable bit
    PG2EVTHbits.ADTR2EN2 = 1;       // ADC Trigger 2 Source is PGxTRIGB Compare Event Enable bit
    PG2EVTHbits.ADTR2EN1 = 0;       // ADC Trigger 2 Source is PGxTRIGA Compare Event Enable bit
    PG2EVTHbits.ADTR1OFS = 0b00000; // Trigger 1 Offset Selection bits

    PG2DTL = DEAD_TIME;    // Dead time Low 40 * t-Clock
    PG2DTH = DEAD_TIME;    // Dead time High
    PG2TRIGA = 0;
    PG2TRIGB = 0;
    PG2TRIGC = 0;

    // *****************************************************************************
    //                          PWM Generator 3
    //******************************************************************************
    PG3CONLbits.ON = 0;             // OFF during initialization
    PG3DC = 0;                      // unused
    PG3PER = 0;                     // unused
    PG3PHASE = 0;                   // unused
    PG3CONHbits.MDCSEL = 1;         // PWM Generator uses MDC Register 
    PG3CONHbits.MPERSEL = 1;        // PWM Generator uses MPER Register 
    PG3CONHbits.MPHSEL = 0;         // PWM Generator uses MPHASE Register 
    PG3CONHbits.MSTEN = 0;          // PWM Generator does not broadcast UPDREQ status bit or EOC signal 
    PG3CONHbits.UPDMOD = CLIENT_SOC;
                                    // 0b000 SOC Update Data registers at start of next PWM cycle if UPDREQ = 1. The
                                    // UPDATE status bit will be cleared automatically after the update occurs.(1)
                                    // 0b001 Immediate Update Data registers immediately, or as soon as possible, if
                                    // UPDREQ = 1. The UPDATE status bit will be cleared automatically after the update occurs.
                                    // 010 Slaved SOC Update Data registers at start of next cycle if a master update request is
                                    // received. A master update request will be transmitted if MSTEN = 1 and UPDREQ = 1 for the requesting PWM Generator.
                                    // 0b011 Slaved Immediate Update Data registers immediately, or as soon as possible, when a master
                                    // update request is received. A master update request will be transmitted if MSTEN = 1 and UPDREQ = 1 for the requesting PWM Generator.
    PG3CONHbits.TRGMOD = 0;         // 0 Single Trigger Mode / 1 Retriggerable mode
    PG3CONHbits.SOCS = 0b0001;      // 0b0000 Start of Cycle Selection 0x0 Local EOC
                                    // 0b0001 Trigger output selected by PG1 PGTRGSEL<2:0> bits (PGxEVTL<2:0>) 

    PG3CONLbits.TRGCNT = 0b000;     // 1 Cycle after Trigger
    PG3CONLbits.CLKSEL = 0b01;      // PWM Generator uses Master clock by PCLKCONbits.MCLKSEL 
    PG3CONLbits.MODSEL = 0b000;     // 0b000 Independent Edge PWM Mode / 0b100 Center Aligned PWM mode

    PG3IOCONHbits.CAPSRC = 0b000;   // No hardware source for time base capture
    PG3IOCONHbits.DTCMPSEL = 0b0;   // Dead time compensation is controlled by PCI Sync logic
    PG3IOCONHbits.PMOD = 0b00;      // PWM Generator outputs operate in Complementary mode
    PG3IOCONHbits.PENH = 1;         // PWM Generator controls the PWMxH output pin
    PG3IOCONHbits.PENL = 1;         // PWM Generator controls the PWMxL output pin
    PG3IOCONHbits.POLH = 0;         // Output pin is active-high
    PG3IOCONHbits.POLL = 0;         // Output pin is active-high

    PG3IOCONLbits.CLMOD = 0;        // If PCI current limit is active, then the CLDAT<1:0> bits define the PWM output levels
    PG3IOCONLbits.SWAP = 0;         // PWMxH/L signals are mapped to their respective pins
    PG3IOCONLbits.OVRENH = 0;       // OVRDAT<1> provides data for output on the PWMxH pin
    PG3IOCONLbits.OVRENL = 0;       // OVRDAT<0> provides data for output on the PWMxL pin
    PG3IOCONLbits.OVRDAT = 0b00;    // OVRDAT<1> provides data for PWMxH. - OVRDAT<0> provides data for PWMxL     
    PG3IOCONLbits.OSYNC = 0b00;     // 0b00:User output overrides via the OVRENL/H and OVRDAT<1:0> bits are synchronized to the
                                    // local PWM time base (next Start-of-Cycle)
                                    // 0b01:User output overrides via the OVRENL/H and OVRDAT<1:0> bits occur immediately (as
                                    // soon as possible)
                                    // 0b10:User output overrides via the OVRENL/H and OVRDAT<1:0> bits occur when specified by
                                    // the UPDMOD<2:0> bits in the PGxCONH register
    PG3IOCONLbits.FLTDAT = 0b00;
    PG3IOCONLbits.CLDAT = 0b00;
    PG3IOCONLbits.FFDAT = 0b00;
    PG3IOCONLbits.DBDAT = 0b00;                                        

    PG3EVTLbits.ADTR1PS = 0b00000;  // ADC Trigger 1 Postscaler Selection bits 1:1
    PG3EVTLbits.ADTR1EN3 = 0;       // ADC Trigger 1 Source is PGxTRIGC Compare Event Enable bit
    PG3EVTLbits.ADTR1EN2 = 0;       // ADC Trigger 1 Source is PGxTRIGB Compare Event Enable bit 
    PG3EVTLbits.ADTR1EN1 = 1;       // ADC Trigger 1 Source is PGxTRIGA Compare Event Enable bit

    PG3EVTLbits.UPDTRG = 0b00;      // Update Trigger Select bits
                                    // 0b11 A write of the PGxTRIGA register automatically sets the UPDREQ bit
                                    // 0b10 A write of the PGxPHASE register automatically sets the UPDREQ bit
                                    // 0b01 A write of the PGxDC register automatically sets the UPDREQ bit
                                    // 0b00 User must set the UPDREQ bit (PGxSTAT<3>) manually
    
    PG3EVTHbits.IEVTSEL = 0;        // Interrupts CPU at EOC
    PG3EVTHbits.ADTR2EN3 = 0;       // ADC Trigger 2 Source is PGxTRIGC Compare Event Enable bit
    PG3EVTHbits.ADTR2EN2 = 1;       // ADC Trigger 2 Source is PGxTRIGB Compare Event Enable bit
    PG3EVTHbits.ADTR2EN1 = 0;       // ADC Trigger 2 Source is PGxTRIGA Compare Event Enable bit
    PG3EVTHbits.ADTR1OFS = 0b00000; // Trigger 1 Offset Selection bits

    PG3DTL = DEAD_TIME;    // Dead time Low 40 * t-Clock
    PG3DTH = DEAD_TIME;    // Dead time High
    PG3TRIGA = 0;
    PG3TRIGB = 0;
    PG3TRIGC = 0;

    // *****************************************************************************
    //                          PWM Generator 4
    //******************************************************************************
    PG4CONLbits.ON = 0;             // OFF during initialization
    PG4DC = 399;                    // 200Mhz/(399+1) = 500kHz
    PG4PER = 200;                   // 50%
    PG4PHASE = 0;
    PG4CONHbits.MDCSEL = 0;         // PWM Generator uses MDC Register 
    PG4CONHbits.MPERSEL = 0;        // PWM Generator uses MPER Register 
    PG4CONHbits.MPHSEL = 0;         // PWM Generator uses MPHASE Register 
    PG4CONHbits.MSTEN = 0;          // PWM Generator does not broadcast UPDREQ status bit or EOC signal 
    PG4CONHbits.UPDMOD = SOC;
                                    // 0b000 SOC Update Data registers at start of next PWM cycle if UPDREQ = 1. The
                                    // UPDATE status bit will be cleared automatically after the update occurs.(1)
                                    // 0b001 Immediate Update Data registers immediately, or as soon as possible, if
                                    // UPDREQ = 1. The UPDATE status bit will be cleared automatically after the update occurs.
                                    // 010 Slaved SOC Update Data registers at start of next cycle if a master update request is
                                    // received. A master update request will be transmitted if MSTEN = 1 and UPDREQ = 1 for the requesting PWM Generator.
                                    // 0b011 Slaved Immediate Update Data registers immediately, or as soon as possible, when a master
                                    // update request is received. A master update request will be transmitted if MSTEN = 1 and UPDREQ = 1 for the requesting PWM Generator.
    PG4CONHbits.TRGMOD = 0;         // 0 Single Trigger Mode / 1 Retriggerable mode
    PG4CONHbits.SOCS = 0b0000;      // 0b0000 Start of Cycle Selection 0x0 Local EOC
                                    // 0b0001 Trigger output selected by PG1 PGTRGSEL<2:0> bits (PGxEVTL<2:0>) 

    PG4CONLbits.TRGCNT = 0b000;     // 1 Cycle after Trigger
    PG4CONLbits.CLKSEL = 0b01;      // PWM Generator uses Master clock by PCLKCONbits.MCLKSEL 
    PG4CONLbits.MODSEL = 0b000;     // 0b000 Independent Edge PWM Mode / 0b100 Center Aligned PWM mode

    PG4IOCONHbits.CAPSRC = 0b000;   // No hardware source for time base capture
    PG4IOCONHbits.DTCMPSEL = 0b0;   // Dead time compensation is controlled by PCI Sync logic
    PG4IOCONHbits.PMOD = 0b10;      // PWM Generator outputs operate in Complementary mode
    PG4IOCONHbits.PENH = 1;         // PWM Generator controls the PWMxH output pin
    PG4IOCONHbits.PENL = 1;         // PWM Generator controls the PWMxL output pin
    PG4IOCONHbits.POLH = 0;         // Output pin is active-high
    PG4IOCONHbits.POLL = 0;         // Output pin is active-high

    PG4IOCONLbits.CLMOD = 0;        // If PCI current limit is active, then the CLDAT<1:0> bits define the PWM output levels
    PG4IOCONLbits.SWAP = 0;         // PWMxH/L signals are mapped to their respective pins
    PG4IOCONLbits.OVRENH = 0;       // OVRDAT<1> provides data for output on the PWMxH pin
    PG4IOCONLbits.OVRENL = 0;       // OVRDAT<0> provides data for output on the PWMxL pin
    PG4IOCONLbits.OVRDAT = 0b00;    // OVRDAT<1> provides data for PWMxH. - OVRDAT<0> provides data for PWMxL     
    PG4IOCONLbits.OSYNC = 0b00;     // 0b00:User output overrides via the OVRENL/H and OVRDAT<1:0> bits are synchronized to the
                                    // local PWM time base (next Start-of-Cycle)
                                    // 0b01:User output overrides via the OVRENL/H and OVRDAT<1:0> bits occur immediately (as
                                    // soon as possible)
                                    // 0b10:User output overrides via the OVRENL/H and OVRDAT<1:0> bits occur when specified by
                                    // the UPDMOD<2:0> bits in the PGxCONH register
    PG4IOCONLbits.FLTDAT = 0b00;
    PG4IOCONLbits.CLDAT = 0b00;
    PG4IOCONLbits.FFDAT = 0b00;
    PG4IOCONLbits.DBDAT = 0b00;                                        

    PG4EVTLbits.ADTR1PS = 0b00000;  // ADC Trigger 1 Postscaler Selection bits 1:1
    PG4EVTLbits.ADTR1EN3 = 0;       // ADC Trigger 1 Source is PGxTRIGC Compare Event Enable bit
    PG4EVTLbits.ADTR1EN2 = 0;       // ADC Trigger 1 Source is PGxTRIGB Compare Event Enable bit 
    PG4EVTLbits.ADTR1EN1 = 0;       // ADC Trigger 1 Source is PGxTRIGA Compare Event Enable bit

    PG4EVTLbits.UPDTRG = 0b00;      // Update Trigger Select bits
                                    // 0b11 A write of the PGxTRIGA register automatically sets the UPDREQ bit
                                    // 0b10 A write of the PGxPHASE register automatically sets the UPDREQ bit
                                    // 0b01 A write of the PGxDC register automatically sets the UPDREQ bit
                                    // 0b00 User must set the UPDREQ bit (PGxSTAT<3>) manually
    
    PG4EVTHbits.IEVTSEL = 0;        // Interrupts CPU at EOC
    PG4EVTHbits.ADTR2EN3 = 0;       // ADC Trigger 2 Source is PGxTRIGC Compare Event Enable bit
    PG4EVTHbits.ADTR2EN2 = 0;       // ADC Trigger 2 Source is PGxTRIGB Compare Event Enable bit
    PG4EVTHbits.ADTR2EN1 = 0;       // ADC Trigger 2 Source is PGxTRIGA Compare Event Enable bit
    PG4EVTHbits.ADTR1OFS = 0b00000; // Trigger 1 Offset Selection bits

    PG4DTL = 0;    // Dead time Low
    PG4DTH = 0;    // Dead time High
    PG4TRIGA = 0;
    PG4TRIGB = 0;
    PG4TRIGC = 0;
    
    //PWM Generator 1 Interrupt
    IFS4bits.PWM1IF = 0;
    IEC4bits.PWM1IE = 1;  // 1 if enabled
    
    //PWM Generator 2 Interrupt
    IFS4bits.PWM2IF = 0;
    IEC4bits.PWM2IE = 0;
    
    //PWM Generator 3 Interrupt
    IFS4bits.PWM3IF = 0;
    IEC4bits.PWM3IE = 0;
    
    //PWM Generator 4 Interrupt
    IFS4bits.PWM4IF = 0;
    IEC4bits.PWM4IE = 0;

    PG1CONLbits.ON = 1;
    PG2CONLbits.ON = 1;
    PG3CONLbits.ON = 1;
    PG4CONLbits.ON = 1;
}