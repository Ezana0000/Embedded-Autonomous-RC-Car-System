//------------------------------------------------------------------------------
//  NC State University ECE 306
//  Instructor: Jim Carlson
//  Homework #03
//
//  File Name: led.c
//  Author: Ezana Enquobahrie
//  Date: February 5, 2026
//  Compiler: Code Composer Studio 12.8.1
//
//  Description:
//  This file contains LED initialization routines.
//  It sets the initial output state for the LEDs.
//
//  Global Variables Defined:
//  None
//------------------------------------------------------------------------------

#include  "msp430.h"
#include  <string.h>
#include  "include/functions.h"
#include  "Include/LCD.h"
#include  "include/ports.h"
#include  "macros.h"

//------------------------------------------------------------------------------
//  Function Name: Init_LEDs
//
//  Description:
//  Initializes LED output states.
//
//  Globals Used:
//  None
//
//  Globals Modified:
//  P1OUT, P6OUT
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
extern volatile unsigned char display_changed;
extern volatile unsigned int Time_Sequence;
extern volatile char one_time;


void Init_LEDs(void){
  P1OUT &= ~RED_LED;
  P6OUT &= ~GRN_LED;
}

//------------------------------------------------------------------------------
//  Function Name: Carlson_StateMachine
//
//  Description:
//  Executes timed actions based on Time_Sequence.
//
//  Globals Used:
//  Time_Sequence, one_time
//
//  Globals Modified:
//  Time_Sequence, one_time, display_changed
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

void Carlson_StateMachine(void){
  switch(Time_Sequence){
    case 250:
      if(one_time){
        Init_LEDs();
        lcd_BIG_mid();
        display_changed = 1;
        one_time = 0;
      }
      Time_Sequence = 0;
      break;

    case 200:
      if(one_time){
        P6OUT |= GRN_LED;
        one_time = 0;
      }
      break;

    case 150:
      if(one_time){
        P1OUT |= RED_LED;
        P6OUT &= ~GRN_LED;
        one_time = 0;
      }
      break;

    case 100:
      if(one_time){
        lcd_BIG_bot();
        P6OUT |= GRN_LED;
        display_changed = 1;
        one_time = 0;
      }
      break;

    case 50:
      if(one_time){
        one_time = 0;
      }
      break;

    default:
      break;
  }
}

