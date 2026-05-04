//------------------------------------------------------------------------------
//
//  Description: I use this file to manage basic LED control functions
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

void Init_LEDs(void){
//------------------------------------------------------------------------------
// I'm initializing the LEDs by turning both the Red and Green ones on.
// This gives me a quick visual that the board powered up correctly.
//------------------------------------------------------------------------------
  P1OUT |= RED_LED;
  P6OUT |= GRN_LED;
//------------------------------------------------------------------------------
}
