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
extern volatile unsigned int backlight;

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

//===========================================================================
// Function Name: switchP4_interrupt
// Description: Triggered when SW1 is pressed.
// Interrupt Source: Port 4 (SW1)
// Trigger: High-to-Low Edge
// Globals Changed: display_line, display_changed, p5_started,
//                  p5_step, step_init, p5_timer, p5_running,
//                  sw1_debounce_count, sw1_debounce_active, backlight
// Locals Used: None
//===========================================================================
#pragma vector = PORT4_VECTOR
__interrupt void switchP4_interrupt(void){
    if (P4IFG & SW1) {
        P4IFG &= ~SW1;

        P4IE &= ~SW1;                // Disable SW1 interrupt
        sw1_debounce_active = 1;     // Flag debounce active

        P6OUT &= ~LCD_BACKLITE;      // Turn off backlight physically
        backlight = 0;               // Update global flag
        TB0CCTL0 &= ~CCIE;           // Disable blinking timer

        strcpy(display_line[3], " Switch 1 ");
        display_changed = 1;

        // Start Project 5 Sequence
        if (!p5_started) {
            p5_step = P5_FWD1;
            p5_started = 1;
            step_init = 1;
            p5_timer = 0;
            p5_running = 0;
        }

        sw1_debounce_count = 0;
        TB0CCR1 = TB0R + TB0CCR1_INTERVAL;
        TB0CCTL1 |= CCIE;                  // Start debounce timer
    }
}

//===========================================================================
// Function Name: switchP2_interrupt
// Description: Triggered when SW2 is pressed.
// Interrupt Source: Port 2 (SW2)
// Trigger: High-to-Low Edge
// Globals Changed: display_line, display_changed, p5_started,
//                  p5_step, p5_running, sw2_debounce_count,
//                  sw2_debounce_active, backlight
// Locals Used: None
//===========================================================================
#pragma vector = PORT2_VECTOR
__interrupt void switchP2_interrupt(void){
    if (P2IFG & SW2) {
        P2IFG &= ~SW2;

        P2IE &= ~SW2;                // Disable SW2 interrupt
        sw2_debounce_activme = 1;

        P6OUT &= ~LCD_BACKLITE;      // Turn off backlight physically
        backlight = 0;               // Update global flag
        TB0CCTL0 &= ~CCIE;           // Disable blinking timer

        strcpy(display_line[3], " Switch 2 ");
        display_changed = 1;

        // Emergency Stop for Project 5
        Wheels_All_Off();
        p5_started = 0;
        p5_running = 0;
        p5_step = 0;

        sw2_debounce_count = 0;
        TB0CCR2 = TB0R + TB0CCR2_INTERVAL;
        TB0CCTL2 |= CCIE;                  // Start debounce timer
    }
}
