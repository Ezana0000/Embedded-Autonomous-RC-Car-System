//------------------------------------------------------------------------------
//
//  Description: This file contains the Timer Initializations and ISRs
//
//  Jim Carlson
//  Jan 2023
//  Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------

#include  "msp430.h"
#include  "macros.h"

// Global Variables
volatile unsigned int Time_Sequence;
volatile unsigned int update_display_count;
volatile char one_time;

//------------------------------------------------------------------------------
//  Function Name: Init_Timers
//
//  Description:
//  Initializes Timer B0 for 5ms interrupts (Fast PWM).
//------------------------------------------------------------------------------
void Init_Timers(void){
  Time_Sequence = 0;
  update_display_count = 0;
  one_time = 0;

  // SMCLK = 8MHz. Divided by 8 = 1MHz (1us per tick).
  // 5000 ticks = 5ms.
  TB0CTL = TBSSEL__SMCLK | MC__UP | ID__8; // SMCLK, Up Mode, /8
  TB0CCR0 = 5000;          // 5ms Interrupt (Was 50000/50ms)
  TB0CCTL0 = CCIE;         // Enable CCR0 interrupt
  TB0EX0 = TBIDEX__1;      // Expansion divider
  TB0CTL &= ~TBIFG;        // Clear overflow interrupt flag
}

//------------------------------------------------------------------------------
//  Function Name: Timer0_B0_ISR
//
//  Description:
//  Interrupt Service Routine for Timer B0.
//------------------------------------------------------------------------------
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
    Time_Sequence++; // Increments every 5ms
    one_time = 1;

    // LCD Update Loop (Run slower than motors)
    // Update every ~200ms (40 * 5ms)
    update_display_count++;
    if(update_display_count > 40){
        update_display_count = 0;
        extern volatile unsigned char update_display;
        update_display = 1;
    }
}
