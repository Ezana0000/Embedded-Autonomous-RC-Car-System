//===========================================================================
// File Name : macros.h
// Description: Constant definitions and macros. No magic numbers.
//===========================================================================
#ifndef MACROS_H_
#define MACROS_H_

// Standard Constants
#define ALWAYS               (1)
#define RESET_STATE          (0)
#define TRUE                 (1)
#define RESET_REGISTER       (0x0000)

// Timer B0 Math (8,000,000 Hz / 8 / 8 = 125,000 Hz)
#define TB0CCR0_INTERVAL     (25000) // 50ms base tick
#define TB0CCR1_INTERVAL     (25000) // 50ms SW1 debounce tick
#define TB0CCR2_INTERVAL     (25000) // 50ms SW2 debounce tick
#define DEBOUNCE_THRESHOLD   (20)    // 20 * 50ms = 1000ms debounce

// Project 5 States
#define P5_FWD1              (1)
#define P5_PAUSE1            (2)
#define P5_REV               (3)
#define P5_PAUSE2            (4)
#define P5_FWD2              (5)
#define P5_PAUSE3            (6)
#define P5_SPIN_CW           (7)
#define P5_PAUSE4            (8)
#define P5_SPIN_CCW          (9)
#define P5_PAUSE5            (10)
#define P5_DONE              (11)

// Project 5 Timing (Counter increments every 200ms)
#define ONE_SEC              (5)     // 5 * 200ms = 1.0 sec
#define TWO_SEC              (10)    // 10 * 200ms = 2.0 sec
#define THREE_SEC            (15)    // 15 * 200ms = 3.0 sec

// ADC Channels
#define THUMB_WHEEL_CH    (5)
#define LEFT_DETECTOR_CH  (2)
#define RIGHT_DETECTOR_CH (3)

// Project 6 States
// Project 6 Navigation States
#define P6_WAIT           (1)
#define P6_FORWARD        (2)
#define P6_STOP_DETECTED  (3)
#define P6_TURN_SEARCH    (4)
#define P6_TURN_CROSS     (5)
#define P6_RECENTER       (6)
#define P6_DONE           (7)

// Project 6 Timing (Assumes p5_timer increments every 200ms)
#define ONE_SEC           (5)  // 5 * 200ms = 1.0 sec
#define FOUR_SEC          (20) // 20 * 200ms = 4.0 sec

#define TURN_BLIND_TIME   (1)  // 4 * 200ms = 0.8 seconds (Tweak this!)

// Calibration Constant
// *** CHANGE THIS NUMBER TO YOUR CALIBRATED 12-BIT THRESHOLD ***
#define BLACK_LINE_THRESH (3000)


#endif /* MACROS_H_ */


