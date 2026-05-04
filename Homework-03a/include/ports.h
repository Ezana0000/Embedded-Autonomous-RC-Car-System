//------------------------------------------------------------------------------
// ports.h
//------------------------------------------------------------------------------
//
// Description: Port bit masks + prototypes for MSP430FR2355
// Course: ECE 306 (based on Lecture 06 - Ports)
//-----------------------------------------------------------------------------

// Port 1 Pins
#define RED_LED        (0x01)   // 0 RED LED
#define A1_SEEED       (0x02)   // 1 A1_SEEED
#define V_DETECT_L     (0x04)   // 2 V_DETECT_L
#define V_DETECT_R     (0x08)   // 3 V_DETECT_R
#define A4_SEEED       (0x10)   // 4 A4_SEEED
#define V_THUMB        (0x20)   // 5 V_THUMB
#define UCA0RXD        (0x40)   // 6 Back Channel UCA0RXD
#define UCA0TXD        (0x80)   // 7 Back Channel UCA0TXD

// Port 2 Pins
#define SLOW_CLK       (0x01)   // 0 SLOW_CLK
#define CHECK_BAT      (0x02)   // 1 Check ADC Voltages
#define IR_LED         (0x04)   // 2 IR_LED
#define SW2            (0x08)   // 3 SW2
#define IOT_RUN_RED    (0x10)   // 4 IOT_RUN_CPU
#define DAC_ENB        (0x20)   // 5 DAC_ENB
#define LFXOUT         (0x40)   // 6 XOUTR
#define LFXIN          (0x80)   // 7 XINR

// Port 3 Pins
#define TEST_PROBE     (0x01)   // 0 TEST PROBE
#define OA2O           (0x02)   // 1 OA2O
#define OA2N           (0x04)   // 2 OA2N
#define OA2P           (0x08)   // 3 OA2P
#define SMCLK          (0x10)   // 4 SMCLK
#define DAC_CNTL       (0x20)   // 5 DAC signal from Processor
#define IOT_LINK_GRN   (0x40)   // 6 IOT_LINK_GRN
#define IOT_EN         (0x80)   // 7 IOT_EN

// Port 4 Pins
#define RESET_LCD      (0x01)   // 0 RESET_LCD
#define SW1            (0x02)   // 1 SW1
#define UCA1RXD        (0x04)   // 2 Back Channel UCA1RXD
#define UCA1TXD        (0x08)   // 3 Back Channel UCA1TXD
#define UCB1_CS_LCD    (0x10)   // 4 Chip Select
#define UCB1CLK        (0x20)   // 5 SPI mode - clock output UCB1CLK
#define UCB1SIMO       (0x40)   // 6 UCB1SIMO
#define UCB1SOMI       (0x80)   // 7 UCB1SOMI

// Port 5 Pins
#define V_BAT          (0x01)   // 0 V_BAT
#define V_5            (0x02)   // 1 V_5
#define V_DAC          (0x04)   // 2 V_DAC
#define V_3_3          (0x08)   // 3 V_3.3
#define IOT_BOOT_CPU   (0x10)   // 4 IOT_BOOT_CPU


// Port 6 Pins
#define LCD_BACKLITE   (0x01)   // 0 LCD_BACKLITE
#define R_FORWARD      (0x02)   // 1 R_FORWARD
#define L_FORWARD      (0x04)   // 2 L_FORWARD
#define R_REVERSE      (0x08)   // 3 R_REVERSE
#define L_REVERSE      (0x10)   // 4 L_REVERSE
#define P6_5           (0x20)   // 4 P_65
#define GRN_LED        (0x40)   // 4 GRN_LED





void Init_Ports(void);

void Init_Port_1(void);
void Init_Port_2(void);
void Init_Port_3(void);
void Init_Port_4(void);
void Init_Port_5(void);
void Init_Port_6(void);

