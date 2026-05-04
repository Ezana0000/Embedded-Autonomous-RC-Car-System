//===========================================================================
// File Name : macros.h
//===========================================================================
#ifndef MACROS_H_
#define MACROS_H_

// Standard Constants
#define ALWAYS               (1)
#define RESET_STATE          (0)
#define TRUE                 (1)
#define RESET_REGISTER       (0x0000)

// Timer B0 Math
#define TB0CCR0_INTERVAL     (6250) // 50ms base tick
#define TB0CCR1_INTERVAL     (6250) // 50ms SW1 debounce tick
#define TB0CCR2_INTERVAL     (6250) // 50ms SW2 debounce tick
#define DEBOUNCE_THRESHOLD   (20)   // 20 * 50ms = 1000ms debounce

// ADC Channels
#define THUMB_WHEEL_CH       (5)
#define LEFT_DETECTOR_CH     (2)
#define RIGHT_DETECTOR_CH    (3)

// Project 7 Navigation States
#define P7_WAIT           (1)
#define P7_INTERCEPTING   (2)
#define P7_TURNING        (3)
#define P7_CIRCLING       (4)
#define P7_CENTERING      (5)
#define P7_STOPPED        (6)

// Timing Macros
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

// --- SHARPER, SLOWER PWM SPEEDS ---
#define WHEEL_PERIOD        (50000)
#define WHEEL_OFF           (0)
#define PERCENT_80          (45000)

// The Natural Curve
#define ARC_FAST            (18000)
#define ARC_SLOW            (10000)

// Gentle Drift
#define GENTLE_FAST         (22000)
#define GENTLE_SLOW         (5000)

// Hard Corrections
#define HARD_FAST           (28000)
#define HARD_SLOW           (WHEEL_OFF)

#endif /* MACROS_H_ */
