//------------------------------------------------------------------------------
//
//  Description: Corrected motor control logic.
//               Swapped Spin_CW and Spin_CCW to match physical motor inversion.
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

    RIGHT_FORWARD_SPEED = 38000;        // Dial in right forward speed
    LEFT_FORWARD_SPEED = 45000;         // Dial in left forward speed
}

void Reverse_On(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF;    // Safely kill forward first
    LEFT_FORWARD_SPEED = WHEEL_OFF;

    RIGHT_REVERSE_SPEED = 33000;        // Drive reverse
    LEFT_REVERSE_SPEED = 45000;
}

// --- FIXED: Removed Wheels_All_Off to stop pulsing ---
void Spin_CW_On(void){
    // Kill the pins that would fight the turn
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED = WHEEL_OFF;

    // Apply turn power
    RIGHT_FORWARD_SPEED = 23200;
    LEFT_REVERSE_SPEED = 35000;
}

// --- FIXED: Removed Wheels_All_Off to stop pulsing ---
void Spin_CCW_On(void){
    // Kill the pins that would fight the turn
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;

    // Apply turn power
    LEFT_FORWARD_SPEED = 23200;
    RIGHT_REVERSE_SPEED = 35000;
}
