//------------------------------------------------------------------------------
//  NC State University ECE 306
//  Instructor: Jim Carlson
//  Homework #04
//
//  File Name: shapes.c
//  Author: Ezana Enquobahrie
//  Date: February 14, 2026
//  Compiler: Code Composer Studio 12.8.1
//
//  Description:
//  Complete Shape State Machine for Project 4.
//  Handles the logic for Circle, Figure-8 (with split timing), and Triangle.
//  Includes a "Decision Window" to detect double-button presses reliably.
//
//  Global Variables Defined:
//  state, active_shape, shape_timer, segment_count
//------------------------------------------------------------------------------
#include  "msp430.h"
#include  "macros.h"
#include  "include/functions.h"
#include  "include/ports.h"
#include  <string.h>

extern volatile int sw1_position;
extern volatile int sw2_position;
extern char display_line[4][11];
extern volatile unsigned char display_changed;
extern unsigned int wheel_period;

unsigned int state = IDLE;
unsigned int active_shape = NONE;
unsigned int shape_timer = 0;
unsigned int segment_count = 0;

//------------------------------------------------------------------------------
//  Function Name: Execute_PWM
//
//  Description:
//  Direct wheel control based on wheel_period from main.
//  Ezana: This ensures both wheels turn at all times during circles.
//
//  Globals Used:
//  wheel_period
//
//  Globals Modified:
//  P6OUT
//
//  Passed Variables:
//  left_duty, right_duty
//
//  Local Variables:
//  None
//
//  Returned Value:
//  None
//------------------------------------------------------------------------------
void Execute_PWM(unsigned int left_duty, unsigned int right_duty) {
    if(wheel_period < left_duty) P6OUT |= L_FORWARD;
    else P6OUT &= ~L_FORWARD;

    if(wheel_period < right_duty) P6OUT |= R_FORWARD;
    else P6OUT &= ~R_FORWARD;

    if(wheel_period >= WHEEL_COUNT_TIME) wheel_period = 0;
}

//------------------------------------------------------------------------------
//  Function Name: Shapes_Process
//
//  Description:
//  Main State Machine for movement.
//  IDLE -> PAUSE (Decision) -> GO -> RUN -> STOP.
//
//  Globals Used:
//  sw1_position, sw2_position
//
//  Globals Modified:
//  state, active_shape, shape_timer, segment_count
//
//  Passed Variables:
//  None
//
//  Local Variables:
//  None
//
//  Returned Value:
//  None
//------------------------------------------------------------------------------
void Shapes_Process(void){
    switch(state){
        case IDLE:
            if(sw1_position == PRESSED || sw2_position == PRESSED){
                state = PAUSE;
                shape_timer = 0;
                active_shape = NONE;
            }
            break;

        case PAUSE:
            shape_timer++;

            if(sw1_position == PRESSED && sw2_position == PRESSED){
                active_shape = TRIANGLE;
            }

            if(shape_timer >= PAUSE_DURATION){
                if(active_shape == NONE){
                    if(sw1_position == PRESSED) active_shape = CIRCLE;
                    else if(sw2_position == PRESSED) active_shape = FIGURE8;
                }

                if(active_shape != NONE){
                    state = GO;
                    if(active_shape == TRIANGLE) strcpy(display_line[0], " TRIANGLE ");
                    else if(active_shape == CIRCLE) strcpy(display_line[0], "  CIRCLE  ");
                    else strcpy(display_line[0], " FIGURE 8 ");
                    display_changed = TRUE;
                } else {
                    state = IDLE;
                }
            }
            break;

        case GO:
            shape_timer = 0;
            segment_count = 0;
            state = RUN;
            break;

        case RUN:
            shape_timer++;

            if(active_shape == CIRCLE){
                Execute_PWM(LEFT_COUNT_TIME, SLOW_COUNT_TIME);
                if(shape_timer >= CIRCLE_TIME){
                    shape_timer = 0;
                    segment_count++;
                    if(segment_count >= 2) state = STOP;
                }
            }
            else if(active_shape == FIGURE8){
                if(segment_count == 0 || segment_count == 2){
                     Execute_PWM(SLOW_COUNT_TIME, RIGHT_COUNT_TIME); // Turn Left
                     if(shape_timer >= CIRCLE_TIME_L){
                        shape_timer = 0;
                        segment_count++;
                     }
                }
                else {
                     Execute_PWM(LEFT_COUNT_TIME, SLOW_COUNT_TIME); // Turn Right
                     if(shape_timer >= CIRCLE_TIME_R){
                        shape_timer = 0;
                        segment_count++;
                     }
                }
                if(segment_count >= 4) state = STOP;
            }
            else if(active_shape == TRIANGLE){
                // Straight side for even segments (0,2,4 loop 1; 6,8,10 loop 2)
                if(segment_count == 0 || segment_count == 2 || segment_count == 4 ||
                   segment_count == 6 || segment_count == 8 || segment_count == 10){
                    Execute_PWM(LEFT_COUNT_TIME, RIGHT_COUNT_TIME);
                    if(shape_timer >= SIDE_TIME){
                        shape_timer = 0;
                        segment_count++;
                    }
                }
                // Pivot turn for odd segments (1,3,5 loop 1; 7,9,11 loop 2)
                else {
                    Execute_PWM(LEFT_COUNT_TIME, 0);
                    if(shape_timer >= TURN_TIME){
                        shape_timer = 0;
                        segment_count++;
                    }
                }
                if(segment_count >= 12) state = STOP;
            }
            break;

        case STOP:
            P6OUT &= ~L_FORWARD;
            P6OUT &= ~R_FORWARD;
            active_shape = NONE;
            state = IDLE;
            strcpy(display_line[0], "   DONE   ");
            display_changed = TRUE;
            break;

        default: break;
    }
}
