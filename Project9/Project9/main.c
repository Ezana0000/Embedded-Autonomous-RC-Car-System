//===========================================================================
// File Name : main.c
//===========================================================================

#include "msp430.h"
#include <string.h>
#include "functions.h"
#include "LCD.h"
#include "ports.h"
#include "macros.h"

// --- Macros from Kayla's Code ---
#define WAIT ('W')
#define FORWARDS ('F')
#define BACK ('B')
#define RIGHT ('R')
#define LEFT ('L')

extern char display_line[4][11];
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;

// Pulling variables from serial.c
extern volatile char ip_display1[11];
extern volatile char ip_display2[11];
extern volatile char IOT_Ring_Rx[32];
void Transmit_UCA0_String(char *str);

// --- RESTORED TIMER VARIABLES (Fixes Linker Error) ---
volatile unsigned int Time_Sequence = 0;
volatile unsigned int p5_timer = 0;
volatile unsigned int p5_running = 0;

// --- KAYLA'S STATE MACHINE VARIABLES ---
unsigned int response_parse = 0;
unsigned int command_op = WAIT;
unsigned int setTime = 0;
unsigned int movement = 0;
volatile unsigned int run_time = 0; // Increment this in your Timer ISR!
unsigned int run_time_flag = 0;

int hw8_mode = 1;
int hw8_action = 0;

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

    // --- SPLASH SCREEN ---
    strcpy(display_line[0], "  EZANA   ");
    strcpy(display_line[1], " BOOTING  ");
    strcpy(display_line[2], " WIFI...  ");
    strcpy(display_line[3], "          ");
    display_changed = 1;
    update_display = 1;
    Display_Process();

    __enable_interrupt();

    // =======================================================================
    // BOOT SEQUENCE
    // =======================================================================
    Transmit_UCA0_String("AT+CWJAP=\"ncsu\",\"\"\r\n");
    __delay_cycles(48000000);

    Transmit_UCA0_String("AT+CIPSERVER=1,8080\r\n");
    __delay_cycles(8000000);

    Transmit_UCA0_String("AT+CIFSR\r\n");
    __delay_cycles(8000000);

    strcpy(display_line[0], "IP ADDRESS");
    strcpy(display_line[1], "          ");
    strcpy(display_line[2], (char *)ip_display1);
    strcpy(display_line[3], (char *)ip_display2);
    display_changed = 1;
    Display_Process();

    // Kayla's Initial State
    command_op = WAIT;
    // =======================================================================

    while(ALWAYS) {

        // =======================================================================
        // KAYLA'S PARSER
        // =======================================================================
        if (IOT_Ring_Rx[response_parse++] == '^') {
            if(IOT_Ring_Rx[response_parse] == '3' && IOT_Ring_Rx[response_parse+1] == '0' &&
               IOT_Ring_Rx[response_parse+2] == '6'&& IOT_Ring_Rx[response_parse+3] == '1'){

                response_parse = response_parse + 4;
                int vv;

                switch (IOT_Ring_Rx[response_parse]){
                    case 'F':
                        if(movement){ break; }
                        setTime = ((IOT_Ring_Rx[response_parse+1] - '0') * 100) +
                                  ((IOT_Ring_Rx[response_parse+2] - '0') * 10) +
                                  (IOT_Ring_Rx[response_parse+3] - '0');
                        run_time_flag = 1;
                        command_op = FORWARDS;
                        run_time = 0;
                        for (vv = 0; vv < 32; vv++){ IOT_Ring_Rx[vv] = 0; }
                        break;
                    case 'B':
                        if(movement){ break; }
                        setTime = ((IOT_Ring_Rx[response_parse+1] - '0') * 100) +
                                  ((IOT_Ring_Rx[response_parse+2] - '0') * 10) +
                                  (IOT_Ring_Rx[response_parse+3] - '0');
                        run_time_flag = 1;
                        command_op = BACK;
                        run_time = 0;
                        for (vv = 0; vv < 32; vv++){ IOT_Ring_Rx[vv] = 0; }
                        break;
                    case 'R':
                        if(movement){ break; }
                        setTime = ((IOT_Ring_Rx[response_parse+1] - '0') * 100) +
                                  ((IOT_Ring_Rx[response_parse+2] - '0') * 10) +
                                  (IOT_Ring_Rx[response_parse+3] - '0');
                        run_time_flag = 1;
                        command_op = RIGHT;
                        run_time = 0;
                        for (vv = 0; vv < 32; vv++){ IOT_Ring_Rx[vv] = 0; }
                        break;
                    case 'L':
                        if(movement){ break; }
                        setTime = ((IOT_Ring_Rx[response_parse+1] - '0') * 100) +
                                  ((IOT_Ring_Rx[response_parse+2] - '0') * 10) +
                                  (IOT_Ring_Rx[response_parse+3] - '0');
                        run_time_flag = 1;
                        command_op = LEFT;
                        run_time = 0;
                        for (vv = 0; vv < 32; vv++){ IOT_Ring_Rx[vv] = 0; }
                        break;
                    case 'S':
                        if(movement){ break; }
                        command_op = WAIT;
                        run_time = 0;
                        for (vv = 0; vv < 32; vv++){ IOT_Ring_Rx[vv] = 0; }
                        break;
                }
            }
        }

        if (response_parse > 31) {
            response_parse = 0;
        }

        // =======================================================================
        // KAYLA'S EXECUTION STATE MACHINE (Patched with Ezana's Motor Logic)
        // =======================================================================
        switch (command_op){
            case WAIT:
                Wheels_All_Off();
                strcpy(display_line[1], " WAITING  ");
                display_changed = TRUE;
                break;

            case FORWARDS:
                Forward_On();
                strcpy(display_line[1], " FORWARD  ");
                display_changed = TRUE;

                if (run_time > setTime){
                    run_time_flag = 0;
                    Wheels_All_Off();
                    run_time = 0;
                    command_op = WAIT;
                    movement = 0;
                }
                break;

            case BACK:
                Reverse_On();
                strcpy(display_line[1], " REVERSE  ");
                display_changed = TRUE;

                if (run_time > setTime){
                    run_time_flag = 0;
                    Wheels_All_Off();
                    run_time = 0;
                    command_op = WAIT;
                    movement = 0;
                }
                break;

            case RIGHT:
                Spin_CW_On(); // Clockwise turn
                strcpy(display_line[1], " RIGHT    ");
                display_changed = TRUE;

                if (run_time >= setTime){
                    run_time_flag = 0;
                    Wheels_All_Off();
                    run_time = 0;
                    command_op = WAIT;
                    movement = 0;
                }
                break;

            case LEFT:
                Spin_CCW_On(); // Counter-clockwise turn
                strcpy(display_line[1], " LEFT     ");
                display_changed = TRUE;

                if (run_time >= setTime){
                    run_time_flag = 0;
                    Wheels_All_Off();
                    run_time = 0;
                    command_op = WAIT;
                    movement = 0;
                }
                break;

            default: break;
        }

        Display_Process();
    }
}
