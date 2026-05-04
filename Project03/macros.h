//------------------------------------------------------------------------------
//  NC State University ECE 306
//  Instructor: Jim Carlson
//  Homework #05
//
//  File Name: macros.h
//  Author: Ezana Enquobahrie
//  Date: February 18, 2026
//  Compiler: Code Composer Studio 12.8.1
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

// PORT 3 CONFIGURATION (HW05)
#define USE_GPIO             (0x00)
#define USE_SMCLK            (0x01)

// SWITCH DEBOUNCE
#define PRESSED              (1)
#define RELEASED             (0)
#define NOT_OKAY             (0)
#define OKAY                 (1)
#define DEBOUNCE_TIME        (6)
#define DEBOUNCE_RESTART     (0)

// Timer Interval for 500kHz SMCLK
#define TB0CCR0_INTERVAL    (2500)

// Thomas Pin Name Fixes
#define IOT_LINK_CPU        (0x40) // P3.6
#define IOT_EN_CPU          (0x80) // P3.7

#endif
