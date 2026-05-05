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
    P1OUT = 0x00;       // Clear Port 1 output registers
    P1DIR = 0x01;       // Set pin 0 to output, others to input

    P1SEL0 &= ~RED_LED; // Select GPIO for Red LED
    P1SEL1 &= ~RED_LED; // Complete GPIO selection
    P1OUT &= ~RED_LED;  // Set Red LED off
    P1DIR |= RED_LED;   // Set Red LED as output

    P1SEL0 &= ~A1_SEEED; // Select GPIO for A1 SEEED
    P1SEL1 &= ~A1_SEEED; // Complete GPIO selection
    P1OUT &= ~A1_SEEED;  // Set initial output low
    P1DIR &= ~A1_SEEED;  // Set A1 SEEED as input

    P1SEL0 &= ~V_DETECT_L; // Select GPIO for left voltage detect
    P1SEL1 &= ~V_DETECT_L; // Complete GPIO selection
    P1OUT &= ~V_DETECT_L;  // Set initial output low
    P1DIR &= ~V_DETECT_L;  // Set left voltage detect as input

    P1SEL0 &= ~V_DETECT_R; // Select GPIO for right voltage detect
    P1SEL1 &= ~V_DETECT_R; // Complete GPIO selection
    P1OUT &= ~V_DETECT_R;  // Set initial output low
    P1DIR &= ~V_DETECT_R;  // Set right voltage detect as input

    P1SEL0 &= ~A4_SEEED; // Select GPIO for A4 SEEED
    P1SEL1 &= ~A4_SEEED; // Complete GPIO selection
    P1OUT &= ~A4_SEEED;  // Set initial output low
    P1DIR &= ~A4_SEEED;  // Set A4 SEEED as input

    P1SEL0 &= ~V_THUMB; // Select GPIO for thumbwheel
    P1SEL1 &= ~V_THUMB; // Complete GPIO selection
    P1OUT &= ~V_THUMB;  // Set initial output low
    P1DIR &= ~V_THUMB;  // Set thumbwheel as input

    P1SEL0 &= ~UCA0RXD; // Select GPIO for UART RX
    P1SEL1 &= ~UCA0RXD; // Complete GPIO selection
    P1OUT &= ~UCA0RXD;  // Set initial output low
    P1DIR &= ~UCA0RXD;  // Set UART RX as input

    P1SEL0 &= ~UCA0TXD; // Select GPIO for UART TX
    P1SEL1 &= ~UCA0TXD; // Complete GPIO selection
    P1OUT &= ~UCA0TXD;  // Set initial output low
    P1DIR &= ~UCA0TXD;  // Set UART TX as input
}

// Configure Port 2 (Includes SW2 Interrupts)
void Init_Port2(void){
    P2OUT = 0x00; // Clear Port 2 output registers
    P2DIR = 0x00; // Default all Port 2 pins to input

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
    P2OUT &= ~IR_LED;  // Set IR LED off
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
    P2OUT |= DAC_ENB;   // Set DAC enable high initially
    P2DIR |= DAC_ENB;   // Set DAC enable as output

    P2SEL0 &= ~LFXOUT; // Select primary function for LFXOUT
    P2SEL1 |= LFXOUT;  // Complete primary function selection

    P2SEL0 &= ~LFXIN;  // Select primary function for LFXIN
    P2SEL1 |= LFXIN;   // Complete primary function selection
}

// Configure Port 3
void Init_Port3(void){
    P3OUT = 0x00; // Clear Port 3 output registers
    P3DIR = 0x01; // Set pin 0 to output, others to input

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

    P3SEL0 &= ~DAC_CNTL; // Select GPIO for DAC control
    P3SEL1 &= ~DAC_CNTL; // Complete GPIO selection
    P3OUT &= ~DAC_CNTL;  // Set initial output low
    P3DIR &= ~DAC_CNTL;  // Set DAC control as input

    P3SEL0 &= ~IOT_LINK_CPU; // Select GPIO for IoT link
    P3SEL1 &= ~IOT_LINK_CPU; // Complete GPIO selection
    P3OUT &= ~IOT_LINK_CPU;  // Set initial output low
    P3DIR &= ~IOT_LINK_CPU;  // Set IoT link as input

    P3SEL0 &= ~IOT_EN_CPU; // Select GPIO for IoT enable
    P3SEL1 &= ~IOT_EN_CPU; // Complete GPIO selection
    P3OUT &= ~IOT_EN_CPU;  // Set initial output low
    P3DIR &= ~IOT_EN_CPU;  // Set IoT enable as input
}

// Configure Port 4 (Includes SW1 Interrupts)
void Init_Port4(void){
    P4OUT = 0x00; // Clear Port 4 output registers
    P4DIR = 0x00; // Default all Port 4 pins to input

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
    P5OUT = 0x00; // Clear Port 5 output registers
    P5DIR = 0x01; // Set pin 0 to output, others to input

    P5SEL0 &= ~V_BAT; // Select GPIO for battery voltage pin
    P5SEL1 &= ~V_BAT; // Complete GPIO selection
    P5OUT &= ~V_BAT;  // Set initial output low
    P5DIR &= ~V_BAT;  // Set battery voltage pin as input

    P5SEL0 &= ~V_5; // Select GPIO for 5V sense pin
    P5SEL1 &= ~V_5; // Complete GPIO selection
    P5OUT &= ~V_5;  // Set initial output low
    P5DIR &= ~V_5;  // Set 5V sense pin as input

    P5SEL0 &= ~V_DAC; // Select GPIO for DAC voltage pin
    P5SEL1 &= ~V_DAC; // Complete GPIO selection
    P5OUT &= ~V_DAC;  // Set initial output low
    P5DIR &= ~V_DAC;  // Set DAC voltage pin as input

    P5SEL0 &= ~V_3_3; // Select GPIO for 3.3V sense pin
    P5SEL1 &= ~V_3_3; // Complete GPIO selection
    P5OUT &= ~V_3_3;  // Set initial output low
    P5DIR &= ~V_3_3;  // Set 3.3V sense pin as input

    P5SEL0 &= ~IOT_BOOT_CPU; // Select GPIO for IoT boot pin
    P5SEL1 &= ~IOT_BOOT_CPU; // Complete GPIO selection
    P5OUT &= ~IOT_BOOT_CPU;  // Set initial output low
    P5DIR &= ~IOT_BOOT_CPU;  // Set IoT boot pin as input
}

// Configure Port 6
void Init_Port6(void){
    P6OUT = 0x00; // Clear Port 6 output registers
    P6DIR = 0x00; // Default all Port 6 pins to input

    P6SEL0 &= ~LCD_BACKLITE; // Select GPIO for LCD backlight
    P6SEL1 &= ~LCD_BACKLITE; // Complete GPIO selection
    P6OUT |= LCD_BACKLITE;  // Set backlight off initially
    P6DIR |= LCD_BACKLITE;   // Set backlight pin as output

    P6SEL0 &= ~R_FORWARD; // Select GPIO for right forward
    P6SEL1 &= ~R_FORWARD; // Complete GPIO selection
    P6OUT &= ~R_FORWARD;  // Set motor off initially
    P6DIR |= R_FORWARD;   // Set right forward pin as output

    P6SEL0 &= ~L_FORWARD; // Select GPIO for left forward
    P6SEL1 &= ~L_FORWARD; // Complete GPIO selection
    P6OUT &= ~L_FORWARD;  // Set motor off initially
    P6DIR |= L_FORWARD;   // Set left forward pin as output

    P6SEL0 &= ~R_REVERSE; // Select GPIO for right reverse
    P6SEL1 &= ~R_REVERSE; // Complete GPIO selection
    P6OUT &= ~R_REVERSE;  // Set motor off initially
    P6DIR |= R_REVERSE;   // Set right reverse pin as output

    P6SEL0 &= ~L_REVERSE; // Select GPIO for left reverse
    P6SEL1 &= ~L_REVERSE; // Complete GPIO selection
    P6OUT &= ~L_REVERSE;  // Set motor off initially
    P6DIR |= L_REVERSE;   // Set left reverse pin as output

    P6SEL0 &= ~P6_5; // Select GPIO for pin 6.5
    P6SEL1 &= ~P6_5; // Complete GPIO selection
    P6OUT &= ~P6_5;  // Set initial output low
    P6DIR &= ~P6_5;  // Leave pin 6.5 as input

    P6SEL0 &= ~GRN_LED; // Select GPIO for Green LED
    P6SEL1 &= ~GRN_LED; // Complete GPIO selection
    P6OUT &= ~GRN_LED;  // Set Green LED off initially
    P6DIR |= GRN_LED;   // Set Green LED pin as output
}
