//------------------------------------------------------------------------------
//  NC State University ECE 306
//  Instructor: Jim Carlson
//  Homework #03
//
//  File Name: ports.c
//  Author: Ezana Enquobahrie
//  Date: February 5, 2026
//------------------------------------------------------------------------------
#include  "msp430.h"
#include  "macros.h"
#include  "include/ports.h"

void Init_Ports(void){
  Init_Port_1();
  Init_Port_2();
  Init_Port_3(USE_GPIO);
  Init_Port_4();
  Init_Port_5();
  Init_Port_6();
}

void Init_Port_1(void) {
    P1SEL0 &= ~RED_LED; P1SEL1 &= ~RED_LED; P1OUT &= ~RED_LED; P1DIR |= RED_LED;
    P1SEL0 |=  A1_SEEED; P1SEL1 |=  A1_SEEED;
    P1SEL0 |=  V_DETECT_L; P1SEL1 |=  V_DETECT_L;
    P1SEL0 |=  V_DETECT_R; P1SEL1 |=  V_DETECT_R;
    P1SEL0 |=  A4_SEEED; P1SEL1 |=  A4_SEEED;
    P1SEL0 |=  V_THUMB; P1SEL1 |=  V_THUMB;
    P1SEL0 |=  UCA0RXD; P1SEL1 &= ~UCA0RXD;
    P1SEL0 |=  UCA0TXD; P1SEL1 &= ~UCA0TXD;
}

void Init_Port_2(void){
  P2OUT = 0x00; P2DIR = 0x00;
  P2SEL0 &= ~SLOW_CLK; P2SEL1 &= ~SLOW_CLK; P2OUT &= ~SLOW_CLK; P2DIR |= SLOW_CLK;
  P2SEL0 &= ~CHECK_BAT; P2SEL1 &= ~CHECK_BAT; P2OUT &= ~CHECK_BAT; P2DIR |= CHECK_BAT;
  P2SEL0 &= ~IR_LED; P2SEL1 &= ~IR_LED; P2OUT &= ~IR_LED; P2DIR |= IR_LED;
  P2SEL0 &= ~SW2; P2SEL1 &= ~SW2; P2OUT |= SW2; P2DIR &= ~SW2; P2REN |= SW2;
  P2SEL0 &= ~IOT_RUN_RED; P2SEL1 &= ~IOT_RUN_RED; P2OUT &= ~IOT_RUN_RED; P2DIR |= IOT_RUN_RED;
  P2SEL0 &= ~DAC_ENB; P2SEL1 &= ~DAC_ENB; P2OUT |= DAC_ENB; P2DIR |= DAC_ENB;
  P2SEL0 &= ~LFXOUT; P2SEL1 |=  LFXOUT;
  P2SEL0 &= ~LFXIN; P2SEL1 |=  LFXIN;
}

void Init_Port_3(unsigned char smclk_mode) {
    P3OUT = 0x00; P3DIR = 0x01;
    P3SEL0 &= ~TEST_PROBE; P3SEL1 &= ~TEST_PROBE; P3OUT &= ~TEST_PROBE; P3DIR |= TEST_PROBE;
    P3SEL0 |=  OA2O; P3SEL1 |=  OA2O;
    P3SEL0 |=  OA2N; P3SEL1 |=  OA2N;
    P3SEL0 |=  OA2P; P3SEL1 |=  OA2P;

    // THOMAS GILBERT LOGIC: Route SMCLK to Pin 3.4
    if (smclk_mode == USE_SMCLK) {
        P3SEL0 |=  SMCLK;
        P3SEL1 &= ~SMCLK;
        P3DIR  |=  SMCLK;
    } else {
        P3SEL0 &= ~SMCLK;
        P3SEL1 &= ~SMCLK;
        P3OUT  &= ~SMCLK;
        P3DIR  |=  SMCLK;
    }

    P3SEL0 &= ~DAC_CNTL; P3SEL1 &= ~DAC_CNTL; P3OUT &= ~DAC_CNTL; P3DIR &= ~DAC_CNTL;
    P3SEL0 &= ~IOT_LINK_CPU; P3SEL1 &= ~IOT_LINK_CPU; P3OUT &= ~IOT_LINK_CPU; P3DIR &= ~IOT_LINK_CPU;
    P3SEL0 &= ~IOT_EN_CPU; P3SEL1 &= ~IOT_EN_CPU; P3OUT &= ~IOT_EN_CPU; P3DIR &= ~IOT_EN_CPU;
}

void Init_Port_4(void){
  P4OUT = 0x00; P4DIR = 0x00;
  P4SEL0 &= ~RESET_LCD; P4SEL1 &= ~RESET_LCD; P4OUT &= ~RESET_LCD; P4DIR |= RESET_LCD;
  P4SEL0 &= ~SW1; P4SEL1 &= ~SW1; P4OUT |= SW1; P4DIR &= ~SW1; P4REN |= SW1;
  P4SEL0 |=  UCA1TXD; P4SEL1 &= ~UCA1TXD;
  P4SEL0 |=  UCA1RXD; P4SEL1 &= ~UCA1RXD;
  P4SEL0 &= ~UCB1_CS_LCD; P4SEL1 &= ~UCB1_CS_LCD; P4OUT |= UCB1_CS_LCD; P4DIR |= UCB1_CS_LCD;
  P4SEL0 |=  UCB1CLK; P4SEL1 &= ~UCB1CLK;
  P4SEL0 |=  UCB1SIMO; P4SEL1 &= ~UCB1SIMO;
  P4SEL0 |=  UCB1SOMI; P4SEL1 &= ~UCB1SOMI;
}

void Init_Port_5(void) {
    P5SEL0 |=  V_BAT; P5SEL1 |=  V_BAT;
    P5SEL0 |=  V_5; P5SEL1 |=  V_5;
    P5SEL0 |=  V_DAC; P5SEL1 |=  V_DAC;
    P5SEL0 |=  V_3_3; P5SEL1 |=  V_3_3;
    P5SEL0 &= ~IOT_BOOT_CPU; P5SEL1 &= ~IOT_BOOT_CPU; P5OUT &= ~IOT_BOOT_CPU; P5DIR &= ~IOT_BOOT_CPU;
}

void Init_Port_6(void) {
    P6SEL0 &= ~LCD_BACKLITE; P6SEL1 &= ~LCD_BACKLITE; P6OUT |= LCD_BACKLITE; P6DIR |= LCD_BACKLITE;
    P6SEL0 &= ~R_FORWARD; P6SEL1 &= ~R_FORWARD; P6OUT &= ~R_FORWARD; P6DIR |= R_FORWARD;
    P6SEL0 &= ~L_FORWARD; P6SEL1 &= ~L_FORWARD; P6OUT &= ~L_FORWARD; P6DIR |= L_FORWARD;
    P6SEL0 &= ~R_REVERSE; P6SEL1 &= ~R_REVERSE; P6OUT &= ~R_REVERSE; P6DIR |= R_REVERSE;
    P6SEL0 &= ~L_REVERSE; P6SEL1 &= ~L_REVERSE; P6OUT &= ~L_REVERSE; P6DIR |= L_REVERSE;
    P6SEL0 &= ~P6_5; P6SEL1 &= ~P6_5; P6OUT &= ~P6_5; P6DIR &= ~P6_5;
    P6SEL0 &= ~GRN_LED; P6SEL1 &= ~GRN_LED; P6OUT &= ~GRN_LED; P6DIR |= GRN_LED;
}
