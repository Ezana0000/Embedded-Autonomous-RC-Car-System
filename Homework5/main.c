//------------------------------------------------------------------------------
//  NC State University ECE 306
//  Instructor: Jim Carlson
//  Homework #05
//
//  File Name: main.c
//  Author: Ezana Enquobahrie
//------------------------------------------------------------------------------
#include  "msp430.h"
#include  <string.h>
#include  "include/functions.h"
#include  "Include/LCD.h"
#include  "include/ports.h"
#include  "macros.h"

extern char display_line[4][11];
extern volatile unsigned char display_changed;
extern volatile unsigned int Time_Sequence;
extern volatile int sw1_position;
extern volatile int sw2_position;
unsigned int old_Time_Sequence;

void main(void){
  PM5CTL0 &= ~LOCKLPM5;

  Init_Ports();
  Init_Clocks();
  Init_Conditions();
  Init_Timers();
  Init_LCD();

  strcpy(display_line[0], " LED1 ON ");
  strcpy(display_line[1], " WOLFPACK ");
  strcpy(display_line[2], "  HW 05   ");
  strcpy(display_line[3], " 500 kHz ");
  display_changed = TRUE;

  old_Time_Sequence = 0;

  while(ALWAYS) {
    if(Time_Sequence != old_Time_Sequence){
        old_Time_Sequence = Time_Sequence;
        Switches_Process();

        // Switch Logic to toggle SMCLK output
        if(sw1_position == PRESSED){
            Init_Port_3(USE_SMCLK);
        }
        if(sw2_position == PRESSED){
            Init_Port_3(USE_GPIO);
        }

        Carlson_StateMachine();
        Display_Process();
    }
  }
}
