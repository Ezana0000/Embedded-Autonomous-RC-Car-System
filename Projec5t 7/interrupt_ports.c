//===========================================================================
// File Name : interrupt_ports.c
//===========================================================================
#include "msp430.h"
#include <string.h>
#include "macros.h"
#include "ports.h"

extern char display_line[4][11];
extern volatile unsigned char display_changed;

extern char p5_started;
extern unsigned int p5_step;
extern volatile unsigned int p5_timer;
extern volatile unsigned int p5_running;
extern char step_init;
void Wheels_All_Off(void);

volatile unsigned int sw1_debounce_count = 0;
volatile unsigned int sw2_debounce_count = 0;
volatile char sw1_debounce_active = 0;
volatile char sw2_debounce_active = 0;

extern unsigned int cal_step;
extern unsigned int Black_Left_Val;
extern unsigned int Black_Right_Val;
extern unsigned int Left_Black_To_Gray;
extern unsigned int Right_Black_To_Gray;
extern unsigned int Left_Gray_To_White;
extern unsigned int Right_Gray_To_White;

extern volatile unsigned int ADC_Left;
extern volatile unsigned int ADC_Right;

// SW1 Interrupt
#pragma vector = PORT4_VECTOR
__interrupt void switchP4_interrupt(void){
    if (P4IFG & SW1) {
        P4IFG &= ~SW1;
        P4IE &= ~SW1;

        sw1_debounce_active = 1;
        sw1_debounce_count = 0;
        P6OUT &= ~LCD_BACKLITE;

        if (!p5_started) {
            if (cal_step == 0) {
                // STEP 1: Save Black Values and calculate the Gray threshold
                Black_Left_Val = ADC_Left;
                Black_Right_Val = ADC_Right;

                Left_Black_To_Gray = Black_Left_Val - 75;
                Right_Black_To_Gray = Black_Right_Val - 75;

                cal_step = 1;
            }
            else if (cal_step == 2) {
                // STEP 3: Start Project 7
                p5_step = P7_WAIT;
                p5_started = 1;
                step_init = 1;
                p5_timer = 0;
                p5_running = 0;
            }
        }
    }
}

// SW2 Interrupt
#pragma vector = PORT2_VECTOR
__interrupt void switchP2_interrupt(void){
    if (P2IFG & SW2) {
        P2IFG &= ~SW2;
        P2IE &= ~SW2;

        sw2_debounce_active = 1;
        sw2_debounce_count = 0;
        P6OUT &= ~LCD_BACKLITE;

        if (!p5_started) {
            if (cal_step == 1) {
                // STEP 2: Save White thresholds (Black minus 200)
                Left_Gray_To_White = Black_Left_Val - 200;
                Right_Gray_To_White = Black_Right_Val - 200;

                cal_step = 2;
            }
        } else {
            // Emergency Stop
            Wheels_All_Off();
            p5_started = 0;
            p5_running = 0;
            p5_step = 0;
            cal_step = 0; // Reset calibration sequence
        }
    }
}
