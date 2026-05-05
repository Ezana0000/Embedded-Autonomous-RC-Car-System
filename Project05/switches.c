//------------------------------------------------------------------------------
//
//  Description: This file handles my switch processing, including debouncing
//               and tying SW1 to start the sequence, and SW2 to an E-Stop.
//
//  Ezana
//  Feb 2026
//  Built with Code Composer Studio
//------------------------------------------------------------------------------

#include "msp430.h"
#include "functions.h"
#include "macros.h"
#include "ports.h"
#include <string.h>

extern char display_line[4][11];
extern volatile unsigned char display_changed;

// These are my state variables from main.c so I can change them here
extern unsigned int p5_step;
extern char p5_started;
extern char step_init;
extern volatile unsigned int p5_timer;
extern volatile unsigned int p5_running;

// I use these variables to count loops and ignore bouncy switch presses
static unsigned int sw1_debounce = 0;
static unsigned int sw2_debounce = 0;
#define DEBOUNCE_THRESHOLD  (500)

void Switches_Process(void) {

    //--------------------------------------------------------------------------
    // Checking SW1 - If pressed, it starts my Project 5 sequence
    //--------------------------------------------------------------------------
    if (sw1_debounce > 0) {
        sw1_debounce--;
    } else {
        if (!(P4IN & SW1)) {                    // Is the pin pulled low?

            sw1_debounce = DEBOUNCE_THRESHOLD;  // Reset my delay counter

            // I only want to initiate the sequence if it isn't already running
            if (!p5_started) {
                p5_step = P5_FWD1;              // Tell the state machine to hit step 1
                p5_started = 1;                 // Let main() know we are live
                step_init = 1;                  // Flag for initial execution
                p5_timer = 0;                   // Clear the timer
                p5_running = 0;                 // This gets enabled in Run_Project5
            }
        }
    }

    //--------------------------------------------------------------------------
    // Checking SW2 - This is my emergency stop button
    //--------------------------------------------------------------------------
    if (sw2_debounce > 0) {
        sw2_debounce--;
    } else {
        if (!(P2IN & SW2)) {                    // Is SW2 pulled low?

            sw2_debounce = DEBOUNCE_THRESHOLD;

            // Absolute safety: I am forcing every single motor pin to ground
            P6OUT &= ~R_FORWARD;
            P6OUT &= ~L_FORWARD;
            P6OUT &= ~R_REVERSE;
            P6OUT &= ~L_REVERSE;

            // I'm completely resetting my state machine back to zero
            p5_started = 0;
            p5_running = 0;
            p5_step = 0;

            // Give myself a visual indicator that the E-Stop worked
            strcpy(display_line[0], "  STOPPED ");
            strcpy(display_line[1], "          ");
            strcpy(display_line[2], " Press SW1");
            strcpy(display_line[3], " to start ");
            display_changed = TRUE;
        }
    }
}
