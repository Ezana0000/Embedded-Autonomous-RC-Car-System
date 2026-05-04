//===========================================================================
// File Name : main.c
// Description: Main Routine - Background/Foreground Operating System.
// Author: Ezana
//===========================================================================

#include "msp430.h"
#include <string.h>
#include "functions.h"
#include "LCD.h"
#include "ports.h"
#include "macros.h"

// --- SYSTEM VARIABLES ---
extern char display_line[4][11];
extern volatile unsigned char display_changed;
volatile unsigned int backlight = 0;
volatile unsigned int p5_timer = 0;
volatile unsigned int p5_running = 0;
unsigned int p5_step = 0;
char p5_started = 0;
char step_init = 1;
volatile unsigned int Time_Sequence = 0;

// --- CALIBRATION VARIABLES ---
unsigned int cal_step = 0; // Tracks our 3-step sequence

unsigned int Black_Left_Val = 0;
unsigned int Black_Right_Val = 0;

// Individual thresholds for the Left and Right sensors
unsigned int Left_Black_To_Gray = 0;
unsigned int Right_Black_To_Gray = 0;
unsigned int Left_Gray_To_White = 0;
unsigned int Right_Gray_To_White = 0;

void main(void){
    // RAM Garbage Fix: Initialize strings with blanks!
    char thumb_str[5] = "    ";
    char left_str[5]  = "    ";
    char right_str[5] = "    ";

    extern volatile unsigned int ADC_Thumb;
    extern volatile unsigned int ADC_Left;
    extern volatile unsigned int ADC_Right;

    PM5CTL0 &= ~LOCKLPM5; // Disable high-impedance mode

    Init_Ports();
    Init_Clocks();
    Init_Timers();
    Init_LCD();
    Init_Conditions();
    Init_ADC();
    Init_DAC();

    Wheels_All_Off();
    P6OUT |= LCD_BACKLITE;
    P2OUT |= IR_LED;

    while(ALWAYS) {
        if(p5_started){
            Run_Project7();

            // Project 7 Live Timer Display (Format: XXX.X)
            // Only update the timer if the car is actively running the course
            if (p5_step != P7_STOPPED && p5_step != P7_WAIT) {
                int total_deciseconds = p5_timer * 2; // Each tick is 200ms (0.2s)
                int seconds = total_deciseconds / 10;
                int decimal = total_deciseconds % 10;

                strcpy(display_line[3], "Tm:       ");
                display_line[3][4] = '0' + ((seconds / 100) % 10);
                display_line[3][5] = '0' + ((seconds / 10) % 10);
                display_line[3][6] = '0' + (seconds % 10);
                display_line[3][7] = '.';
                display_line[3][8] = '0' + decimal;
                display_changed = 1;
            }
        }
        else {
            // --- LIVE CALIBRATION MODE ---
            HEXtoBCD(ADC_Thumb, thumb_str);
            HEXtoBCD(ADC_Left, left_str);
            HEXtoBCD(ADC_Right, right_str);

            if (cal_step == 0) {
                strcpy(display_line[0], "Calibrate ");
                strcpy(display_line[1], "Black Line");
                strcpy(display_line[2], "Press SW1 ");
            }
            else if (cal_step == 1) {
                strcpy(display_line[0], "Calibrate ");
                strcpy(display_line[1], "White Area");
                strcpy(display_line[2], "Press SW2 ");
            }
            else if (cal_step == 2) {
                strcpy(display_line[0], "  Ready!  ");
                strcpy(display_line[1], "Press SW1 ");
                strcpy(display_line[2], "To Start  ");
            }

            // EXACTLY 10 CHARACTERS: "L" + 4 digits + "R" + 4 digits
            strcpy(display_line[3], "L    R    ");
            display_line[3][1] = left_str[0]; display_line[3][2] = left_str[1];
            display_line[3][3] = left_str[2]; display_line[3][4] = left_str[3];

            display_line[3][6] = right_str[0]; display_line[3][7] = right_str[1];
            display_line[3][8] = right_str[2]; display_line[3][9] = right_str[3];

            display_changed = 1;
        }
        Display_Process();
        P3OUT ^= TEST_PROBE;
    }
}
