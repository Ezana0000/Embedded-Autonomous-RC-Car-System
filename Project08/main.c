//===========================================================================
// File Name : main.c
//===========================================================================

#include "msp430.h"
#include <string.h>
#include "functions.h"
#include "LCD.h"
#include "ports.h"
#include "macros.h"

extern char display_line[4][11];
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;

// --- SYSTEM VARIABLES (Needed for Timers and P7) ---
volatile unsigned int backlight = 0;
volatile unsigned int p5_timer = 0;
volatile unsigned int p5_running = 0;
unsigned int p5_step = 0;
char p5_started = 0;
char step_init = 1;
volatile unsigned int Time_Sequence = 0;

// --- HW8 / PROJECT 8 VARIABLES ---
int hw8_mode = 1;    // 1 = Doing Project 8
int hw8_action = 0;  // Caught by the while loop when a button is pressed

// Project 8 Serial Variables
extern char ad2_message[11];
extern volatile char message_ready;
extern volatile unsigned int ad2_rx_index;

unsigned long current_baud = 115200;
char baud_str[11] = " 115,200  ";

// Serial Functions

void main(void){
    PM5CTL0 &= ~LOCKLPM5;

    Init_Ports();
    Init_Clocks();
    Init_Timers();
    Init_LCD();
    Init_Conditions();

    Init_Serial_UCA0();
    Init_Serial_UCA1();

    P6OUT |= LCD_BACKLITE;

    if (hw8_mode) {
        // --- SPLASH SCREEN ---
        strcpy(display_line[0], "  EZANA   ");
        strcpy(display_line[1], "  PROJ 8  ");
        strcpy(display_line[2], " Starting ");
        strcpy(display_line[3], "          ");
        display_changed = 1;
        update_display = 1;
        Display_Process();

        // Wait 2 seconds for splash (8MHz clock = 16 million cycles)
        __delay_cycles(16000000);

        // STATE 1: BOOT UP / WAITING
        strcpy(display_line[0], " Waiting  ");
        strcpy(display_line[1], "          ");
        strcpy(display_line[2], "   Baud   ");
        strcpy(display_line[3], baud_str);
        display_changed = 1;
        update_display = 1;
        Display_Process();
    }

    while(ALWAYS) {
        if (hw8_mode) {

            // STATE 2: MESSAGE RECEIVED
            if (message_ready) {
                message_ready = 0; // Lower the flag

                strcpy(display_line[0], " Received ");
                strcpy(display_line[1], "          ");
                strcpy(display_line[2], "   Baud   ");
                strcpy(display_line[3], ad2_message); // Show the 10 characters
                display_changed = 1;
            }

            // STATE 3: SW1 PRESSED (Transmit back to AD2)
            if (hw8_action == 1) {
                hw8_action = 0; // Reset flag

                strcpy(display_line[0], " Transmit ");
                strcpy(display_line[1], ad2_message); // Move message up to Line 2
                strcpy(display_line[2], "   Baud   ");
                strcpy(display_line[3], "          "); // Clear bottom line
                display_changed = 1;

                // Update screen before sending
                Display_Process();

                Transmit_UCA0_String(ad2_message);
            }

            // STATE 4: SW2 PRESSED (Baud Swap)
            if (hw8_action == 2) {
                hw8_action = 0; // Reset flag

                // Toggle Baud Rate Logic
                if (current_baud == 115200) {
                    Change_Baud_57600();
                    current_baud = 57600;
                    strcpy(baud_str, "  57,600  ");
                } else {
                    Change_Baud_115200();
                    current_baud = 115200;
                    strcpy(baud_str, " 115,200  ");
                }

                // Reset back to Waiting State
                strcpy(display_line[0], " Waiting  ");
                strcpy(display_line[1], "          ");
                strcpy(display_line[2], "   Baud   ");
                strcpy(display_line[3], baud_str);

                // Clear the message buffer out so old text doesn't linger
                strcpy(ad2_message, "          ");
                ad2_rx_index = 0;

                display_changed = 1;
            }
        }

        Display_Process();
    }
}
