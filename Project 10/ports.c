//===========================================================================
// File Name : ports.c
//
// Description: Port pin initialization and hardware interrupt setup.
//
// Author: Ezana
// Date: Feb 2026
// Compiler: Built with Code Composer Studio
//===========================================================================

#include "msp430.h"
#include "functions.h"
#include "LCD.h"
#include "ports.h"
#include "macros.h"

// Initialize all ports
void Init_Ports(void){
    Init_Port1();
    Init_Port2();
    Init_Port3();
    Init_Port4();
    Init_Port5();
    Init_Port6();
}


// Configure Port 1
void Init_Port1(void){
    P1DIR = 0x00;           // Set P1 direction to input
    P1OUT = 0x00;           // P1 set Low

    // Configure Red LED
    P1SEL0 &= ~RED_LED;     // Set RED_LED as GP I/O
    P1SEL1 &= ~RED_LED;     // Set RED_LED as GP I/O
    P1OUT |= RED_LED;       // Set Red LED On
    P1DIR |= RED_LED;       // Set Red LED direction to output

    // Configure ADC Analog Inputs using P1SELC
    P1SELC |= V_A1_SEEED;   // ADC input for A1_SEEED
    P1SELC |= V_DETECT_L;   // ADC input for V_DETECT_L
    P1SELC |= V_DETECT_R;   // ADC input for V_DETECT_R
    P1SELC |= V_A4_SEEED;   // ADC input for V_A4_SEEED
    P1SELC |= V_THUMB;      // ADC input for V_THUMB

    // Configure UART TX
    P1SEL0 |= UCA0TXD;      // UCA0TXD pin
    P1SEL1 &= ~UCA0TXD;     // UCA0TXD pin

    // Configure UART RX
    P1SEL0 |= UCA0RXD;      // UCA0RXD pin
    P1SEL1 &= ~UCA0RXD;     // UCA0RXD pin
}

// Configure Port 2 (Includes SW2 Interrupts)
void Init_Port2(void){

    P2SEL0 &= ~SLOW_CLK; // Select GPIO for slow clock
    P2SEL1 &= ~SLOW_CLK; // Complete GPIO selection
    P2OUT &= ~SLOW_CLK;  // Set initial output low
    P2DIR |= SLOW_CLK;   // Set slow clock as output

    P2SEL0 &= ~CHECK_BAT; // Select GPIO for battery check
    P2SEL1 &= ~CHECK_BAT; // Complete GPIO selection
    P2OUT &= ~CHECK_BAT;  // Set initial output low
    P2DIR |= CHECK_BAT;   // Set battery check as output

    P2SEL0 &= ~IR_LED; // Select GPIO for IR LED
    P2SEL1 &= ~IR_LED; // Complete GPIO selection
    P2OUT |= IR_LED;  // Set IR LED off
    P2DIR |= IR_LED;   // Set IR LED as output

    // SW2 Interrupt Configuration
    P2SEL0 &= ~SW2; // Select GPIO for Switch 2
    P2SEL1 &= ~SW2; // Complete GPIO selection
    P2OUT |= SW2;   // Configure pull-up resistor
    P2DIR &= ~SW2;  // Set Switch 2 as input
    P2REN |= SW2;   // Enable pull-up resistor
    P2IES |= SW2;   // Set high-to-low edge interrupt trigger
    P2IFG &= ~SW2;  // Clear interrupt flag
    P2IE |= SW2;    // Enable hardware interrupt for SW2

    P2SEL0 &= ~IOT_RUN_RED; // Select GPIO for IoT red run
    P2SEL1 &= ~IOT_RUN_RED; // Complete GPIO selection
    P2OUT &= ~IOT_RUN_RED;  // Set initial output low
    P2DIR |= IOT_RUN_RED;   // Set IoT red run as output

    P2SEL0 &= ~DAC_ENB; // Select GPIO for DAC enable
    P2SEL1 &= ~DAC_ENB; // Complete GPIO selection
    P2OUT &= ~DAC_ENB;
    P2DIR |= DAC_ENB;   // Set DAC enable as output

    P2SEL0 &= ~LFXOUT; // Select primary function for LFXOUT
    P2SEL1 |= LFXOUT;  // Complete primary function selection

    P2SEL0 &= ~LFXIN;  // Select primary function for LFXIN
    P2SEL1 |= LFXIN;   // Complete primary function selection
}

// Configure Port 3
void Init_Port3(void){

    P3SEL0 &= ~TEST_PROBE; // Select GPIO for test probe
    P3SEL1 &= ~TEST_PROBE; // Complete GPIO selection
    P3OUT &= ~TEST_PROBE;  // Set initial output low
    P3DIR |= TEST_PROBE;   // Set test probe as output

    P3SEL0 &= ~OA2O; // Select GPIO for op-amp output
    P3SEL1 &= ~OA2O; // Complete GPIO selection
    P3OUT &= ~OA2O;  // Set initial output low
    P3DIR &= ~OA2O;  // Set op-amp output as input

    P3SEL0 &= ~OA2N; // Select GPIO for op-amp negative input
    P3SEL1 &= ~OA2N; // Complete GPIO selection
    P3OUT &= ~OA2N;  // Set initial output low
    P3DIR &= ~OA2N;  // Set op-amp negative input as output

    P3SEL0 &= ~OA2P; // Select GPIO for op-amp positive input
    P3SEL1 &= ~OA2P; // Complete GPIO selection
    P3OUT &= ~OA2P;  // Set initial output low
    P3DIR &= ~OA2P;  // Set op-amp positive input as input

    P3SEL0 &= ~SMCLK; // Select GPIO for SMCLK output
    P3SEL1 &= ~SMCLK; // Complete GPIO selection
    P3OUT &= ~SMCLK;  // Set initial output low
    P3DIR &= ~SMCLK;  // Set SMCLK as input

    // P3 Pin 5
    P3SELC |= DAC_CNTL;  // DAC_CNTL DAC operation

    P3SEL0 &= ~IOT_LINK_CPU; // Select GPIO for IoT link
    P3SEL1 &= ~IOT_LINK_CPU; // Complete GPIO selection
    P3OUT &= ~IOT_LINK_CPU;  // Set initial output low
    P3DIR &= ~IOT_LINK_CPU;  // Set IoT link as input

    // =======================================================================
    // FIXED: IOT ENABLE PIN
    // The ESP32 must have this pin driven HIGH to turn on
    // =======================================================================
    P3SEL0 &= ~IOT_EN_CPU; // Select GPIO for IoT enable
    P3SEL1 &= ~IOT_EN_CPU; // Complete GPIO selection
    P3OUT |= IOT_EN_CPU;   // Set output HIGH (Turns ESP32 ON)
    P3DIR |= IOT_EN_CPU;   // Set direction to Output
}

// Configure Port 4 (Includes SW1 Interrupts)
void Init_Port4(void){

    P4SEL0 &= ~RESET_LCD; // Select GPIO for LCD reset
    P4SEL1 &= ~RESET_LCD; // Complete GPIO selection
    P4OUT &= ~RESET_LCD;  // Set LCD reset low
    P4DIR |= RESET_LCD;   // Set LCD reset as output

    // SW1 Interrupt Configuration
    P4SEL0 &= ~SW1; // Select GPIO for Switch 1
    P4SEL1 &= ~SW1; // Complete GPIO selection
    P4OUT |= SW1;   // Configure pull-up resistor
    P4DIR &= ~SW1;  // Set Switch 1 as input
    P4REN |= SW1;   // Enable pull-up resistor
    P4IES |= SW1;   // Set high-to-low edge interrupt trigger
    P4IFG &= ~SW1;  // Clear interrupt flag
    P4IE |= SW1;    // Enable hardware interrupt for SW1

    P4SEL0 |= UCA1TXD;  // Select primary function for UART TX
    P4SEL1 &= ~UCA1TXD; // Complete primary function selection

    P4SEL0 |= UCA1RXD;  // Select primary function for UART RX
    P4SEL1 &= ~UCA1RXD; // Complete primary function selection

    P4SEL0 &= ~UCB1_CS_LCD; // Select GPIO for LCD chip select
    P4SEL1 &= ~UCB1_CS_LCD; // Complete GPIO selection
    P4OUT |= UCB1_CS_LCD;   // Set chip select high (disabled)
    P4DIR |= UCB1_CS_LCD;   // Set chip select as output

    P4SEL0 |= UCB1CLK;  // Select primary function for SPI clock
    P4SEL1 &= ~UCB1CLK; // Complete primary function selection

    P4SEL0 |= UCB1SIMO;  // Select primary function for SPI MOSI
    P4SEL1 &= ~UCB1SIMO; // Complete primary function selection

    P4SEL0 |= UCB1SOMI;  // Select primary function for SPI MISO
    P4SEL1 &= ~UCB1SOMI; // Complete primary function selection
}

// Configure Port 5
void Init_Port5(void){
    //------------------------------------------------------------------------------
    // Port 5 Pins
    // V_BAT            (0x01) // 0 V_BAT
    // V_5_0            (0x02) // 1 V_5_0
    // V_DAC            (0x04) // 2 V_DAC
    // V_3_3            (0x08) // 3 V_3.3
    // IOT_BOOT         (0x10) // 4 IOT_BOOT
    //------------------------------------------------------------------------------

    P5DIR = 0x00;           // Set P5 direction to input
    P5OUT = 0x00;           // P5 set Low

    // Configure Analog Inputs using P5SELC
    P5SELC |= V_BAT;        // ADC input for V_BAT
    P5SELC |= V_5_0;        // ADC input for V_5_0
    P5SELC |= V_DAC;        // ADC input for V_DAC
    P5SELC |= V_3_3;        // ADC input for V_3_3

    // Configure IOT_BOOT as Digital Output
    P5SEL0 &= ~IOT_BOOT;    // IOT_BOOT GPIO operation
    P5SEL1 &= ~IOT_BOOT;    // IOT_BOOT GPIO operation
    P5OUT |= IOT_BOOT;      // Set Output value inactive (High)
    P5DIR |= IOT_BOOT;      // Set direction to output
}

// Configure Port 6
void Init_Port6(void){
    // Map Output to Timer B3 for LCD Backlight PWM
    P6SEL0 |= LCD_BACKLITE;     // Select primary peripheral function
    P6SEL1 &= ~LCD_BACKLITE;    // Complete peripheral selection
    P6DIR |= LCD_BACKLITE;      // Set direction to output

    // Map Output to Timer B3 for Right Forward PWM
    P6SEL0 |= R_FORWARD;        // Select primary peripheral function
    P6SEL1 &= ~R_FORWARD;       // Complete peripheral selection
    P6DIR |= R_FORWARD;         // Set direction to output

    // Map Output to Timer B3 for Left Forward PWM
    P6SEL0 |= L_FORWARD;        // Select primary peripheral function
    P6SEL1 &= ~L_FORWARD;       // Complete peripheral selection
    P6DIR |= L_FORWARD;         // Set direction to output

    // Map Output to Timer B3 for Right Reverse PWM
    P6SEL0 |= R_REVERSE;        // Select primary peripheral function
    P6SEL1 &= ~R_REVERSE;       // Complete peripheral selection
    P6DIR |= R_REVERSE;         // Set direction to output

    // Map Output to Timer B3 for Left Reverse PWM
    P6SEL0 |= L_REVERSE;        // Select primary peripheral function
    P6SEL1 &= ~L_REVERSE;       // Complete peripheral selection
    P6DIR |= L_REVERSE;         // Set direction to output

    // Configure P6.5 as General Purpose I/O
    P6SEL0 &= ~P6_5;            // Select GPIO function
    P6SEL1 &= ~P6_5;            // Complete GPIO selection
    P6OUT &= ~P6_5;             // Set initial output low
    P6DIR &= ~P6_5;             // Set direction to input

    // Configure Green LED as General Purpose I/O
    P6SEL0 &= ~GRN_LED;         // Select GPIO function
    P6SEL1 &= ~GRN_LED;         // Complete GPIO selection
    P6OUT &= ~GRN_LED;          // Set initial output low (LED off)
    P6DIR |= GRN_LED;           // Set direction to output
}
