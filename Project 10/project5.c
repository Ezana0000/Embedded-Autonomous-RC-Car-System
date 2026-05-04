//------------------------------------------------------------------------------
//
//  Description: This file contains the State Machine logic for Project 5.
//               I moved this here to keep the main operating system clean.
//
//  Ahan
//  Feb 2026
//  Built with Code Composer Studio
//------------------------------------------------------------------------------

#include "msp430.h"       // MSP430 register definitions
#include <string.h>       // For strcpy() to write LCD display strings
#include "functions.h"    // Function prototypes for all modules
#include "LCD.h"          // LCD control functions and constants
#include "ports.h"        // Port pin definitions
#include "macros.h"       // Project-wide #define constants

// I use 'extern' here to link to variables defined in my other files
extern char display_line[4][11];
extern volatile unsigned char display_changed;
extern volatile unsigned int p5_timer;
extern volatile unsigned int p5_running;
extern unsigned int p5_step;
extern char step_init;

//------------------------------------------------------------------------------
// Function: Run_Project5
// Description: My sequential state machine for the Project 5 movement sequence.
//              I use p5_timer (incremented in the ISR) to time each movement.
//------------------------------------------------------------------------------

void Run_Project5(void){

    switch(p5_step){

    //--- Step 1: Forward for 1 second -----------------------------------------
    case P5_FWD1:
        if(step_init){
            step_init = 0; // Lock out the init block
            strcpy(display_line[0], " Forward  "); // Update LCD
            strcpy(display_line[1], " 1 second ");
            strcpy(display_line[3], "  Step 1  ");
            display_changed = 1; // Flag the screen to refresh
            Wheels_All_Off();    // Safety: all off before direction change
            Forward_On();        // Both wheels forward
            p5_timer = 0;        // Reset the interrupt counter
            p5_running = 1;      // Start counting in ISR
        }
        if(p5_timer >= ONE_SEC){ // Wait for 200 ticks (1s)
            Wheels_All_Off();
            p5_step = P5_PAUSE1; // Move to the first pause
            step_init = 1;       // Reset init for the next state
        }
        break;

    //--- Pause 1 second -------------------------------------------------------
    case P5_PAUSE1:
        if(step_init){
            step_init = 0;
            strcpy(display_line[0], "  Pause   ");
            strcpy(display_line[1], " 1 second ");
            display_changed = 1;
            p5_timer = 0;
        }
        if(p5_timer >= ONE_SEC){
            p5_step = P5_REV;
            step_init = 1;
        }
        break;

    //--- Step 2: Reverse for 2 seconds ----------------------------------------
    case P5_REV:
        if(step_init){
            step_init = 0;
            strcpy(display_line[0], " Reverse  ");
            strcpy(display_line[1], " 2 seconds");
            strcpy(display_line[3], "  Step 2  ");
            display_changed = 1;
            Reverse_On();        // Both wheels reverse
            p5_timer = 0;
        }
        if(p5_timer >= TWO_SEC){ // Wait for 400 ticks (2s)
            Wheels_All_Off();
            p5_step = P5_PAUSE2;
            step_init = 1;
        }
        break;

    //--- Pause 1 second -------------------------------------------------------
    case P5_PAUSE2:
        if(step_init){
            step_init = 0;
            strcpy(display_line[0], "  Pause   ");
            strcpy(display_line[1], " 1 second ");
            display_changed = 1;
            p5_timer = 0;
        }
        if(p5_timer >= ONE_SEC){
            p5_step = P5_FWD2;
            step_init = 1;
        }
        break;

    //--- Step 3: Forward for 1 second -----------------------------------------
    case P5_FWD2:
        if(step_init){
            step_init = 0;
            strcpy(display_line[0], " Forward  ");
            strcpy(display_line[1], " 1 second ");
            strcpy(display_line[3], "  Step 3  ");
            display_changed = 1;
            Forward_On();
            p5_timer = 0;
        }
        if(p5_timer >= ONE_SEC){
            Wheels_All_Off();
            p5_step = P5_PAUSE3;
            step_init = 1;
        }
        break;

    //--- Pause 1 second -------------------------------------------------------
    case P5_PAUSE3:
        if(step_init){
            step_init = 0;
            strcpy(display_line[0], "  Pause   ");
            strcpy(display_line[1], " 1 second ");
            display_changed = 1;
            p5_timer = 0;
        }
        if(p5_timer >= ONE_SEC){
            p5_step = P5_SPIN_CW;
            step_init = 1;
        }
        break;

    //--- Step 4: Spin clockwise for 3 seconds ---------------------------------
    case P5_SPIN_CW:
        if(step_init){
            step_init = 0;
            strcpy(display_line[0], " Spin CW  ");
            strcpy(display_line[1], " 3 seconds");
            strcpy(display_line[3], "  Step 4  ");
            display_changed = 1;
            Spin_CW_On();        // Left fwd + Right rev
            p5_timer = 0;
        }
        if(p5_timer >= THREE_SEC){ // Wait for 600 ticks (3s)
            Wheels_All_Off();
            p5_step = P5_PAUSE4;
            step_init = 1;
        }
        break;

    //--- Pause 2 seconds -----------------------------------------------------
    case P5_PAUSE4:
        if(step_init){
            step_init = 0;
            strcpy(display_line[0], "  Pause   ");
            strcpy(display_line[1], " 2 seconds");
            display_changed = 1;
            p5_timer = 0;
        }
        if(p5_timer >= TWO_SEC){
            p5_step = P5_SPIN_CCW;
            step_init = 1;
        }
        break;

    //--- Step 5: Spin counter-clockwise for 3 seconds -------------------------
    case P5_SPIN_CCW:
        if(step_init){
            step_init = 0;
            strcpy(display_line[0], " Spin CCW ");
            strcpy(display_line[1], " 3 seconds");
            strcpy(display_line[3], "  Step 5  ");
            display_changed = 1;
            Spin_CCW_On();       // Right fwd + Left rev
            p5_timer = 0;
        }
        if(p5_timer >= THREE_SEC){
            Wheels_All_Off();
            p5_step = P5_PAUSE5;
            step_init = 1;
        }
        break;

    //--- Pause 2 seconds -----------------------------------------------------
    case P5_PAUSE5:
        if(step_init){
            step_init = 0;
            strcpy(display_line[0], "  Pause   ");
            strcpy(display_line[1], " 2 seconds");
            display_changed = 1;
            p5_timer = 0;
        }
        if(p5_timer >= TWO_SEC){
            p5_step = P5_DONE;
            step_init = 1;
        }
        break;

    //--- Done! ----------------------------------------------------------------
    case P5_DONE:
        Wheels_All_Off();
        p5_running = 0;               // Stop the timer counting
        strcpy(display_line[0], "   Done!  ");
        display_changed = 1;
        extern char p5_started;      // External flag from main
        p5_started = 0;               // Reset system
        break;

    default:
        Wheels_All_Off();
        break;
    }
}
