//===========================================================================
// File Name : main.c
// Description: Main Routine - Background/Foreground Operating System.
// Author: Ahan
//===========================================================================

#include "msp430.h"
#include <string.h>
#include "functions.h"
#include "LCD.h"
#include "ports.h"
#include "macros.h"

extern char display_line[4][11];
extern volatile unsigned char display_changed;
volatile unsigned int backlight = 0;
volatile unsigned int p5_timer = 0;
volatile unsigned int p5_running = 0;
unsigned int p5_step = 0;
char p5_started = 0;
char step_init = 1;
volatile unsigned int Time_Sequence = 0;

void main(void){
    // ALL VARIABLES DECLARED AT THE VERY TOP
    char thumb_str[5];
    char left_str[5];
    char right_str[5];
    extern volatile unsigned int ADC_Thumb;
    extern volatile unsigned int ADC_Left;
    extern volatile unsigned int ADC_Right;

    PM5CTL0 &= ~LOCKLPM5;                   // Disable high-impedance mode

    Init_Ports();
    Init_Clocks();
    Init_Timers();
    Init_LCD();

    Init_Conditions();                      // Must be placed here safely

    Init_ADC();
    ADCCTL0 |= ADCENC | ADCSC;              // Jump-start the first ADC conversion

    strcpy(display_line[0], " Project6 ");
    strcpy(display_line[1], "Calibratng");
    strcpy(display_line[2], " Press SW1");
    strcpy(display_line[3], " to Start ");
    display_changed = TRUE;

    Wheels_All_Off();
    P6OUT |= LCD_BACKLITE;                  // Ensure backlight stays on
    P2OUT |= IR_LED;                        // Turn Emitter on for calibration

    while(ALWAYS) {
        if(p5_started){
            Run_Project6();
        } else {
            // Live Calibration Mode (Runs when car is sitting still)
            HEXtoBCD(ADC_Thumb, thumb_str);
            HEXtoBCD(ADC_Left, left_str);
            HEXtoBCD(ADC_Right, right_str);

            // Line 1: Thumbwheel
            strcpy(display_line[1], "Thb:      ");
            display_line[1][5] = thumb_str[0]; display_line[1][6] = thumb_str[1];
            display_line[1][7] = thumb_str[2]; display_line[1][8] = thumb_str[3];

            // Line 2: Left Detector
            strcpy(display_line[2], "L:        ");
            display_line[2][3] = left_str[0]; display_line[2][4] = left_str[1];
            display_line[2][5] = left_str[2]; display_line[2][6] = left_str[3];

            // Line 3: Right Detector
            strcpy(display_line[3], "R:        ");
            display_line[3][3] = right_str[0]; display_line[3][4] = right_str[1];
            display_line[3][5] = right_str[2]; display_line[3][6] = right_str[3];

            display_changed = 1;
        }
        Display_Process();
        P3OUT ^= TEST_PROBE;
    }
}
