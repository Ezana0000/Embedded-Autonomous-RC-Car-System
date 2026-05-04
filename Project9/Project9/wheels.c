//------------------------------------------------------------------------------
//
//  Description: This file contains my motor control logic. Because the full
//               H-Bridge is dangerous, I always force opposite direction pins
//               low before I pull the requested pins high to prevent a short circuit.
//
// Ezana
//  Feb 2026
//  Built with Code Composer Studio
//------------------------------------------------------------------------------

#include "msp430.h"
#include "functions.h"
#include "macros.h"
#include "ports.h"

void Wheels_All_Off(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
}

void Forward_On(void){
    RIGHT_REVERSE_SPEED = WHEEL_OFF;    // Safely kill right reverse
    LEFT_REVERSE_SPEED = WHEEL_OFF;     // Safely kill left reverse

    RIGHT_FORWARD_SPEED = 35000;        // Dial in right forward speed
    LEFT_FORWARD_SPEED = 35000;         // Dial in left forward speed
}

void Reverse_On(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF;    // Safely kill right forward
    LEFT_FORWARD_SPEED = WHEEL_OFF;     // Safely kill left forward

    RIGHT_REVERSE_SPEED = 35000;        // Dial in right reverse speed
    LEFT_REVERSE_SPEED = 35000;         // Dial in left reverse speed
}

void Spin_CW_On(void){
    Wheels_All_Off();                   // Master off switch first
    LEFT_FORWARD_SPEED = 35000;
    RIGHT_REVERSE_SPEED = 35000;
}

void Spin_CCW_On(void){
    Wheels_All_Off();                   // Master off switch first
    RIGHT_FORWARD_SPEED = 35000;
    LEFT_REVERSE_SPEED = 35000;
}
