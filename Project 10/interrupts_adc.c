//------------------------------------------------------------------------------
//
//  Description: Interrupt Service Routine for the ADC.
//  Cycles through the left detector, right detector, and thumbwheel channels.
//
//  Ezana
//  April 2026
//  Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------

#include "msp430.h"
#include "macros.h"

volatile unsigned int ADC_Thumb = 0;
volatile unsigned int ADC_Left = 0;
volatile unsigned int ADC_Right = 0;
volatile unsigned int ADC_Channel = 5;

#pragma vector = ADC_VECTOR
__interrupt void ADC_ISR(void){
    switch(__even_in_range(ADCIV,ADCIV_ADCIFG)){
        case ADCIV_NONE: break;
        case ADCIV_ADCOVIFG: break;
        case ADCIV_ADCTOVIFG: break;
        case ADCIV_ADCHIIFG: break;
        case ADCIV_ADCLOIFG: break;
        case ADCIV_ADCINIFG: break;

        case ADCIV_ADCIFG:
            ADCCTL0 &= ~ADCENC; // Disable conversions while updating

            switch (ADC_Channel++){
                case 0x00:
                    ADC_Left = ADCMEM0 >> 2;
                    ADCMCTL0 &= ~ADCINCH_2;
                    ADCMCTL0 |= ADCINCH_3;
                    break;

                case 0x01:
                    ADC_Right = ADCMEM0 >> 2;
                    ADCMCTL0 &= ~ADCINCH_3;
                    ADCMCTL0 |= ADCINCH_5;
                    break;

                case 0x02:
                    // Retain full 12-bit resolution (0-4095) for the thumbwheel
                    ADC_Thumb = ADCMEM0;
                    ADCMCTL0 &= ~ADCINCH_5;
                    ADCMCTL0 |= ADCINCH_2;
                    ADC_Channel = 0;
                    break;

                default: break;
            }

            ADCCTL0 |= ADCENC;                          // Re-enable Conversions
            ADCCTL0 |= ADCSC;                           // Start next sample
            break;

        default: break;
    }
}
