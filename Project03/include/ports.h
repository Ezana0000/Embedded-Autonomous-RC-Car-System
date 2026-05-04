//------------------------------------------------------------------------------
// ports.h
//------------------------------------------------------------------------------
// Description: Port bit masks + prototypes for MSP430FR2355
// Course: ECE 306 (based on Lecture 06 - Ports)
//-----------------------------------------------------------------------------
#ifndef PORTS_H
#define PORTS_H

// Port 1 Pins
#define RED_LED        (0x01)
#define A1_SEEED       (0x02)
#define V_DETECT_L     (0x04)
#define V_DETECT_R     (0x08)
#define A4_SEEED       (0x10)
#define V_THUMB        (0x20)
#define UCA0RXD        (0x40)
#define UCA0TXD        (0x80)

// Port 2 Pins
#define SLOW_CLK       (0x01)
#define CHECK_BAT      (0x02)
#define IR_LED         (0x04)
#define SW2            (0x08)
#define IOT_RUN_RED    (0x10)
#define DAC_ENB        (0x20)
#define LFXOUT         (0x40)
#define LFXIN          (0x80)

// Port 3 Pins
#define TEST_PROBE     (0x01)
#define OA2O           (0x02)
#define OA2N           (0x04)
#define OA2P           (0x08)
#define SMCLK          (0x10)
#define DAC_CNTL       (0x20)
#define IOT_LINK_GRN   (0x40)
#define IOT_EN         (0x80)

// Port 4 Pins
#define RESET_LCD      (0x01)
#define SW1            (0x02)
#define UCA1RXD        (0x04)
#define UCA1TXD        (0x08)
#define UCB1_CS_LCD    (0x10)
#define UCB1CLK        (0x20)
#define UCB1SIMO       (0x40)
#define UCB1SOMI       (0x80)

// Port 5 Pins
#define V_BAT          (0x01)
#define V_5            (0x02)
#define V_DAC          (0x04)
#define V_3_3          (0x08)
#define IOT_BOOT_CPU   (0x10)

// Port 6 Pins
#define LCD_BACKLITE   (0x01)
#define R_FORWARD      (0x02)
#define L_FORWARD      (0x04)
#define R_REVERSE      (0x08)
#define L_REVERSE      (0x10)
#define P6_5           (0x20)
#define GRN_LED        (0x40)

void Init_Ports(void);
void Init_Port_1(void);
void Init_Port_2(void);
void Init_Port_3(unsigned char smclk_state);
void Init_Port_4(void);
void Init_Port_5(void);
void Init_Port_6(void);

void Motors_All_Off(void);
void Motors_Forward_On(void);
void Motors_Reverse_On(void);

#endif
