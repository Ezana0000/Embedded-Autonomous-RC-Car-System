//===========================================================================
// File Name : interrupts_timers.c
// Description: Single Timer B0 ISR using the Hint's Counting Method.
//===========================================================================

#include "msp430.h"
#include "macros.h"
#include "ports.h"

#define FIFTY_MS_COUNT (4) // 4 * 50ms = 200ms (2.5 blinks per second)

extern volatile unsigned char update_display;
extern volatile unsigned int p5_timer;
extern volatile unsigned int p5_running;

extern volatile unsigned int sw1_debounce_count;
extern volatile unsigned int sw2_debounce_count;
extern volatile char sw1_debounce_active;
extern volatile char sw2_debounce_active;

extern volatile unsigned int Time_Sequence; // From Hints
volatile char one_time;                     // From Hints
unsigned int blink_count = 0;

#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
    //--------------------------------------------------------------------
    // TimerB0 0 Interrupt handler (Fires every 50ms)
    //--------------------------------------------------------------------

    // 1. LCD Backlight and Display Update (Every 200ms)
    // Only blink if a switch debounce is NOT actively freezing the screen
    if(!sw1_debounce_active && !sw2_debounce_active){
        blink_count++;
        if(blink_count >= FIFTY_MS_COUNT){
            blink_count = 0;
            //P6OUT ^= LCD_BACKLITE;       // Flip State of LCD_BACKLITE
            update_display = 1;          // Flag LCD refresh safely

            if(p5_running){
                p5_timer++;
            }
        }
    }

    // 2. Main Loop Time Sequence (From Hints)
    one_time = 1;
    if(Time_Sequence++ > 250){
        Time_Sequence = 0;
    }

    // 3. Switch 1 Debounce Counting
    if (sw1_debounce_active) {
        sw1_debounce_count++;
        if(sw1_debounce_count >= DEBOUNCE_THRESHOLD){
            sw1_debounce_active = 0;     // Done debouncing
            P4IFG &= ~SW1;               // Clear physical bounces
            P4IE |= SW1;                 // Re-enable SW1
            blink_count = 0;             // Reset blinker sync
        }
    }

    // 4. Switch 2 Debounce Counting
    if (sw2_debounce_active) {
        sw2_debounce_count++;
        if(sw2_debounce_count >= DEBOUNCE_THRESHOLD){
            sw2_debounce_active = 0;     // Done debouncing
            P2IFG &= ~SW2;               // Clear physical bounces
            P2IE |= SW2;                 // Re-enable SW2
            blink_count = 0;             // Reset blinker sync
        }
    }

    TB0CCR0 += TB0CCR0_INTERVAL;         // Set next 50ms alarm
}
