//------------------------------------------------------------------------------
//
//  Description: System-wide macros, definitions, and motor speeds.
//  Contains hardware register mappings and state machine definitions.
//
//  Ezana
//  April 2026
//  Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------

#ifndef MACROS_H_
#define MACROS_H_

// Standard Constants
#define ALWAYS               (1)
#define RESET_STATE          (0)
#define TRUE                 (1)
#define RESET_REGISTER       (0x0000)

// Timer B0 Configurations
#define TB0CCR0_INTERVAL     (6250) // 50ms base tick
#define TB0CCR1_INTERVAL     (6250) // 50ms SW1 debounce tick
#define TB0CCR2_INTERVAL     (6250) // 50ms SW2 debounce tick
#define DEBOUNCE_THRESHOLD   (20)   // 20 * 50ms = 1000ms debounce

// ADC Channels
#define THUMB_WHEEL_CH       (5)
#define LEFT_DETECTOR_CH     (2)
#define RIGHT_DETECTOR_CH    (3)

// Navigation States
#define P7_WAIT           (1)
#define P7_INTERCEPTING   (2)
#define P7_TURNING        (3)
#define P7_CIRCLING       (4)
#define P7_CENTERING      (5)
#define P7_STOPPED        (6)

// Timing Logic
#define ONE_SEC           (5)
#define FOUR_SEC          (20)
#define TURN_BLIND_TIME   (2)
#define TWO_CIRCLES_TIME  (160)

// PWM Hardware Registers
#define PWM_PERIOD          TB3CCR0
#define LCD_BACKLITE_DIMING TB3CCR1
#define RIGHT_FORWARD_SPEED TB3CCR2
#define LEFT_FORWARD_SPEED  TB3CCR3
#define RIGHT_REVERSE_SPEED TB3CCR4
#define LEFT_REVERSE_SPEED  TB3CCR5

// Asymmetric Motor Calibrations
#define WHEEL_PERIOD         (50000)
#define WHEEL_OFF            (0)
#define PERCENT_80           (45000)

// Standard Navigation Speeds
#define R_ARC_FAST           (25800)
#define L_ARC_FAST           (45000)
#define R_GENTLE_FAST        (30000)
#define L_GENTLE_FAST        (50000)
#define R_GENTLE_SLOW        (5000)
#define L_GENTLE_SLOW        (10000)

// Hard Corner Pivots
#define R_HARD_FAST          (35000)
#define L_HARD_FAST          (60000)
#define HARD_SLOW            (WHEEL_OFF)

// Low-Gear Speeds
#define C_R_FAST             (20000)
#define C_L_FAST             (35000)
#define C_SLOW               (WHEEL_OFF)

// Menu and Display Configuration
#define MAIN_MENU_DIVIDER      1365  // 4096 / 3 items
#define SUB_MENU_DIVIDER       410   // 4096 / 10 items
#define MAX_SUB_MENU_INDEX     9     // Array bounds (0-9)
#define SONG_SCROLL_THRESHOLD  150   // ADC change threshold
#define MAX_SONG_INDEX         24    // Array bounds (0-24)

// UI States
#define SPLASH 0
#define MAIN_MENU 1
#define MENU_RESISTORS 2
#define MENU_SHAPES 3
#define MENU_SONG 4

// Serial Commands
#define WAIT ('W')
#define FORWARDS ('F')
#define BACK ('B')
#define RIGHT ('R')
#define LEFT ('L')

#endif /* MACROS_H_ */
