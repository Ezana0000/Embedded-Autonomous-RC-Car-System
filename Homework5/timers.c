//------------------------------------------------------------------------------
//  Description: Timer Initializations and ISRs - Homework 05
//  Ezana Enquobahrie
//------------------------------------------------------------------------------
#include  "msp430.h"
#include  "macros.h"

volatile unsigned int Time_Sequence;
volatile unsigned int update_display_count;
volatile char one_time;

void Init_Timers(void){
  Time_Sequence = 0;
  update_display_count = 0;
  one_time = 0;

  // THOMAS GILBERT TIMER LOGIC
  TB0CTL  = TBSSEL__SMCLK;   // Clock source = SMCLK (500 kHz)
  TB0CTL |= MC__UP;          // Up mode
  TB0CTL |= TBCLR;           // Clear the counter

  TB0CCR0 = TB0CCR0_INTERVAL; // 2500 for 5ms
  TB0CCTL0 |= CCIE;          // Enable interrupt
}

#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
    Time_Sequence++;
    one_time = 1;

    update_display_count++;
    if(update_display_count >= 40){
        update_display_count = 0;
        extern volatile unsigned char update_display;
        update_display = 1;
    }
}
