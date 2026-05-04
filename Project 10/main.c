//------------------------------------------------------------------------------
//
//  Description: Main routine for the autonomous vehicle.
//  Handles the network bootup sequence, command parsing, and core state handling.
//
//  Ezana
//  April 2026
//  Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------

#include "msp430.h"
#include <string.h>
#include "functions.h"
#include "LCD.h"
#include "ports.h"
#include "macros.h"

#define WAIT      ('W')
#define FORWARDS  ('F')
#define BACK      ('B')
#define RIGHT     ('R')
#define LEFT      ('L')
#define ARRIVED   ('A')
#define INTERCEPT ('I')
#define EXIT      ('E')

volatile unsigned int p5_timer = 0;
volatile unsigned int p5_running = 0;
unsigned int Left_Black_To_Gray = 0;
unsigned int Right_Black_To_Gray = 0;
unsigned int Left_Gray_To_White = 0;
unsigned int Right_Gray_To_White = 0;
volatile char course_started = 0;
volatile char calibration_done = 0;

// Dynamic intercept angle mapping
unsigned int intercept_type = 0;

extern char display_line[4][11];
extern volatile unsigned char display_changed;

extern volatile char IOT_Ring_Rx[32];
extern volatile unsigned int iot_rx_wr;
extern volatile char ip_display1[11];
extern volatile char ip_display2[11];
void Transmit_UCA0_String(char *str);

volatile unsigned int initialize_done = 0;
volatile unsigned int initial_process = 0;
volatile unsigned int iot_on_time = 0;
volatile unsigned int run_time = 0;
volatile unsigned int Time_Sequence = 0;
volatile unsigned int total_seconds = 0;

unsigned int last_ping_second = 0;
extern unsigned int p10_state;

unsigned int response_parse = 0;
unsigned int setTime = 0;
unsigned int command_op = WAIT;
char pad_number = '0';
char last_cmd[6] = "     ";

void main(void){
    unsigned int vv;
    int buffer_count;
    unsigned int p1, p2, p3, p4, dir, t1, t2, t3;

    PM5CTL0 &= ~LOCKLPM5;
    Init_Ports();
    Init_Clocks();
    Init_Timers();
    Init_LCD();
    Init_Conditions();
    Init_Serial_UCA0();
    Init_Serial_UCA1();
    Init_ADC();

    P6OUT |= LCD_BACKLITE;
    for(vv=0; vv<4; vv++) strcpy(display_line[vv], "          ");
    strcpy(display_line[0], "  EZANA   ");
    display_changed = 1;

    __enable_interrupt();

    while(ALWAYS) {
        //------------------------------------------------------------------------------
        // Bootup Sequence and Network Handshake
        //------------------------------------------------------------------------------
        if (initialize_done == 0) {
            if (iot_on_time < 15) {
                if (iot_on_time <= 10) strcpy(display_line[0], " IOT OFF  ");
                else { P3OUT |= IOT_EN_CPU; strcpy(display_line[0], " IOT ON   "); if (initial_process == 0) initial_process = 1; }
                display_changed = 1;
            }
            else if (iot_on_time >= 50) {
                switch (initial_process) {
                    case 1: Transmit_UCA0_String("AT\r\n"); initial_process = 2; break;
                    case 2: Transmit_UCA0_String("AT+SYSSTORE=0\r\n"); initial_process = 3; break;
                    case 3: Transmit_UCA0_String("AT+CIPMUX=1\r\n"); initial_process = 4; break;
                    case 4: Transmit_UCA0_String("AT+CIPSERVER=1,1221\r\n"); initial_process = 5; iot_on_time = 0; break;
                    case 5: if (iot_on_time >= 150) initial_process = 6; break;
                    case 6: Transmit_UCA0_String("AT+CIFSR\r\n"); initial_process = 7; break;
                    case 7: initialize_done = 1; Time_Sequence = 0; break;
                }

                if (initial_process != 5) iot_on_time = 40;
            }
        }
        //------------------------------------------------------------------------------
        // Sequential Command Parsing and Driving Mode
        //------------------------------------------------------------------------------
        else {
            total_seconds = Time_Sequence / 20;

            // 5-second background ping to prevent enterprise router timeout
            if ((total_seconds - last_ping_second) >= 5) {
                Transmit_UCA0_String("AT+PING=\"8.8.8.8\"\r\n");
                last_ping_second = total_seconds;
            }

            if (p10_state != 10 && p10_state != 103) {
                if (course_started == 0) {
                    if (calibration_done == 1) { strcpy(display_line[0], " Waiting  "); strcpy(display_line[1], "for input "); }
                    strcpy(display_line[2], (char *)ip_display1); strcpy(display_line[3], (char *)ip_display2);
                    display_changed = 1;
                }
                else {
                    if (p10_state == 0) {
                        // Clear the top line during manual operation
                        if (command_op == FORWARDS || command_op == BACK || command_op == LEFT || command_op == RIGHT) {
                            strcpy(display_line[0], "          ");
                        }
                        strcpy(display_line[1], (char *)ip_display1);
                    }
                    strcpy(display_line[2], (char *)ip_display2);
                    display_line[3][0] = last_cmd[0]; display_line[3][1] = last_cmd[1]; display_line[3][2] = last_cmd[2];
                    display_line[3][3] = last_cmd[3]; display_line[3][4] = last_cmd[4]; display_line[3][5] = ' ';
                    display_line[3][6] = (total_seconds / 100) % 10 + '0';
                    display_line[3][7] = (total_seconds / 10) % 10 + '0';
                    display_line[3][8] = (total_seconds % 10) + '0';
                    display_line[3][9] = 's';
                    display_changed = 1;
                }
            }

            if ((command_op == WAIT || command_op == ARRIVED || command_op == INTERCEPT) && response_parse != iot_rx_wr) {
                if (IOT_Ring_Rx[response_parse] == '^') {
                    buffer_count = (iot_rx_wr >= response_parse) ? (iot_rx_wr - response_parse) : (32 - (int)response_parse + (int)iot_rx_wr);
                    if (buffer_count >= 9) {
                        p1=(response_parse+1)%32; p2=(response_parse+2)%32; p3=(response_parse+3)%32; p4=(response_parse+4)%32;
                        dir=(response_parse+5)%32; t1=(response_parse+6)%32; t2=(response_parse+7)%32; t3=(response_parse+8)%32;

                        if (IOT_Ring_Rx[p1]=='1' && IOT_Ring_Rx[p2]=='2' && IOT_Ring_Rx[p3]=='2' && IOT_Ring_Rx[p4]=='1') {

                            last_cmd[0] = IOT_Ring_Rx[dir]; last_cmd[2] = IOT_Ring_Rx[t1]; last_cmd[3] = IOT_Ring_Rx[t2]; last_cmd[4] = IOT_Ring_Rx[t3];
                            run_time = 0;
                            command_op = IOT_Ring_Rx[dir];

                            if (command_op == ARRIVED) pad_number = IOT_Ring_Rx[t3];

                            // Extract the dynamic intercept angle identifier
                            if (command_op == INTERCEPT) intercept_type = IOT_Ring_Rx[t3] - '0';

                            setTime = ((unsigned int)(IOT_Ring_Rx[t1]-'0')*100) + ((unsigned int)(IOT_Ring_Rx[t2]-'0')*10) + (unsigned int)(IOT_Ring_Rx[t3]-'0');

                            if (course_started == 0) { course_started = 1; Time_Sequence = 0; }
                            response_parse = (response_parse + 9) % 32;
                        } else { response_parse = (response_parse + 1) % 32; }
                    }
                } else { response_parse = (response_parse + 1) % 32; }
            }

            switch (command_op) {
                case WAIT:     Wheels_All_Off(); break;
                case FORWARDS: Forward_On();   if (run_time >= setTime) command_op = WAIT; break;
                case BACK:     Reverse_On();   if (run_time >= setTime) command_op = WAIT; break;
                case RIGHT:    Spin_CW_On();   if (run_time >= setTime) command_op = WAIT; break;
                case LEFT:     Spin_CCW_On();  if (run_time >= setTime) command_op = WAIT; break;

                case ARRIVED:
                    if (pad_number == '8') {
                        Run_Project10();
                    } else {
                        Wheels_All_Off();
                        strcpy(display_line[0], "Arrived 0 ");
                        display_line[0][9] = pad_number;
                        display_changed = 1;
                    }
                    break;

                case INTERCEPT:
                case EXIT:
                    Run_Project10();
                    break;
            }
        }
        Display_Process();
    }
}






