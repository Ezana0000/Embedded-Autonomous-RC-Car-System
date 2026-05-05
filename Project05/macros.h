//===========================================================================
// File Name : macros.h
// Description: Constant definitions and macros. No magic numbers.
//===========================================================================
#ifndef MACROS_H_
#define MACROS_H_

#define ALWAYS                  (1)
#define RESET_STATE             (0)
#define TRUE                 (0x01)

// Timer B0 Math from old project (8MHz / 8 / 8 = 125,000 Hz)
#define TB0CCR0_INTERVAL     (25000) // 200ms tick for display/backlight
#define TB0CCR1_INTERVAL     (6250)  // 50ms SW1 Debounce
#define TB0CCR2_INTERVAL     (6250)  // 50ms SW2 Debounce
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

// Project 5 Timing
#define ONE_SEC              (5)     // 5 * 200ms = 1 sec
#define TWO_SEC              (10)
#define THREE_SEC            (15)

#endif /* MACROS_H_ */
