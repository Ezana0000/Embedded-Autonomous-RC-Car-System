//===========================================================================
// File Name : interrupts_timers.c
// Description: Timer B0 interrupt service routines for system timing.
//===========================================================================

#include "msp430.h"
#include "macros.h"
#include "ports.h"

extern volatile unsigned char update_display;
extern volatile unsigned int p5_timer;
extern volatile unsigned int p5_running;

extern volatile unsigned int sw1_debounce_count;
extern volatile unsigned int sw2_debounce_count;
extern volatile char sw1_debounce_active;
extern volatile char sw2_debounce_active;

volatile unsigned int backlight = 0; // Global backlight flag

//===========================================================================
// Function Name: Timer0_B0_ISR
// Description: Handles backlight blinking and Project 5 timing.
// Interrupt Source: Timer B0 CCR0
// Trigger: 200ms Interval (25000 ticks)
// Globals Changed: update_display, p5_timer, backlight
// Locals Used: None
//===========================================================================
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){

    update_display = 1;          // Flag LCD refresh

    // Exact backlight toggling logic from old reference code
    if(backlight){
        P6OUT &= ~LCD_BACKLITE;
        backlight = 0;
    }
    else {
        P6OUT |= LCD_BACKLITE;
        backlight = 1;
    }

    if(p5_running){
        p5_timer++;              // Increment Project 5 timer
    }

    TB0CCR0 += TB0CCR0_INTERVAL; // Add interval for continuous mode
}

//===========================================================================
// Function Name: Timer0_B1_ISR
// Description: Handles debouncing for SW1 and SW2.
// Interrupt Source: Timer B0 CCR1 and CCR2
// Trigger: 50ms Interval (6250 ticks)
// Globals Changed: sw1_debounce_count, sw2_debounce_count,
//                  sw1_debounce_active, sw2_debounce_active
// Locals Used: None
//===========================================================================
#pragma vector = TIMER0_B1_VECTOR
__interrupt void Timer0_B1_ISR(void){
    switch(__even_in_range(TB0IV, 14)){
        case 0x00: break;

        case 0x02:               // CCR1 (SW1 Debounce)
            if (sw1_debounce_active) {
                sw1_debounce_count++;
                if(sw1_debounce_count >= DEBOUNCE_THRESHOLD){
                    sw1_debounce_active = 0;
                    TB0CCTL1 &= ~CCIE;           // Stop debounce timer
                    P4IFG &= ~SW1;               // Clear pending bounces
                    P4IE |= SW1;                 // Re-enable SW1 interrupt

                    TB0CCR0 = TB0R + TB0CCR0_INTERVAL; // Sync timer
                    TB0CCTL0 |= CCIE;            // Resume blinking
                }
            }
            TB0CCR1 += TB0CCR1_INTERVAL;
            break;

        case 0x04:               // CCR2 (SW2 Debounce)
            if (sw2_debounce_active) {
                sw2_debounce_count++;
                if(sw2_debounce_count >= DEBOUNCE_THRESHOLD){
                    sw2_debounce_active = 0;
                    TB0CCTL2 &= ~CCIE;           // Stop debounce timer
                    P2IFG &= ~SW2;               // Clear pending bounces
                    P2IE |= SW2;                 // Re-enable SW2 interrupt

                    TB0CCR0 = TB0R + TB0CCR0_INTERVAL; // Sync timer
                    TB0CCTL0 |= CCIE;            // Resume blinking
                }
            }
            TB0CCR2 += TB0CCR2_INTERVAL;
            break;

        default: break;
    }
}
