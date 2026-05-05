#include "msp430.h"
#include "macros.h"

volatile unsigned int ADC_Thumb = 0;
volatile unsigned int ADC_Left = 0;
volatile unsigned int ADC_Right = 0;
volatile unsigned int ADC_Channel = THUMB_WHEEL_CH; // Starts at 5

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

            // We MUST disable ADC before changing channels
            ADCCTL0 &= ~ADCENC;

            // Read value and switch to next channel
            if(ADC_Channel == THUMB_WHEEL_CH){
                ADC_Thumb = ADCMEM0;
                ADC_Channel = LEFT_DETECTOR_CH;
                ADCMCTL0 &= ~0x0F;
                ADCMCTL0 |= ADCINCH_2;
            }
            else if (ADC_Channel == LEFT_DETECTOR_CH){
                ADC_Left = ADCMEM0;
                ADC_Channel = RIGHT_DETECTOR_CH;
                ADCMCTL0 &= ~0x0F;
                ADCMCTL0 |= ADCINCH_3;
            }
            else if (ADC_Channel == RIGHT_DETECTOR_CH){
                ADC_Right = ADCMEM0;
                ADC_Channel = THUMB_WHEEL_CH;
                ADCMCTL0 &= ~0x0F;
                ADCMCTL0 |= ADCINCH_5;
            }

            ADCCTL0 |= ADCENC; // Re-enable
            ADCCTL0 |= ADCSC;  // Start next sample
            break;

        default:
            break;
    }
}
