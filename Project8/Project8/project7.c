//===========================================================================
// File Name : project7.c
// Description: Line Following Logic with Memory & Dynamic Thresholds
// Author: Ezana
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

// Dynamic thresholds calculated during calibration mode
extern unsigned int Left_Black_To_Gray;
extern unsigned int Right_Black_To_Gray;
extern unsigned int Left_Gray_To_White;
extern unsigned int Right_Gray_To_White;

// Memory variable: 1 = Left sensor saw it last, 2 = Right sensor saw it last
unsigned int last_line_pos = 1;

void Run_Project7(void){
    switch(p5_step){

    case P7_WAIT:
        if(step_init){
            step_init = 0;
            strcpy(display_line[0], " Waiting  ");
            strcpy(display_line[1], "          ");
            strcpy(display_line[2], "          ");
            display_changed = 1;
            Wheels_All_Off();
            p5_timer = 0;
            p5_running = 1;
            P2OUT |= IR_LED;
        }
        if(p5_timer >= ONE_SEC){
            p5_step = P7_INTERCEPTING;
            step_init = 1;
        }
        break;

    case P7_INTERCEPTING:
        if(step_init){
            step_init = 0;
            strcpy(display_line[0], "Interceptg");
            display_changed = 1;
            Forward_On();
        }
        // Use the new dynamic gray threshold to stop!
        if(ADC_Left > Left_Black_To_Gray || ADC_Right > Right_Black_To_Gray){
            Wheels_All_Off();
            p5_step = P7_TURNING;
            step_init = 1;
            p5_timer = 0;
        }
        break;

    case P7_TURNING:
        if(step_init){
            step_init = 0;
            strcpy(display_line[0], " Turning  ");
            display_changed = 1;
        }

        if ((Time_Sequence % 2) == 0) Spin_CCW_On(); else Wheels_All_Off();

        if (p5_timer >= TURN_BLIND_TIME) {
            if(ADC_Left > Left_Black_To_Gray){
                Wheels_All_Off();
                p5_step = P7_CIRCLING;
                step_init = 1;
                p5_timer = 0;
            }
        }
        break;

    case P7_CIRCLING:
        if(step_init){
            step_init = 0;
            strcpy(display_line[0], " Circling ");
            display_changed = 1;
            RIGHT_REVERSE_SPEED = WHEEL_OFF;
            LEFT_REVERSE_SPEED = WHEEL_OFF;
        }



        if (ADC_Left > Left_Black_To_Gray && ADC_Right > Right_Black_To_Gray) {
            // Perfect: Both on solid black. Carve a natural left arc
            RIGHT_FORWARD_SPEED = ARC_FAST;
            LEFT_FORWARD_SPEED  = ARC_SLOW;
        }
        else if (ADC_Left < Left_Black_To_Gray && ADC_Right >= Right_Black_To_Gray) {
            // Left is White, Right is Black. Drifting Left (Inside). STEER RIGHT
            last_line_pos = 2; // Remember the line is on the right

            if (ADC_Left > Left_Gray_To_White) {
                RIGHT_FORWARD_SPEED = GENTLE_SLOW;
                LEFT_FORWARD_SPEED  = GENTLE_FAST;
            } else {
                RIGHT_FORWARD_SPEED = HARD_SLOW;
                LEFT_FORWARD_SPEED  = HARD_FAST;
            }
        }
        else if (ADC_Right < Right_Black_To_Gray && ADC_Left >= Left_Black_To_Gray) {
            // Right is White, Left is Black. Drifting Right (Outside). STEER LEFT
            last_line_pos = 1; // Remember the line is on the left

            if (ADC_Right > Right_Gray_To_White) {
                RIGHT_FORWARD_SPEED = GENTLE_FAST;
                LEFT_FORWARD_SPEED  = GENTLE_SLOW;
            } else {
                RIGHT_FORWARD_SPEED = HARD_FAST;
                LEFT_FORWARD_SPEED  = HARD_SLOW;
            }
        }
        else {
            //  (Both White)
            if (last_line_pos == 1) {
                // Fell off the outside. Carve Left
                RIGHT_FORWARD_SPEED = HARD_FAST;
                LEFT_FORWARD_SPEED  = HARD_SLOW;
            } else {
                // Fell off the inside. Carve Right
                RIGHT_FORWARD_SPEED = HARD_SLOW;
                LEFT_FORWARD_SPEED  = HARD_FAST;
            }
        }


        if(p5_timer >= TWO_CIRCLES_TIME){
            Wheels_All_Off();
            p5_step = P7_CENTERING;
            step_init = 1;
            p5_timer = 0;
        }
        break;

    case P7_CENTERING:
        if(step_init){
            step_init = 0;
            strcpy(display_line[0], " Turning In");
            display_changed = 1;
        }

        // Pulse CCW to physically turn the car to face the inside of the circle
        if ((Time_Sequence % 2) == 0) Spin_CCW_On(); else Wheels_All_Off();

        // Wait ~0.6 seconds for the car to face the center
        if(p5_timer >= 3){
            p5_step = P7_STOPPED;
            step_init = 1;
            p5_timer = 0;
        }
        break;

    case P7_STOPPED:
        if(step_init){
            step_init = 0;
            strcpy(display_line[0], " Stopped  ");
            display_changed = 1;
            Forward_On(); // Drive into the middle!
        }

        // Drive forward for 1 second to reach the center, then freeze forever
        if(p5_timer >= ONE_SEC){
            Wheels_All_Off();
            p5_running = 0; // Freeze the LCD timer!
        }
        break;

    default:
        Wheels_All_Off();
        break;
    }
}
