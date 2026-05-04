//===========================================================================
// File Name : interrupt_ports.c
//===========================================================================
#include "msp430.h"
#include <string.h>
#include "macros.h"
#include "ports.h"

extern char display_line[4][11];
extern volatile unsigned char display_changed;

volatile unsigned int sw1_debounce_count = 0;
volatile unsigned int sw2_debounce_count = 0;
volatile char sw1_debounce_active = 0;
volatile char sw2_debounce_active = 0;

// Project 8 Flags
extern int hw8_mode;
extern int hw8_action;

// SW1 Interrupt
#pragma vector = PORT4_VECTOR
__interrupt void switchP4_interrupt(void){
    if (P4IFG & SW1) {
        P4IFG &= ~SW1;
        P4IE &= ~SW1;

        sw1_debounce_active = 1;
        sw1_debounce_count = 0;

        if (hw8_mode) {
            hw8_action = 1; // Trigger the Transmit sequence
        }
    }
}

// SW2 Interrupt
#pragma vector = PORT2_VECTOR
__interrupt void switchP2_interrupt(void){
    if (P2IFG & SW2) {
        P2IFG &= ~SW2;
        P2IE &= ~SW2;

        sw2_debounce_active = 1;
        sw2_debounce_count = 0;

        if (hw8_mode) {
            hw8_action = 2; // Trigger the Baud Swap sequence
        }
    }
}
