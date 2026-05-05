//------------------------------------------------------------------------------
//
//  Description: This file contains my motor control logic. Because the full
//               H-Bridge is dangerous, I always force opposite direction pins
//               low before I pull the requested pins high to prevent a short circuit.
//
//  Ahan
//  Feb 2026
//  Built with Code Composer Studio
//------------------------------------------------------------------------------

#include "msp430.h"
#include "functions.h"
#include "macros.h"
#include "ports.h"

//------------------------------------------------------------------------------
// Function: Wheels_All_Off
// I use this function as a safety catch-all. It guarantees that all wheels
// stop moving, regardless of their current direction.
//------------------------------------------------------------------------------
void Wheels_All_Off(void){
    P6OUT &= ~R_FORWARD;
    P6OUT &= ~L_FORWARD;
    P6OUT &= ~R_REVERSE;
    P6OUT &= ~L_REVERSE;
}

//------------------------------------------------------------------------------
// Function: Forward_On
// I drive the car forward. Notice that I turn the reverse pins OFF first
//------------------------------------------------------------------------------
void Forward_On(void){
    P6OUT &= ~R_REVERSE;     // Safely kill right reverse
    P6OUT &= ~L_REVERSE;     // Safely kill left reverse
    P6OUT |= R_FORWARD;      // NOW it's safe to turn right forward on
    P6OUT |= L_FORWARD;      // NOW it's safe to turn left forward on
}

//------------------------------------------------------------------------------
// Function: Forward_Off
//------------------------------------------------------------------------------
void Forward_Off(void){
    P6OUT &= ~R_FORWARD;
    P6OUT &= ~L_FORWARD;
}

//------------------------------------------------------------------------------
// Function: Reverse_On
// I drive the car backward. Notice that I turn the forward pins OFF first
//------------------------------------------------------------------------------
void Reverse_On(void){
    P6OUT &= ~R_FORWARD;     // Safely kill right forward
    P6OUT &= ~L_FORWARD;     // Safely kill left forward
    P6OUT |= R_REVERSE;
    P6OUT |= L_REVERSE;
}

//------------------------------------------------------------------------------
// Function: Reverse_Off
//------------------------------------------------------------------------------
void Reverse_Off(void){
    P6OUT &= ~R_REVERSE;
    P6OUT &= ~L_REVERSE;
}

//------------------------------------------------------------------------------
// Function: Spin_CW_On
// This spins the car clockwise. Left wheel drives forward, right wheel drives in reverse.
//------------------------------------------------------------------------------
void Spin_CW_On(void){
    Wheels_All_Off();         // I call the master off switch first for absolute safety
    P6OUT |= L_FORWARD;
    P6OUT |= R_REVERSE;
}

//------------------------------------------------------------------------------
// Function: Spin_CCW_On
// This spins the car counter-clockwise. Right wheel drives forward, left wheel drives in reverse.
//------------------------------------------------------------------------------
void Spin_CCW_On(void){
    Wheels_All_Off();         // Call the master off switch first
    P6OUT |= R_FORWARD;
    P6OUT |= L_REVERSE;
}

//------------------------------------------------------------------------------
// These are some old dummy functions I kept so I don't break legacy function calls
//------------------------------------------------------------------------------
void Forward_Move(void){
    // Deprecated for Project 5
}

void Wheels_Forward(void){
    Forward_On();  // I just route it to my new safe function
}

void Wheels_Off(void){
    Wheels_All_Off();  // Route to safe function
}
