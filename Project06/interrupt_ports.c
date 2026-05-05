//===========================================================================
// File Name : interrupt_ports.c
// Description: Port interrupt service routines for switch actuation.
//===========================================================================

#include "msp430.h"
#include <string.h>
#include "macros.h"
#include "ports.h"

extern char display_line[4][11];
extern volatile unsigned char display_changed;

extern char p5_started;
extern unsigned int p5_step;
extern volatile unsigned int p5_timer;
extern volatile unsigned int p5_running;
extern char step_init;
void Wheels_All_Off(void);

volatile unsigned int sw1_debounce_count = 0;
volatile unsigned int sw2_debounce_count = 0;
volatile char sw1_debounce_active = 0;
volatile char sw2_debounce_active = 0;
extern unsigned int blink_count;

// SW1 Interrupt
#pragma vector = PORT4_VECTOR
__interrupt void switchP4_interrupt(void){
    if (P4IFG & SW1) {
        P4IFG &= ~SW1;
        P4IE &= ~SW1;                // Disable SW1

        sw1_debounce_active = 1;     // Flag debounce active
        sw1_debounce_count = 0;      // Start stopwatch at 0

        P6OUT &= ~LCD_BACKLITE;      // Turn off backlight physically

        strcpy(display_line[3], " Switch 1 ");
        display_changed = 1;

        if (!p5_started) {
            p5_step = P6_WAIT; // Changed from P5_FWD1
            p5_started = 1;
            step_init = 1;
            p5_timer = 0;
            p5_running = 0;
        }
    }
}

// SW2 Interrupt
#pragma vector = PORT2_VECTOR
__interrupt void switchP2_interrupt(void){
    if (P2IFG & SW2) {
        P2IFG &= ~SW2;
        P2IE &= ~SW2;                // Disable SW2

        sw2_debounce_active = 1;     // Flag debounce active
        sw2_debounce_count = 0;      // Start stopwatch at 0

        P6OUT &= ~LCD_BACKLITE;      // Turn off backlight physically

        strcpy(display_line[3], " Switch 2 ");
        display_changed = 1;

        Wheels_All_Off();
        p5_started = 0;
        p5_running = 0;
        p5_step = 0;
    }
}
