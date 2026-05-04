//------------------------------------------------------------------------------
//  NC State University ECE 306
//  Instructor: Jim Carlson
//  Homework #04
//
//  File Name: switches.c
//  Author: Ezana Enquobahrie
//  Date: February 14, 2026
//  Compiler: Code Composer Studio 12.8.1
//
//  Description:
//  This file contains the switch debounce logic.
//  It ensures that a single button press is not counted as multiple presses.
//
//  Global Variables Defined:
//  sw1_position, sw2_position, okay_to_look_at_switch1, okay_to_look_at_switch2,
//  count_debounce_SW1, count_debounce_SW2
//------------------------------------------------------------------------------
#include  "msp430.h"
#include  "macros.h"
#include  "include/ports.h"

// Global Variables for Switches
volatile int sw1_position = RELEASED;
volatile int sw2_position = RELEASED;
volatile int okay_to_look_at_switch1 = OKAY;
volatile int okay_to_look_at_switch2 = OKAY;
volatile int count_debounce_SW1 = 0;
volatile int count_debounce_SW2 = 0;

// Function Prototypes
void Switch1_Process(void);
void Switch2_Process(void);

//------------------------------------------------------------------------------
//  Function Name: Switches_Process
//  Description:
//  Master function to call individual switch processes.
//  Ezana: I call this from the main time loop.
//------------------------------------------------------------------------------
void Switches_Process(void){
    Switch1_Process();
    Switch2_Process();
}

//------------------------------------------------------------------------------
//  Function Name: Switch1_Process
//  Description:
//  Debounce Logic from Lecture Slides.
//  Checks if P4.1 (SW1) is pressed and waits for debounce time.
//
//  Globals Modified:
//  sw1_position, count_debounce_SW1
//------------------------------------------------------------------------------
void Switch1_Process(void){
    if (okay_to_look_at_switch1 && (sw1_position == RELEASED)){ // Look for press
        if (!(P4IN & SW1)){
            sw1_position = PRESSED;
            okay_to_look_at_switch1 = NOT_OKAY;
            count_debounce_SW1 = DEBOUNCE_RESTART;
        }
    }

    if (count_debounce_SW1 <= DEBOUNCE_TIME){
        count_debounce_SW1++;
    }else{
        okay_to_look_at_switch1 = OKAY;
        if (P4IN & SW1){ // If button is High (Released)
            sw1_position = RELEASED;
        }
    }
}

//------------------------------------------------------------------------------
//  Function Name: Switch2_Process
//  Description:
//  Debounce Logic for Switch 2.
//  Checks if P2.3 (SW2) is pressed and waits for debounce time.
//
//  Globals Modified:
//  sw2_position, count_debounce_SW2
//------------------------------------------------------------------------------
void Switch2_Process(void){
    if (okay_to_look_at_switch2 && (sw2_position == RELEASED)){
        if (!(P2IN & SW2)){
            sw2_position = PRESSED;
            okay_to_look_at_switch2 = NOT_OKAY;
            count_debounce_SW2 = DEBOUNCE_RESTART;
        }
    }

    if (count_debounce_SW2 <= DEBOUNCE_TIME){
        count_debounce_SW2++;
    }else{
        okay_to_look_at_switch2 = OKAY;
        if (P2IN & SW2){
            sw2_position = RELEASED;
        }
    }
}
