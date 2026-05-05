//------------------------------------------------------------------------------
//
//  Description: Handles very low-level system config like the global interrupt toggle
//
//  Ahan
//  Feb 2026
//  Built with Code Composer Studio
//------------------------------------------------------------------------------
#include "msp430.h"
#include "functions.h"
#include "LCD.h"
#include "ports.h"
#include "macros.h"

void enable_interrupts(void);

// I call this function from my init block to enable the global interrupt mask.
// Without this, none of my timers will tick.
void enable_interrupts(void){
  __bis_SR_register(GIE);     // Turn on global interrupts
}
