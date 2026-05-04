//------------------------------------------------------------------------------
//
//  Description: Controls the process of updating the LCD screen safely
//
// Ezana
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



// Convert HEX to BCD to ASCII based on Project 6 instructions
void HEXtoBCD(int hex_value, char* bcd_string){
    int value_passed = hex_value;
    int thousands = 0;
    int hundreds = 0;
    int tens = 0;
    int ones = 0;

    // *** CHANGED: 'if' to 'while' to handle 12-bit ADC values up to 4095 ***
    while(value_passed >= 1000){
        value_passed = value_passed - 1000;
        thousands++;
    }
    while(value_passed >= 100){
        value_passed = value_passed - 100;
        hundreds++;
    }
    while(value_passed >= 10){
        value_passed = value_passed - 10;
        tens++;
    }
    ones = value_passed;



    // Convert to ASCII by ORing with 0x30 and store in array
    bcd_string[0] = thousands | 0x30;
    bcd_string[1] = hundreds | 0x30;
    bcd_string[2] = tens | 0x30;
    bcd_string[3] = ones | 0x30;
    bcd_string[4] = '\0'; // Null terminator
}
