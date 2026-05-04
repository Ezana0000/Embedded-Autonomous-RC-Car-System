//------------------------------------------------------------------------------
//
//  Description: Responsible for setting up the initial system variables and arrays
//
//  Ezana
//  Feb 2026
//  Built with Code Composer Studio
//------------------------------------------------------------------------------
#include "msp430.h"
#include "functions.h"
#include "LCD.h"
#include "ports.h"
#include "macros.h"

extern char display_line[4][11];
extern char *display[4];
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;


void Init_Conditions(void){
//------------------------------------------------------------------------------
  int i;

  // I loop through all elements in the 2D array to make sure it's completely clear
  for(i=0; i<11; i++){
    display_line[0][i] = RESET_STATE;
    display_line[1][i] = RESET_STATE;
    display_line[2][i] = RESET_STATE;
    display_line[3][i] = RESET_STATE;
  }

  // I must null-terminate the end of the strings so strcpy doesn't crash
  display_line[0][10] = 0;
  display_line[1][10] = 0;
  display_line[2][10] = 0;
  display_line[3][10] = 0;

  // Link my display pointers to the beginning of the arrays
  display[0] = &display_line[0][0];
  display[1] = &display_line[1][0];
  display[2] = &display_line[2][0];
  display[3] = &display_line[3][0];

  update_display = 0; // Make sure the display isn't triggered yet

  // I need to enable global interrupts here, or my timers and buttons won't work
  enable_interrupts();
//------------------------------------------------------------------------------
}
