//------------------------------------------------------------------------------
//
//  Description: Controls the process of updating the LCD screen safely
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

extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;

void Display_Process(void){
  // I only update the physical display if the timer flag tells me enough time has passed
  if(update_display){
    update_display = 0; // Clear the timer flag

    // Check if the actual text content has changed
    if(display_changed){
      display_changed = 0; // Reset my local flag
      Display_Update(0,0,0,0); // Push the new text to the screen
    }
  }
}
