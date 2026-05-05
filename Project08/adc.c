#include "msp430.h"
#include "macros.h"

void Init_ADC(void){
    //-----------------------------------------------------------------------------
    // V_DETECT_L      (0x04) // Pin 2 A2
    // V_DETECT_R      (0x08) // Pin 3 A3
    // V_THUMB         (0x20) // Pin 5 A5
    //-----------------------------------------------------------------------------
    // ADCCTL0 Register


    ADCCTL0 = 0;                        // Reset
    ADCCTL0 |=  ADCSHT_2;                // 16 ADC clocks
    ADCCTL0 |=  ADCMSC;                  // MSC
    ADCCTL0 |=  ADCON;                   // ADC ON

    // ADCCTL1 Register
    ADCCTL1 = 0;                // Reset
    ADCCTL1 |=  ADCSHS_0;        // 00b = ADCSC bit
    ADCCTL1 |=  ADCSHP;          // ADC sample-and-hold SAMPCON signal from sampling timer
    ADCCTL1 &= ~ADCISSH;        // ADC invert signal sample-and-hold.
    ADCCTL1 |=  ADCDIV_0;        // ADC clock divider - 000b = Divide by 1
    ADCCTL1 |=  ADCSSEL_0;       // ADC clock MODCLK
    ADCCTL1 |=  ADCCONSEQ_0;     // ADC conversion sequence 00b = Single-channel single-conversion
    // ADCCTL1 & ADCBUSY  identifies a conversion is in process

    // ADCCTL2 Register
    ADCCTL2 = 0;
    ADCCTL2 |= ADCPDIV0;
    ADCCTL2 |= ADCRES_2;
    ADCCTL2 &= ~ADCDF;
    ADCCTL2 &= ~ADCSR;

    // ADCMCTL0 Register
    ADCMCTL0 |=  ADCSREF_0;
    ADCMCTL0 |=  ADCINCH_2;
    ADCIE    |=  ADCIE0;
    ADCCTL0  |=  ADCENC;
    ADCCTL0  |=  ADCSC;

}

