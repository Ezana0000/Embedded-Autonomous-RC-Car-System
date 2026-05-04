//===========================================================================
// File Name : project6.c
// Description: Ultra-Slow "Hunt and Center" Logic (12.5% Duty Cycle)
//===========================================================================
#include "msp430.h"
#include <string.h>
#include "functions.h"
#include "ports.h"
#include "macros.h"

extern char display_line[4][11];
extern volatile unsigned char display_changed;
extern volatile unsigned int p5_timer;
extern volatile unsigned int p5_running;
extern unsigned int p5_step;
extern char step_init;
extern char p5_started;
extern volatile unsigned int Time_Sequence;

extern volatile unsigned int ADC_Left;
extern volatile unsigned int ADC_Right;

char left_str[5];
char right_str[5];

void Run_Project6(void){
    switch(p5_step){

    case P6_WAIT:
        if(step_init){
            step_init = 0;
            strcpy(display_line[0], " Wait 1s  ");
            display_changed = 1;
            Wheels_All_Off();
            p5_timer = 0;
            p5_running = 1;
            P2OUT |= IR_LED; // Ensure Emitter is powered
        }
        if(p5_timer >= ONE_SEC){
            p5_step = P6_FORWARD;
            step_init = 1;
        }
        break;

    case P6_FORWARD:
        if(step_init){
            step_init = 0;
            strcpy(display_line[0], " Seeking  ");
            display_changed = 1;
            Forward_On();
        }
        // Stop when detecting the black line circle
        if(ADC_Left > BLACK_LINE_THRESH || ADC_Right > BLACK_LINE_THRESH){
            Wheels_All_Off();
            p5_step = P6_STOP_DETECTED;
            step_init = 1;
        }
        break;

    case P6_STOP_DETECTED:
        if(step_init){
            step_init = 0;
            // Display requirement: "Black Line" and "Detected"
            strcpy(display_line[0], "Black Line");
            strcpy(display_line[1], " Detected ");
            display_changed = 1;
            p5_timer = 0;
        }
        // Requirement: Stop for 3-5 seconds
        if(p5_timer >= FOUR_SEC){
            p5_step = P6_TURN_CROSS;
            step_init = 1;
        }
        break;

    // Step 4a: Turn CW ULTRA SLOWLY until the Left detector hits White space
    case P6_TURN_CROSS:
        if(step_init){
            step_init = 0;
            strcpy(display_line[0], "Crossing  ");
            display_changed = 1;
        }
        // ULTRA SLOW PULSE: 12.5% Duty Cycle
        if ((Time_Sequence % 8) == 0) Spin_CW_On(); else Wheels_All_Off();

        // Target: White space (Your white value is ~2333)
        if(ADC_Left < (BLACK_LINE_THRESH - 300)){
            p5_step = P6_RECENTER;
            step_init = 1;
        }
        break;

    // Step 4b: Turn CCW ULTRA SLOWLY until the Left detector centers back on Black
    case P6_RECENTER:
        if(step_init){
            step_init = 0;
            strcpy(display_line[0], "Centering ");
            display_changed = 1;
        }
        // Slow pulse in the opposite direction
        if ((Time_Sequence % 8) == 0) Spin_CCW_On(); else Wheels_All_Off();

        // Target: Back to Black (Your black value is ~3700)
        if(ADC_Left > BLACK_LINE_THRESH){
            Wheels_All_Off();
            p5_step = P6_DONE;
            step_init = 1;
        }
        break;

    case P6_DONE:
        Wheels_All_Off();
        p5_running = 0;

        HEXtoBCD(ADC_Left, left_str);
        HEXtoBCD(ADC_Right, right_str);

        // Final Requirement: Show the values while over the black line
        strcpy(display_line[0], " Complete ");
        strcpy(display_line[1], "L:        ");
        strcpy(display_line[2], "R:        ");

        display_line[1][3]=left_str[0]; display_line[1][4]=left_str[1];
        display_line[1][5]=left_str[2]; display_line[1][6]=left_str[3];
        display_line[2][3]=right_str[0]; display_line[2][4]=right_str[1];
        display_line[2][5]=right_str[2]; display_line[2][6]=right_str[3];

        display_changed = 1;
        p5_started = 0;
        break;

    default:
        Wheels_All_Off();
        break;
    }
}
