//------------------------------------------------------------------------------
//  NC State University ECE 306
//  Instructor: Jim Carlson
//  Homework #04
//
//  File Name: macros.h
//  Author: Ezana Enquobahrie
//  Date: February 14, 2026
//  Compiler: Code Composer Studio 12.8.1
//
//  Description:
//  This file contains all the definitions, macros, and constant values used
//  throughout the project. It defines state machine states, shape types,
//  and the specific motor timing values I tuned for my car.
//
//  Global Variables Defined:
//  None (Header file)
//------------------------------------------------------------------------------
#ifndef MACROS_H
#define MACROS_H

#define ALWAYS                  (1)
#define RESET_STATE             (0)
#define RED_LED              (0x01)
#define GRN_LED              (0x40)
#define TEST_PROBE           (0x01)
#define TRUE                 (0x01)
#define FALSE                (0x00)

// STATES

#define IDLE         (0)
#define PAUSE        (1)
#define GO           (2)
#define RUN          (3)
#define STOP         (4)

// SHAPES
#define NONE         (0)
#define CIRCLE       (1)
#define FIGURE8      (2)
#define TRIANGLE     (3)

// SWITCH DEBOUNCE
#define PRESSED              (1)
#define RELEASED             (0)
#define NOT_OKAY             (0)
#define OKAY                 (1)
#define DEBOUNCE_TIME        (6)
#define DEBOUNCE_RESTART     (0)

// MOTOR TUNING
//  I kept SLOW_COUNT_TIME at 3 so the inner wheel turns (KVL requirement).
#define WHEEL_COUNT_TIME    (20)  // 100ms cycle
#define RIGHT_COUNT_TIME    (20)
#define LEFT_COUNT_TIME     (20)
#define SLOW_COUNT_TIME     (3)

// DURATION TUNING
// 200 counts is roughly 1 second on the 5ms timer.
#define PAUSE_DURATION      (200) // 1 Second Wait
#define CIRCLE_TIME         (730) // 3 Seconds per Circle
#define SIDE_TIME           (200) // 1 Second Straight
#define TURN_TIME           (170) // 0.5 Second Turn

// *** FIGURE 8 SPLIT TIMERS ***
// Increase this if the Left circle doesn't close
// I split these because my motors aren't perfectly equal.
#define CIRCLE_TIME_L       (750) // Make this huge
#define CIRCLE_TIME_R       (830)  // Make this tiny

#endif
