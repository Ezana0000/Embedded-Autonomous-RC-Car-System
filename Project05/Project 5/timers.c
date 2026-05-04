//------------------------------------------------------------------------------
//
//  Description: This file is where I configure my hardware timers.
//               I am using Timer B0 to generate a 5ms interrupt, which
//               I use as a heartbeat for the Project 5 movement lengths.
//
//  Ezana
//  Feb 2026
//  Built with Code Composer Studio
//------------------------------------------------------------------------------

#include "msp430.h"
#include "functions.h"
#include "macros.h"
#include "ports.h"

// Pulling in variables that the ISR needs to manipulate
extern volatile unsigned int Time_Sequence;
extern volatile char one_time;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;

extern volatile unsigned int p5_timer;
extern volatile unsigned int p5_running;

volatile unsigned int Time_Sequence = 0;
volatile char one_time = 0;

void Init_Timers(void){
    Init_Timer_B0();
}

//------------------------------------------------------------------------------
// Function: Init_Timer_B0
// Description: I set up Timer_B0 CCR0 so it trips every 5 milliseconds.
//              Since my SMCLK is 8 MHz, 40,000 counts equals 5ms.
//              I will count to 200 in my ISR to track 1 full second.
//------------------------------------------------------------------------------
void Init_Timer_B0(void){
    TB0CTL  = TBSSEL__SMCLK;    // Tell it to use my 8 MHz SMCLK
    TB0CTL |= MC__UP;           // Run it in Up mode
    TB0CTL |= TBCLR;            // Wipe out any existing counts in the register

    TB0CCR0 = TB0CCR0_INTERVAL; // Load my 40000 limit

    TB0CCTL0 |= CCIE;          // Most importantly, enable the interrupt for this timer
}

//------------------------------------------------------------------------------
// ISR: Timer0_B0_ISR
// Description: This is the actual code that runs every 5ms when the hardware
//              timer hits 40,000. It increments my movement timers and LCD flags.
//------------------------------------------------------------------------------
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){

    Time_Sequence++;

    // I keep this old legacy block intact just in case I need to run a 50-tick sequence
    if (Time_Sequence % 50 == 0){
        one_time = 1;
    }

    // Here is where I trigger my LCD screen to refresh
    // 40 ticks * 5ms = 200ms per screen refresh
    update_display_count++;
    if(update_display_count >= 40){
        update_display_count = 0;
        update_display = 1;
    }

    // Here is where I actually track the time for Project 5
    if(p5_running){
        p5_timer++;                     // Adds 1 every 5ms
    }

    TB0CCTL0 &= ~CCIFG;                // I must manually clear the flag so it can interrupt again
}
