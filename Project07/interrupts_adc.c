#include "msp430.h"
#include "macros.h"

// Global ADC Variables
volatile unsigned int ADC_Thumb = 0;
volatile unsigned int ADC_Left = 0;
volatile unsigned int ADC_Right = 0;
volatile unsigned int ADC_Channel = 5; // Assuming 5 is your THUMB_WHEEL_CH

#pragma vector = ADC_VECTOR
__interrupt void ADC_ISR(void){
    switch(__even_in_range(ADCIV,ADCIV_ADCIFG)){
        case ADCIV_NONE:
            break;
        case ADCIV_ADCOVIFG:    // when a conversion result is written to the ADCMEM0
            break;              // before its previous conversion result was read.
        case ADCIV_ADCTOVIFG:   // ADC conversion-time overflow
            break;
        case ADCIV_ADCHIIFG:    // window comparator interrupt flags
            break;
        case ADCIV_ADCLOIFG:    // window comparator interrupt flag
            break;
        case ADCIV_ADCINIFG:    // window comparator interrupt flag
            break;

        case ADCIV_ADCIFG:      // ADCMEM0 memory register with the conversion result
            ADCCTL0 &= ~ADCENC; // Disable ENC bit.

            switch (ADC_Channel++){
                case 0x00:                              // Channel A2 Interrupt
                    ADC_Left = ADCMEM0 >> 2;            // Move result into Global values
                    ADCMCTL0 &= ~ADCINCH_2;             // Disable Last channel A2
                    ADCMCTL0 |= ADCINCH_3;              // Enable Next channel A3
                    break;

                case 0x01:                              // Channel A3 Interrupt
                    ADC_Right = ADCMEM0 >> 2;           // Move result into Global values
                    ADCMCTL0 &= ~ADCINCH_3;             // Disable Last channel A3
                    ADCMCTL0 |= ADCINCH_5;              // Enable Next channel A5 (Thumb wheel)
                    break;

                case 0x02:                              // Channel A5 Interrupt (Fixed slide typo)
                    ADC_Thumb = ADCMEM0 >> 2;           // Move result into Global values
                    ADCMCTL0 &= ~ADCINCH_5;             // Disable Last channel A5
                    ADCMCTL0 |= ADCINCH_2;              // Enable First channel 2
                    ADC_Channel = 0;                    // Reset channel counter
                    break;

                default:
                    break;
            }

            ADCCTL0 |= ADCENC;                          // Enable Conversions
            ADCCTL0 |= ADCSC;                           // Start next sample
            break;

        default:
            break;
    }
}
