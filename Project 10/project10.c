//------------------------------------------------------------------------------
//
//  Description: Core logic for Project 10 (Black Line Following & Course Navigation).
//  Includes the state machine for intercepting, tracking, and exiting the circle.
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

#define TRVL_STR_R               10600
#define TRVL_STR_L               14400

#define TRVL_GENTLE_RIGHT_R_SLOW 3400
#define TRVL_GENTLE_RIGHT_L_FAST 20000
#define TRVL_HARD_RIGHT_R_SLOW   0
#define TRVL_HARD_RIGHT_L_FAST   25600

#define TRVL_GENTLE_LEFT_R_FAST  13800
#define TRVL_GENTLE_LEFT_L_SLOW  4000
#define TRVL_HARD_LEFT_R_FAST    18600
#define TRVL_HARD_LEFT_L_SLOW    0

// Adjusted lower straight speeds for safe intersection entry
#define CRCL_STR_R               12500
#define CRCL_STR_L               10000

// Inner wheel power maintained to prevent line waddling
#define CRCL_GENTLE_RIGHT_R_SLOW 4000
#define CRCL_GENTLE_RIGHT_L_FAST 18000
#define CRCL_HARD_RIGHT_R_SLOW   500
#define CRCL_HARD_RIGHT_L_FAST   24000

#define CRCL_GENTLE_LEFT_R_FAST  11000
#define CRCL_GENTLE_LEFT_L_SLOW  4000
#define CRCL_HARD_LEFT_R_FAST    15000
#define CRCL_HARD_LEFT_L_SLOW    100

extern char display_line[4][11];
extern volatile unsigned char display_changed;
extern volatile unsigned int run_time;
extern volatile unsigned int Time_Sequence;
extern volatile unsigned int ADC_Left;
extern volatile unsigned int ADC_Right;
extern unsigned int command_op;

extern unsigned int Left_Black_To_Gray;
extern unsigned int Right_Black_To_Gray;
extern unsigned int Left_Gray_To_White;
extern unsigned int Right_Gray_To_White;

extern unsigned int intercept_type;

unsigned int last_line_pos = 1;
unsigned int p10_state = 0;
char state_init = 1;

unsigned int black_confirm_count = 0;

void Run_Project10(void){

    // Declared top-scope to comply with compiler standards
    float initial_turn = 8.4;

    // Safety catch to prevent cyclic loop upon exit command
    if (command_op == 'E' && p10_state != 9 && p10_state != 92 && p10_state != 95 && p10_state != 10) {
        p10_state = 9;
        state_init = 1;
    }

    if (command_op == 'A' && p10_state == 0) {
        p10_state = 100;
    }

    switch(p10_state){

    //------------------------------------------------------------------------------
    // Sequence 1: Navigation to Pad 8
    //------------------------------------------------------------------------------
    case 100:
        if(state_init){ Forward_On(); run_time = 0; state_init = 0; }
        if(run_time >= 80){ p10_state = 101; state_init = 1; }
        break;

    case 101:
        if(state_init){ Spin_CW_On(); run_time = 0; state_init = 0; }
        if(run_time >= 9.6){ p10_state = 102; state_init = 1; }
        break;

    case 102:
        if(state_init){ Forward_On(); run_time = 0; state_init = 0; }
        if(run_time >= 90){ p10_state = 103; state_init = 1; }
        break;

    case 103:
        if(state_init){
            Wheels_All_Off();
            strcpy(display_line[0], "Arrived 08");
            display_changed = 1;
            p10_state = 0;
            command_op = 'W';
            state_init = 1;
        }
        break;

    //------------------------------------------------------------------------------
    // Sequence 2: Line Intercept and Filtering
    //------------------------------------------------------------------------------
    case 0:
        if(state_init){
            Forward_On();
            strcpy(display_line[0], " BL Start ");
            display_changed = 1;
            run_time = 0;
            state_init = 0;
        }
        if(run_time >= 1){ p10_state = 1; state_init = 1; }
        break;

    case 1:
        if(state_init){
            Spin_CW_On();
            strcpy(display_line[0], " BL Start ");
            display_changed = 1;
            run_time = 0;
            state_init = 0;
        }

        // Parse desired intercept angle logic
        if (intercept_type == 1) {
            initial_turn = 11.9;
        } else if (intercept_type == 2) {
            initial_turn = 6.4;
        }

        if(run_time >= initial_turn){ p10_state = 2; state_init = 1; }
        break;

    case 2:
        if(state_init){
            Forward_On();
            strcpy(display_line[0], " BL Start ");
            display_changed = 1;
            run_time = 0;
            state_init = 0;
        }
        if (run_time > 1) {
            if(ADC_Left < Left_Gray_To_White && ADC_Right < Right_Gray_To_White){
                p10_state = 3;
                state_init = 1;
            }
        }
        break;

    case 3:
        if(state_init){
            Forward_On();
            strcpy(display_line[0], " BL Start ");
            display_changed = 1;
            run_time = 0;
            black_confirm_count = 0;
            state_init = 0;
        }

        if (run_time > 2) {
            if(ADC_Left > Left_Black_To_Gray && ADC_Right > Right_Black_To_Gray){
                black_confirm_count++;
                if (black_confirm_count >= 1) {
                    p10_state = 35;
                    state_init = 1;
                }
            } else {
                black_confirm_count = 0;
            }
        }
        break;

    case 35:
        if(state_init){
            Wheels_All_Off();
            strcpy(display_line[0], "Intercept ");
            display_changed = 1;
            run_time = 0;
            state_init = 0;
        }
        if(run_time >= 200){ p10_state = 4; state_init = 1; }
        break;

    //------------------------------------------------------------------------------
    // Sequence 3: Intersection and Centering
    //------------------------------------------------------------------------------
    case 4:
        if(state_init){
            Forward_On();
            strcpy(display_line[0], "Intercept ");
            display_changed = 1;
            run_time = 0;
            state_init = 0;
        }
        if(run_time >= 1){
            Wheels_All_Off();
            p10_state = 5;
            state_init = 1;
        }
        break;

    case 5:
        if(state_init){
            Wheels_All_Off();
            strcpy(display_line[0], "Intercept ");
            display_changed = 1;
            run_time = 0;
            state_init = 0;
        }
        if(run_time >= 200){ p10_state = 6; state_init = 1; }
        break;

    case 6:
        if(state_init){
            strcpy(display_line[0], " BL Turn  ");
            display_changed = 1;
            run_time = 0;
            state_init = 0;
        }

        if ((Time_Sequence % 2) == 0) Spin_CCW_On(); else Wheels_All_Off();

        if (run_time >= 9) {
            if(ADC_Left > Left_Gray_To_White && ADC_Right > Right_Gray_To_White){
                Wheels_All_Off();
                p10_state = 65;
                state_init = 1;
            }
        }
        break;

    case 65: // Closed-loop micro adjustment processing
        if(state_init){
            run_time = 0;
            state_init = 0;
        }

        if (ADC_Left > Left_Black_To_Gray && ADC_Right > Right_Black_To_Gray) {
            Wheels_All_Off();
            p10_state = 7;
            state_init = 1;
        }
        else {
            if ((Time_Sequence % 4) < 1) {
                if (ADC_Left < Left_Black_To_Gray && ADC_Right >= Right_Black_To_Gray) {
                    Spin_CCW_On();
                } else if (ADC_Right < Right_Black_To_Gray && ADC_Left >= Left_Black_To_Gray) {
                    Spin_CW_On();
                } else {
                    Spin_CCW_On();
                }
            } else {
                Wheels_All_Off();
            }
        }
        break;

    case 7:
        if(state_init){
            run_time = 0;
            state_init = 0;
            Wheels_All_Off();
            strcpy(display_line[0], " BL Turn  ");
            display_changed = 1;
        }
        if(run_time >= 200){ p10_state = 8; state_init = 1; }
        break;

    //------------------------------------------------------------------------------
    // Sequence 4: S-Curve Tracking Logic
    //------------------------------------------------------------------------------
    case 8:
        if(state_init){
            strcpy(display_line[0], "BL Travel ");
            display_changed = 1;
            run_time = 0;
            state_init = 0;
        }

        if(run_time >= 215) { p10_state = 85; state_init = 1; }

        if (ADC_Left > Left_Black_To_Gray && ADC_Right > Right_Black_To_Gray) {
            RIGHT_REVERSE_SPEED = WHEEL_OFF; LEFT_REVERSE_SPEED  = WHEEL_OFF;
            RIGHT_FORWARD_SPEED = TRVL_STR_R; LEFT_FORWARD_SPEED  = TRVL_STR_L;
        }
        else if (ADC_Left >= Left_Black_To_Gray && ADC_Right < Right_Black_To_Gray) {
            last_line_pos = 1;
            RIGHT_REVERSE_SPEED = WHEEL_OFF; LEFT_REVERSE_SPEED  = WHEEL_OFF;

            if (ADC_Left > Left_Gray_To_White) {
                RIGHT_FORWARD_SPEED = TRVL_GENTLE_LEFT_R_FAST; LEFT_FORWARD_SPEED  = TRVL_GENTLE_LEFT_L_SLOW;
            } else {
                RIGHT_FORWARD_SPEED = TRVL_HARD_LEFT_R_FAST; LEFT_FORWARD_SPEED  = TRVL_HARD_LEFT_L_SLOW;
            }
        }
        else if (ADC_Right >= Right_Black_To_Gray && ADC_Left < Left_Black_To_Gray) {
            last_line_pos = 2;
            RIGHT_REVERSE_SPEED = WHEEL_OFF; LEFT_REVERSE_SPEED  = WHEEL_OFF;

            if (ADC_Right > Right_Gray_To_White) {
                RIGHT_FORWARD_SPEED = TRVL_GENTLE_RIGHT_R_SLOW; LEFT_FORWARD_SPEED  = TRVL_GENTLE_RIGHT_L_FAST;
            } else {
                RIGHT_FORWARD_SPEED = TRVL_HARD_RIGHT_R_SLOW; LEFT_FORWARD_SPEED  = TRVL_HARD_RIGHT_L_FAST;
            }
        }
        else {
            RIGHT_REVERSE_SPEED = WHEEL_OFF; LEFT_REVERSE_SPEED  = WHEEL_OFF;
            if (last_line_pos == 1) {
                RIGHT_FORWARD_SPEED = TRVL_HARD_LEFT_R_FAST; LEFT_FORWARD_SPEED  = TRVL_HARD_LEFT_L_SLOW;
            } else {
                RIGHT_FORWARD_SPEED = TRVL_HARD_RIGHT_R_SLOW; LEFT_FORWARD_SPEED  = TRVL_HARD_RIGHT_L_FAST;
            }
        }
        break;

    case 85:
        if(state_init){
            Wheels_All_Off();
            strcpy(display_line[0], "BL Circle ");
            display_changed = 1;
            run_time = 0;
            state_init = 0;
        }
        if(run_time >= 200){ p10_state = 86; state_init = 1; }
        break;

    //------------------------------------------------------------------------------
    // Sequence 5: Circle Tracking Logic
    //------------------------------------------------------------------------------
    case 86:
        if(state_init){ run_time = 0; state_init = 0; }

        if (ADC_Left > Left_Black_To_Gray && ADC_Right > Right_Black_To_Gray) {
            RIGHT_REVERSE_SPEED = WHEEL_OFF; LEFT_REVERSE_SPEED  = WHEEL_OFF;

            if (last_line_pos == 1) {
                RIGHT_FORWARD_SPEED = CRCL_HARD_LEFT_R_FAST; LEFT_FORWARD_SPEED  = CRCL_HARD_LEFT_L_SLOW;
            } else {
                RIGHT_FORWARD_SPEED = CRCL_HARD_RIGHT_R_SLOW; LEFT_FORWARD_SPEED  = CRCL_HARD_RIGHT_L_FAST;
            }
        }
        else if (ADC_Left >= Left_Black_To_Gray && ADC_Right < Right_Black_To_Gray) {
            last_line_pos = 1;
            RIGHT_REVERSE_SPEED = WHEEL_OFF; LEFT_REVERSE_SPEED  = WHEEL_OFF;

            if (ADC_Left > Left_Gray_To_White) {
                RIGHT_FORWARD_SPEED = CRCL_GENTLE_LEFT_R_FAST; LEFT_FORWARD_SPEED  = CRCL_GENTLE_LEFT_L_SLOW;
            } else {
                RIGHT_FORWARD_SPEED = CRCL_HARD_LEFT_R_FAST; LEFT_FORWARD_SPEED  = CRCL_HARD_LEFT_L_SLOW;
            }
        }
        else if (ADC_Right >= Right_Black_To_Gray && ADC_Left < Left_Black_To_Gray) {
            last_line_pos = 2;
            RIGHT_REVERSE_SPEED = WHEEL_OFF; LEFT_REVERSE_SPEED  = WHEEL_OFF;

            if (ADC_Right > Right_Gray_To_White) {
                RIGHT_FORWARD_SPEED = CRCL_GENTLE_RIGHT_R_SLOW; LEFT_FORWARD_SPEED  = CRCL_GENTLE_RIGHT_L_FAST;
            } else {
                RIGHT_FORWARD_SPEED = CRCL_HARD_RIGHT_R_SLOW; LEFT_FORWARD_SPEED  = CRCL_HARD_RIGHT_L_FAST;
            }
        }
        else {
            RIGHT_REVERSE_SPEED = WHEEL_OFF; LEFT_REVERSE_SPEED  = WHEEL_OFF;
            if (last_line_pos == 1) {
                RIGHT_FORWARD_SPEED = CRCL_HARD_LEFT_R_FAST; LEFT_FORWARD_SPEED  = CRCL_HARD_LEFT_L_SLOW;
            } else {
                RIGHT_FORWARD_SPEED = CRCL_HARD_RIGHT_R_SLOW; LEFT_FORWARD_SPEED  = CRCL_HARD_RIGHT_L_FAST;
            }
        }
        break;

    //------------------------------------------------------------------------------
    // Sequence 6: Exit Procedure
    //------------------------------------------------------------------------------
    case 9:
        if(state_init){
            Wheels_All_Off();
            strcpy(display_line[0], " BL Exit  ");
            display_changed = 1;
            run_time = 0;
            state_init = 0;
        }
        if(run_time >= 200){ p10_state = 92; state_init = 1; }
        break;

    case 92:
        if(state_init){
            Spin_CCW_On();
            run_time = 0;
            state_init = 0;
        }
        if (run_time >= 7) { p10_state = 95; state_init = 1; }
        break;

    case 95:
        if(state_init){
            Forward_On();
            run_time = 0;
            state_init = 0;
        }
        if (run_time >= 50) { p10_state = 10; state_init = 1; }
        break;

    case 10:
        if(state_init){
            Wheels_All_Off();
            strcpy(display_line[0], " BL Stop  ");
            strcpy(display_line[1], "That was  ");
            strcpy(display_line[2], "easy!! ;-)");
            strcpy(display_line[3], "Time: 000s");
            unsigned int final_time = Time_Sequence / 20;
            display_line[3][6] = (final_time / 100) % 10 + '0';
            display_line[3][7] = (final_time / 10) % 10 + '0';
            display_line[3][8] = (final_time % 10) + '0';
            display_changed = 1;
            state_init = 0;
        }
        break;
    }
}
