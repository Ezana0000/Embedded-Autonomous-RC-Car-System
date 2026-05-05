//------------------------------------------------------------------------------
//  NC State University ECE 306
//  Instructor: Jim Carlson
//  Homework #03
//
//  File Name: Display.c
//  Author: Ezana Enquobahrie
//  Date: February 5, 2026
//  Compiler: Code Composer Studio 12.8.1
//
//  Description:
//  This file contains the display update processing for the LCD.
//  It calls Display_Update when update flags indicate new content.
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

extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;

//------------------------------------------------------------------------------
//  Function Name: Display_Process
//
//  Description:
//  Checks display update flags and updates the LCD when necessary.
//
//  Globals Used:
//  update_display, display_changed
//
//  Globals Modified:
//  update_display, display_changed
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

void Display_Process(void){
  if(update_display){
    update_display = 0;
    if(display_changed){
      display_changed = 0;
      Display_Update(0,0,0,0);
    }
  }
}
