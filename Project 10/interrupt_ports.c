//------------------------------------------------------------------------------
//
//  Description: Hardware switch interrupts for Port 2 and Port 4.
//  Handles the debouncing and calibration logic for the black and white sensors.
//
//  Ezana
//  April 2026
//  Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------

#include "msp430.h"
#include <string.h>
#include "functions.h"
#include "ports.h"
#include "macros.h"

extern char display_line[4][11];
extern volatile unsigned char display_changed;

extern volatile unsigned int ADC_Left;
extern volatile unsigned int ADC_Right;

extern volatile char calibration_done;

// Threshold variables used by the navigation state machine
extern unsigned int Left_Black_To_Gray;
extern unsigned int Right_Black_To_Gray;
extern unsigned int Left_Gray_To_White;
extern unsigned int Right_Gray_To_White;

// Baseline calibration values
unsigned int black_val_L = 0;
unsigned int black_val_R = 0;
unsigned int white_val_L = 0;
unsigned int white_val_R = 0;

// Debounce control variables
volatile unsigned int sw1_debounce_count = 0;
volatile unsigned int sw2_debounce_count = 0;
volatile char sw1_debounce_active = 0;
volatile char sw2_debounce_active = 0;

//------------------------------------------------------------------------------
// SW1 Interrupt: Calibrate Black Line Baseline
//------------------------------------------------------------------------------
#pragma vector = PORT4_VECTOR
__interrupt void switchP4_interrupt(void){
    if (P4IFG & SW1) {
        P4IFG &= ~SW1; // Clear interrupt flag
        P4IE &= ~SW1;  // Disable SW1 to debounce

        sw1_debounce_active = 1;
        sw1_debounce_count = 0;

        // Capture current ADC values as the black baseline
        black_val_L = ADC_Left;
        black_val_R = ADC_Right;

        strcpy(display_line[0], "BLACK CAL ");
        strcpy(display_line[1], "          ");
        display_changed = 1;
    }
}

//------------------------------------------------------------------------------
// SW2 Interrupt: Calibrate White Board Baseline & Calculate Thresholds
//------------------------------------------------------------------------------
#pragma vector = PORT2_VECTOR
__interrupt void switchP2_interrupt(void){
    if (P2IFG & SW2) {
        P2IFG &= ~SW2; // Clear interrupt flag
        P2IE &= ~SW2;  // Disable SW2 to debounce

        sw2_debounce_active = 1;
        sw2_debounce_count = 0;

        // Capture current ADC values as the white baseline
        white_val_L = ADC_Left;
        white_val_R = ADC_Right;

        // Ensure black was calibrated first to avoid math errors
        if (black_val_L > white_val_L && black_val_R > white_val_R) {

            // Calculate 75% dark threshold for black detection
            Left_Black_To_Gray = white_val_L + ((black_val_L - white_val_L) * 3 / 4);
            Right_Black_To_Gray = white_val_R + ((black_val_R - white_val_R) * 3 / 4);

            // Calculate 12.5% dark threshold for line boundary recovery
            Left_Gray_To_White = white_val_L + ((black_val_L - white_val_L) / 8);
            Right_Gray_To_White = white_val_R + ((black_val_R - white_val_R) / 8);

            strcpy(display_line[0], "WHITE CAL ");
            strcpy(display_line[1], " SUCCESS! ");
            display_changed = 1;
            calibration_done = 1;
        }
        else {
            strcpy(display_line[0], "CAL ERROR!");
            strcpy(display_line[1], "DO BLK 1ST");
            display_changed = 1;
            calibration_done = 0;
        }
    }
}
