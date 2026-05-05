//------------------------------------------------------------------------------
// Description: Timer B0 Initialization and Interrupt Service Routines
// Ahan
//------------------------------------------------------------------------------

#include "msp430.h"
#include "functions.h"
#include "macros.h"
#include "ports.h"

// Globals accessed by other files
extern volatile unsigned char update_display;
extern volatile unsigned int p5_timer;
extern volatile unsigned int p5_running;

volatile unsigned int sw1_debounce_count = 0;
volatile unsigned int sw2_debounce_count = 0;

void Init_Timers(void){
    Init_Timer_B0();
}

//------------------------------------------------------------------------------
// Function: Init_Timer_B0
// Description: Configures Timer B0 for 125,000 Hz using Continuous Mode.
//------------------------------------------------------------------------------
void Init_Timer_B0(void){
    TB0CTL  = TBSSEL__SMCLK;    // Source from 8MHz SMCLK
    TB0CTL |= MC__CONTINUOUS;   // Run in Continuous mode
    TB0CTL |= ID__8;            // Divide clock by 8
    TB0EX0  = TBIDEX__8;        // Divide clock by another 8
    TB0CTL |= TBCLR;            // Clear timer

    // CCR0: Backlight & Display Update (200ms)
    TB0CCR0 = TB0CCR0_INTERVAL;
    TB0CCTL0 |= CCIE;

    // CCR1: SW1 Debounce (50ms - Starts Disabled)
    TB0CCR1 = TB0CCR1_INTERVAL;
    TB0CCTL1 &= ~CCIE;

    // CCR2: SW2 Debounce (50ms - Starts Disabled)
    TB0CCR2 = TB0CCR2_INTERVAL;
    TB0CCTL2 &= ~CCIE;

    TB0CTL &= ~TBIE;            // Disable overflow interrupt
    TB0CTL &= ~TBIFG;           // Clear overflow flag
}

//------------------------------------------------------------------------------
// ISR: Timer0_B0_ISR (Triggered by CCR0 every 200ms)
//------------------------------------------------------------------------------
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){

    P6OUT ^= LCD_BACKLITE;       // Toggle backlight state
    update_display = 1;          // Flag to refresh LCD

    if(p5_running){
        p5_timer++;              // Increment Project 5 timer
    }

    TB0CCR0 += TB0CCR0_INTERVAL; // Add interval for continuous mode
}

//------------------------------------------------------------------------------
// ISR: Timer0_B1_ISR (Triggered by CCR1 or CCR2 every 50ms)
//------------------------------------------------------------------------------
#pragma vector = TIMER0_B1_VECTOR
__interrupt void Timer0_B1_ISR(void){
    switch(__even_in_range(TB0IV, 14)){
        case 0x00: break; // No interrupt

        case 0x02: // CCR1 (SW1 Debounce)
            sw1_debounce_count++;
            if(sw1_debounce_count >= DEBOUNCE_THRESHOLD){
                TB0CCTL1 &= ~CCIE;  // Disable debounce timer
                P4IFG &= ~SW1;      // Clear pending bounces
                P4IE |= SW1;        // Re-enable SW1 interrupt
                TB0CCTL0 |= CCIE;   // Re-enable backlight blinking
            }
            TB0CCR1 += TB0CCR1_INTERVAL;
            break;

        case 0x04: // CCR2 (SW2 Debounce)
            sw2_debounce_count++;
            if(sw2_debounce_count >= DEBOUNCE_THRESHOLD){
                TB0CCTL2 &= ~CCIE;  // Disable debounce timer
                P2IFG &= ~SW2;      // Clear pending bounces
                P2IE |= SW2;        // Re-enable SW2 interrupt
                TB0CCTL0 |= CCIE;   // Re-enable backlight blinking
            }
            TB0CCR2 += TB0CCR2_INTERVAL;
            break;

        default: break;
    }
}
