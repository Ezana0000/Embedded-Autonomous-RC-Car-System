//===========================================================================
// File Name : main.c
// Description: Main Routine - Background/Foreground Operating System.
// Author: Ezana
//===========================================================================

#include "msp430.h"
#include <string.h>
#include "functions.h"
#include "LCD.h"
#include "ports.h"
#include "macros.h"

extern char display_line[4][11];
extern volatile unsigned char display_changed;

volatile unsigned int p5_timer = 0;
volatile unsigned int p5_running = 0;
unsigned int p5_step = 0;
char p5_started = 0;
char step_init = 1;

void main(void){
    PM5CTL0 &= ~LOCKLPM5;               // Disable high-impedance mode

    Init_Ports();
    Init_Clocks();
    Init_Timers();
    Init_LCD();

    Init_Conditions();                  // Must be placed here to safely enable interrupts

    strcpy(display_line[0], " Project5 ");
    strcpy(display_line[1], "          ");
    strcpy(display_line[2], " Press SW1");
    strcpy(display_line[3], " to Start ");
    display_changed = TRUE;

    Wheels_All_Off();

    while(ALWAYS) {
        if(p5_started){
            Run_Project5();
        }
        Display_Process();              // Executes safely based on the 200ms timer
        P3OUT ^= TEST_PROBE;
    }
}
