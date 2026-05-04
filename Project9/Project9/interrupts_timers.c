//===========================================================================
// File Name : interrupts_timers.c
// Description: Timer B0 ISRs with user's software debounce in B1 cases.
//===========================================================================

#include "msp430.h"
#include "macros.h"
#include "ports.h"

extern volatile unsigned char update_display;
extern volatile unsigned int p5_timer;
extern volatile unsigned int p5_running;

// --- Your original debounce variables ---
extern volatile unsigned int sw1_debounce_count;
extern volatile unsigned int sw2_debounce_count;
extern volatile char sw1_debounce_active;
extern volatile char sw2_debounce_active;

extern volatile unsigned int Time_Sequence;
volatile char one_time;
unsigned int display_count = 0;

extern unsigned int DAC_data;

#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
    //--------------------------------------------------------------------
    // TimerB0 0 Interrupt handler for 50msec timer
    //--------------------------------------------------------------------

    Time_Sequence++;
    one_time = 1;

    // Add counter to count 200msec worth of interrupts then set update_display = 1
    display_count++;
    if(display_count >= 4){              // 4 ticks * 50ms = 200ms
        display_count = 0;
        update_display = 1;

        if(p5_running){
            p5_timer++;
        }
    }

    TB0CCR0 += TB0CCR0_INTERVAL;         // Add Offset to TBCCR0
    //--------------------------------------------------------------------
}

#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void){
    //----------------------------------------------------------------------------
    // TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
    //----------------------------------------------------------------------------
    switch(__even_in_range(TB0IV,14)){
        case 0: break;                   // No interrupt

        case 2:                          // CCR1 Used for SW1 Debounce
            // Your custom SW1 debounce logic
            if (sw1_debounce_active) {
                sw1_debounce_count++;
                if(sw1_debounce_count >= DEBOUNCE_THRESHOLD){
                    sw1_debounce_active = 0;     // Done debouncing
                    P4IFG &= ~SW1;               // Clear physical bounces
                    P4IE |= SW1;                 // Re-enable SW1
                }
            }
            TB0CCR1 += TB0CCR1_INTERVAL; // Set next alarm for CCR1
            break;

        case 4:                          // CCR2 Used for SW2 Debounce
            // Your custom SW2 debounce logic
            if (sw2_debounce_active) {
                sw2_debounce_count++;
                if(sw2_debounce_count >= DEBOUNCE_THRESHOLD){
                    sw2_debounce_active = 0;     // Done debouncing
                    P2IFG &= ~SW2;               // Clear physical bounces
                    P2IE |= SW2;                 // Re-enable SW2
                }
            }
            TB0CCR2 += TB0CCR2_INTERVAL; // Set next alarm for CCR2
            break;

        case 6:  break; // reserved
        case 8:  break; // reserved
        case 10: break; // reserved
        case 12: break; // reserved

        case 14:                         // overflow available for greater than 1 second timer
            // DAC Voltage Ramping
            P2OUT |= DAC_ENB;
            if(DAC_data > 1190){
                DAC_data -= 50;
                SAC3DAT = DAC_data;
            } else {
                TB0CTL &= ~TBIE;         // Target reached: Turn off overflow
            }
            break;

        default: break;
    }
    //----------------------------------------------------------------------------
}
