//------------------------------------------------------------------------------
//  NC State University ECE 306
//  Instructor: Jim Carlson
//  Homework #03
//
//  File Name: init.c
//  Author: Ezana Enquobahrie
//  Date: February 5, 2026
//  Compiler: Code Composer Studio 12.8.1
//
//  Description:
//  This file contains initialization and time-based control functions.
//  It initializes display buffers and runs the Carlson state machine.
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


extern char display_line[4][11];
extern char *display[4];
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int Time_Sequence;
extern volatile char one_time;

//------------------------------------------------------------------------------
//  Function Name: Init_Conditions
//
//  Description:
//  Initializes display buffers, pointers, and enables interrupts.
//
//  Globals Used:
//  display_line, display
//
//  Globals Modified:
//  display, update_display
//
//  Passed Variables:
//  None
//
//  Local Variables:
//  i
//
//  Returned Value:
//  None
//------------------------------------------------------------------------------

void Init_Conditions(void){
  int i;
  for(i=0;i<11;i++){
    display_line[0][i] = RESET_STATE;
    display_line[1][i] = RESET_STATE;
    display_line[2][i] = RESET_STATE;
    display_line[3][i] = RESET_STATE;
  }
  display_line[0][10] = 0;
  display_line[1][10] = 0;
  display_line[2][10] = 0;
  display_line[3][10] = 0;

  display[0] = &display_line[0][0];
  display[1] = &display_line[1][0];
  display[2] = &display_line[2][0];
  display[3] = &display_line[3][0];
  update_display = 0;

  enable_interrupts();
}

