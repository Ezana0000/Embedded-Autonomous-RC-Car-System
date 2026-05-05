//===========================================================================
// File Name : timers.c
// Description: Initialization of Timer B0 using ONLY CCR0 (From Hints)
//===========================================================================

#include "msp430.h"
#include "functions.h"
#include "macros.h"

void Init_Timers(void){
    Init_Timer_B0();
}

void Init_Timer_B0(void) {
    TB0CTL = RESET_REGISTER;    // Clear TB0 Control Register
    TB0EX0 = RESET_REGISTER;    // Clear TBIDEX Register

    TB0CTL |= TBSSEL__SMCLK;    // SMCLK source
    TB0CTL |= MC__CONTINUOUS;   // Continuous up to 0xFFFF and overflow
    TB0CTL |= ID__2;            // Divide clock by 2
    TB0EX0 |= TBIDEX__8;        // Divide clock by an additional 8
    TB0CTL |= TBCLR;            // Resets TB0R hardware flip-flops

    // Capture Compare 0 (Fires exactly every 50ms)
    TB0CCR0 = TB0CCR0_INTERVAL;
    TB0CCTL0 |= CCIE;           // CCR0 enable interrupt

    // Overflow
    TB0CTL &= ~TBIE;            // Disable Overflow Interrupt
    TB0CTL &= ~TBIFG;           // Clear Overflow Interrupt flag
}
