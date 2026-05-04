//------------------------------------------------------------------------------
//  NC State University ECE 306
//  Instructor: Jim Carlson
//  Homework #03
//
//  File Name: ports.c
//  Author: Ezana Enquobahrie
//  Date: February 5, 2026
//  Compiler: Code Composer Studio 12.8.1
//
//  Description:
//  Port configuration routines for MSP430FR2355 microcontroller.
//  Individual port setup will be completed during the ports assignment.
//
//  Global Variables Defined:
//  None
//------------------------------------------------------------------------------

#include  "msp430.h"
#include  <string.h>
#include  "include/functions.h"
#include  "Include/LCD.h"
#include  "include/ports.h"
#include  "macros.h"

//------------------------------------------------------------------------------
//  Function Name: Init_Ports
//
//  Description:
//  Executes initialization routines for all port modules.
//
//  Globals Used:
//  None
//
//  Globals Modified:
//  None
//
//  Passed Variables:
//  None
//
//  Local Variables:
//  None
//
//  Returned Value:
//  None
//------------------------------------------------------------------------------

void Init_Ports(void){
  Init_Port_1();
  Init_Port_2();
  Init_Port_3();
  Init_Port_4();
  Init_Port_5();
  Init_Port_6();
}



void Init_Port_1(void) {
    // P1.0 - RedLED: General purpose digital I/O configured as output with low initial state
    P1SEL0 &= ~RED_LED; // Configure as standard GPIO
    P1SEL1 &= ~RED_LED; // Configure as standard GPIO
    P1OUT  &= ~RED_LED; // Start in off state
    P1DIR  |=  RED_LED; // Set as output pin

    // P1.1 - A1_SEEED: Special function mode for analog-to-digital conversion
    P1SEL0 |=  A1_SEEED; // Enable special function mode
    P1SEL1 |=  A1_SEEED; // Enable special function mode

    // P1.2 - V_DETECT_L: ADC input for left voltage detection
    P1SEL0 |=  V_DETECT_L; // Switch to peripheral mode
    P1SEL1 |=  V_DETECT_L; // Switch to peripheral mode

    // P1.3 - V_DETECT_R: ADC input for right voltage detection
    P1SEL0 |=  V_DETECT_R; // Activate function select
    P1SEL1 |=  V_DETECT_R; // Activate function select

    // P1.4 - A4_SEEED: Analog input configured for ADC operation
    P1SEL0 |=  A4_SEEED; // Enable peripheral function
    P1SEL1 |=  A4_SEEED; // Enable peripheral function

    // P1.5 - V_THUMB: Thumbwheel voltage input via ADC
    P1SEL0 |=  V_THUMB;    // Select alternate function
    P1SEL1 |=  V_THUMB;    // Select alternate function

    // P1.6 - UCA0RXD: Serial receive line for UART module A0
    P1SEL0 |=  UCA0RXD;    // Configure for UART reception
    P1SEL1 &= ~UCA0RXD;    // Configure for UART reception

    // P1.7 - UCA0TXD: Serial transmit line for UART module A0
    P1SEL0 |=  UCA0TXD;    // Configure for UART transmission
    P1SEL1 &= ~UCA0TXD;    // Configure for UART transmission

}

void Init_Port_2(void){           // Port 2 setup routine
//------------------------------------------------------------------------------
  P2OUT = 0x00;                  // Initialize all pins low
  P2DIR = 0x00;                  // Default all pins as outputs

  P2SEL0 &= ~SLOW_CLK;           // SLOW_CLK in GPIO mode
  P2SEL1 &= ~SLOW_CLK;           // SLOW_CLK in GPIO mode
  P2OUT  &= ~SLOW_CLK;           // Begin in low state
  P2DIR  |=  SLOW_CLK;           // Configure as output

  P2SEL0 &= ~CHECK_BAT;          // CHECK_BAT standard I/O mode
  P2SEL1 &= ~CHECK_BAT;          // CHECK_BAT standard I/O mode
  P2OUT  &= ~CHECK_BAT;          // Start disabled (low)
  P2DIR  |=  CHECK_BAT;           // Set as output

  P2SEL0 &= ~IR_LED;             // P2_2 in digital I/O mode
  P2SEL1 &= ~IR_LED;             // P2_2 in digital I/O mode
  P2OUT  &= ~IR_LED;             // Begin deactivated
  P2DIR  |=  IR_LED;             // Configure as output

  P2SEL0 &= ~SW2;                // SW2 button configuration
  P2SEL1 &= ~SW2;                // SW2 button configuration
  P2OUT  |=  SW2;                // Enable internal pull-up
  P2DIR  &= ~SW2;                // Set as input
  P2REN  |=  SW2;                // Activate resistor

  P2SEL0 &= ~IOT_RUN_RED;        // IOT_RUN_CPU digital output
  P2SEL1 &= ~IOT_RUN_RED;        // IOT_RUN_CPU digital output
  P2OUT  &= ~IOT_RUN_RED;        // Start in inactive state
  P2DIR  |=  IOT_RUN_RED;        // Configure as output

  P2SEL0 &= ~DAC_ENB;            // DAC_ENB control pin setup
  P2SEL1 &= ~DAC_ENB;            // DAC_ENB control pin setup
  P2OUT  |=  DAC_ENB;            // Begin enabled (high)
  P2DIR  |=  DAC_ENB;            // Set as output

  P2SEL0 &= ~LFXOUT;             // LFXOUT crystal oscillator output
  P2SEL1 |=  LFXOUT;             // LFXOUT crystal oscillator output

  P2SEL0 &= ~LFXIN;              // LFXIN crystal oscillator input
  P2SEL1 |=  LFXIN;              // LFXIN crystal oscillator input
//------------------------------------------------------------------------------

}


void Init_Port_3(void) {
    // P3.0 - TEST_PROBE: Digital output for test/debug purposes, starts low
    P3SEL0 &= ~TEST_PROBE;    // Standard I/O configuration
    P3SEL1 &= ~TEST_PROBE;    // Standard I/O configuration
    P3OUT  &= ~TEST_PROBE;    // Begin in low state
    P3DIR  |=  TEST_PROBE;    // Set pin direction to output

    // P3.1 - OA2O: Operational amplifier 2 output in peripheral mode
    P3SEL0 |=  OA2O;          // Switch to function mode
    P3SEL1 |=  OA2O;          // Switch to function mode

    // P3.2 - OA2N: Operational amplifier 2 negative input terminal
    P3SEL0 |=  OA2N;          // Activate peripheral function
    P3SEL1 |=  OA2N;          // Activate peripheral function

    // P3.3 - OA2P: Operational amplifier 2 positive input terminal
    P3SEL0 |=  OA2P;          // Enable module function
    P3SEL1 |=  OA2P;          // Enable module function

    // P3.4 - SMCLK: Sub-main clock output signal
    P3SEL0 |=  SMCLK;     // Select clock function
    P3SEL1 &= ~SMCLK;     // Select clock function

    // P3.5 - DAC_CNTL: Digital-to-analog converter control signal, output low
    P3SEL0 &= ~DAC_CNTL;      // Configure as GPIO
    P3SEL1 &= ~DAC_CNTL;      // Configure as GPIO
    P3OUT  &= ~DAC_CNTL;      // Start deactivated
    P3DIR  |=  DAC_CNTL;      // Direction configured as output

    // P3.6 - IOT_LINK_GRN: IoT module link status LED (green), output low
    P3SEL0 &= ~IOT_LINK_GRN;  // Standard digital I/O
    P3SEL1 &= ~IOT_LINK_GRN;  // Standard digital I/O
    P3OUT  &= ~IOT_LINK_GRN;  // LED starts off
    P3DIR  |=  IOT_LINK_GRN;  // Output mode enabled

    // P3.7 - IOT_EN: IoT module enable control, active high output
    P3SEL0 &= ~IOT_EN;        // GPIO mode selected
    P3SEL1 &= ~IOT_EN;        // GPIO mode selected
    P3OUT  &= ~IOT_EN;        // Module disabled initially
    P3DIR  |=  IOT_EN;        // Configured as output

}


void Init_Port_4(void){           // Port 4 initialization sequence
//------------------------------------------------------------------------------

  P4OUT = 0x00;                  // Clear all output latches
  P4DIR = 0x00;                  // Initialize direction register

  P4SEL0 &= ~RESET_LCD;          // RESET_LCD in standard I/O mode
  P4SEL1 &= ~RESET_LCD;          // RESET_LCD in standard I/O mode
  P4OUT  &= ~RESET_LCD;          // Assert reset (active low)
  P4DIR  |=  RESET_LCD;          // Output configuration

  P4SEL0 &= ~SW1;                // SW1 pushbutton setup
  P4SEL1 &= ~SW1;                // SW1 pushbutton setup

  P4OUT  |=  SW1;                // Internal pull-up enabled
  P4DIR  &= ~SW1;                // Input mode
  P4REN  |=  SW1;                // Resistor enabled



  P4SEL0 |=  UCA1TXD;            // UART A1 transmit function
  P4SEL1 &= ~UCA1TXD;            // UART A1 transmit function

  P4SEL0 |=  UCA1RXD;            // UART A1 receive function
  P4SEL1 &= ~UCA1RXD;            // UART A1 receive function

  P4SEL0 &= ~UCB1_CS_LCD;        // LCD chip select as GPIO
  P4SEL1 &= ~UCB1_CS_LCD;        // LCD chip select as GPIO
  P4OUT  |=  UCB1_CS_LCD;        // Deselect LCD (active low)
  P4DIR  |=  UCB1_CS_LCD;        // Output mode for chip select

  P4SEL0 |=  UCB1CLK;            // SPI clock line function
  P4SEL1 &= ~UCB1CLK;            // SPI clock line function



  P4SEL0 |=  UCB1SIMO;           // SPI master-out slave-in
  P4SEL1 &= ~UCB1SIMO;           // SPI master-out slave-in

  P4SEL0 |=  UCB1SOMI;           // SPI master-in slave-out
  P4SEL1 &= ~UCB1SOMI;           // SPI master-in slave-out

//------------------------------------------------------------------------------

}



void Init_Port_5(void) {

    // P5.0 - V_BAT: Battery voltage monitoring via ADC peripheral
    P5SEL0 |=  V_BAT;         // Peripheral mode enabled
    P5SEL1 |=  V_BAT;         // Peripheral mode enabled
    // Note: Output and direction registers not applicable for analog functions

    // P5.1 - V_5_0: 5V rail voltage sensing through ADC
    P5SEL0 |=  V_5;         // Function select active
    P5SEL1 |=  V_5;         // Function select active

    // P5.2 - V_DAC: DAC voltage output monitoring input
    P5SEL0 |=  V_DAC;         // Analog function enabled
    P5SEL1 |=  V_DAC;         // Analog function enabled

    // P5.3 - V_3_3: 3.3V supply voltage measurement
    P5SEL0 |=  V_3_3;         // ADC channel configured
    P5SEL1 |=  V_3_3;         // ADC channel configured

    // P5.4 - IOT_BOOT_CPU: Boot control signal for IoT processor, output low
    P5SEL0 &= ~IOT_BOOT_CPU;  // Digital I/O mode
    P5SEL1 &= ~IOT_BOOT_CPU;  // Digital I/O mode
    P5OUT  &= ~IOT_BOOT_CPU;  // Normal operation mode (low)
    P5DIR  |=  IOT_BOOT_CPU;  // Output pin configuration

}





void Init_Port_6(void) {

    // P6.0 - LCD_BACKLITE: Display backlight control output, initially off
    P6SEL0 &= ~LCD_BACKLITE;  // GPIO mode active
    P6SEL1 &= ~LCD_BACKLITE;  // GPIO mode active
    P6OUT  |=  LCD_BACKLITE;  // Backlight disabled
    P6DIR  |=  LCD_BACKLITE;  // Output direction set



    // P6.1 - R_FORWARD: Right motor forward PWM signal
    P6SEL0 &= ~R_FORWARD;     // Set Select 0 for GP I/O
    P6SEL1 &= ~R_FORWARD;     // Set Select 1 for GP I/O
    P6OUT  &= ~R_FORWARD;     // Set pin LOW [Wheel Off]
    P6DIR  |=  R_FORWARD;     // Set Direction to Output


    // P6.2 - L_FORWARD: Left motor forward PWM signal

    P6SEL0 &= ~L_FORWARD;     // GPIO
    P6SEL1 &= ~L_FORWARD;     // GPIO
    P6OUT  &= ~L_FORWARD;     // LOW [Wheel Off]
    P6DIR  |=  L_FORWARD;     // Output


    // P6.3 - R_REVERSE: Right motor reverse PWM signal
    P6SEL0 &= ~R_REVERSE;     // GPIO
    P6SEL1 &= ~R_REVERSE;     // GPIO
    P6OUT  &= ~R_REVERSE;     // LOW [Wheel Off]
    P6DIR  |=  R_REVERSE;     // Output


    // P6.4 - L_REVERSE: Left motor reverse PWM signal
    P6SEL0 &= ~L_REVERSE;     // GPIO
    P6SEL1 &= ~L_REVERSE;     // GPIO
    P6OUT  &= ~L_REVERSE;     // LOW [Wheel Off]
    P6DIR  |=  L_REVERSE;     // Output



    // P6.5 - P6_5: General purpose output, initially low
    P6SEL0 &= ~P6_5;          // Standard GPIO mode
    P6SEL1 &= ~P6_5;          // Standard GPIO mode
    P6OUT  &= ~P6_5;          // Low output state
    P6DIR  |=  P6_5;          // Output configured

    // P6.6 - GRN_LED: Green status LED output, begins off
    P6SEL0 &= ~GRN_LED;       // Digital I/O selected
    P6SEL1 &= ~GRN_LED;       // Digital I/O selected
    P6OUT  &= ~GRN_LED;       // LED off initially
    P6DIR  |=  GRN_LED;       // Output mode enabled

    // Ensure ALL wheel directions are off
    P6OUT &= ~L_FORWARD;
    P6OUT &= ~R_FORWARD;
    P6OUT &= ~L_REVERSE;
    P6OUT &= ~R_REVERSE;

}


//------------------------------------------------------------------------------
// Motor Control Functions
//------------------------------------------------------------------------------

void Motors_All_Off(void){
  P6OUT &= ~L_FORWARD;
  P6OUT &= ~R_FORWARD;
  P6OUT &= ~L_REVERSE;
  P6OUT &= ~R_REVERSE;
}

void Motors_Forward_On(void){
  Motors_All_Off();          // Safety: never switch directions without off
  P6OUT |=  L_FORWARD;
  P6OUT |=  R_FORWARD;
}

void Motors_Reverse_On(void){
  Motors_All_Off();          // Safety: never switch directions without off
  P6OUT |=  L_REVERSE;
  P6OUT |=  R_REVERSE;
}

