//------------------------------------------------------------------------------
//  NC State University ECE 306
//  Instructor: Jim Carlson
//  Homework #04
//
//  File Name: main.c
//  Author: Ezana Enquobahrie
//  Date: February 14, 2026
//  Compiler: Code Composer Studio 12.8.1
//
//  Description:
//  The Main Loop. This file sets up the 5ms system tick using a timer interrupt.
//  It calls the Shapes and Switches processes every 5ms to ensure smooth movement.
//
//  Global Variables Defined:
//  slow_input_down, display_mode, old_Time_Sequence, mytime, wheel_period, forward
//------------------------------------------------------------------------------
#include  "msp430.h"
#include  <string.h>
#include  "include/functions.h"
#include  "Include/LCD.h"
#include  "include/ports.h"
#include  "macros.h"

// Function Prototypes
void main(void);
void Init_Conditions(void);
void Display_Process(void);
void Init_LEDs(void);
void Carlson_StateMachine(void);
void Switches_Process(void);
void Shapes_Process(void);

// Global Variables
volatile char slow_input_down;
extern char display_line[4][11];
extern char *display[4];
unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern volatile unsigned int Time_Sequence;
extern volatile char one_time;

// Project 4 Globals
unsigned int old_Time_Sequence;
unsigned int mytime;
unsigned int wheel_period;
char forward;

//------------------------------------------------------------------------------
//  Function Name: main
//
//  Description:
//  This is the main routine. It initializes all peripherals and then enters
//  an infinite loop that processes events every 5ms (System Tick).
//
//  Globals Used:
//  Time_Sequence, old_Time_Sequence
//
//  Globals Modified:
//  mytime, wheel_period
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
void main(void){
  PM5CTL0 &= ~LOCKLPM5;

  Init_Ports();
  Motors_All_Off();
  Init_Clocks();
  Init_Conditions();
  Init_Timers();
  Init_LCD();

  strcpy(display_line[0], "   NCSU   ");
  strcpy(display_line[1], " WOLFPACK ");
  strcpy(display_line[2], "  ECE306  ");
  strcpy(display_line[3], " PROJECT4 ");
  display_changed = TRUE;

  // Initialize Variables
  old_Time_Sequence = 0;
  mytime = 0;
  wheel_period = 0;
  forward = TRUE;

  while(ALWAYS) {
    // -------------------------------------------------------------------------
    // SYSTEM TICK (Runs approx every 5ms)
    // -------------------------------------------------------------------------
    if(Time_Sequence != old_Time_Sequence){
        old_Time_Sequence = Time_Sequence;
        mytime++;
        wheel_period++;


        Switches_Process();
        Shapes_Process();
        Carlson_StateMachine();
        Display_Process();
    }
  }
}
