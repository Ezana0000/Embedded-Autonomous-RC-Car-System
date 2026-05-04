//------------------------------------------------------------------------------
//
//  Description: This file handles the main clock initialization (8MHz MCLK)
//
// Ezana
//  Feb 2026
//  Built with Code Composer Studio
//------------------------------------------------------------------------------
#include "msp430.h"
#include "functions.h"
#include "LCD.h"
#include "ports.h"
#include "macros.h"

// I define the master clock frequency here to 8MHz
#define MCLK_FREQ_MHZ           (8)
#define CLEAR_REGISTER     (0X0000)

void Init_Clocks(void);
void Software_Trim(void);

void Init_Clocks(void){
// -----------------------------------------------------------------------------
// I configure the clock system to run MCLK and SMCLK at 8MHz.
// I also make sure to disable the watchdog timer right away so it doesn't
// accidentally reset my program while it's running.
// -----------------------------------------------------------------------------
  WDTCTL = WDTPW | WDTHOLD;  // Turn off the watchdog timer

  do{
    CSCTL7 &= ~XT1OFFG;      // Clear my XT1 fault flag
    CSCTL7 &= ~DCOFFG;       // Clear my DCO fault flag
    SFRIFG1 &= ~OFIFG;
  } while (SFRIFG1 & OFIFG); // Keep checking until the fault flag clears
  __bis_SR_register(SCG0);   // Disable FLL temporarily

  CSCTL1 = DCOFTRIMEN_1;
  CSCTL1 |= DCOFTRIM0;
  CSCTL1 |= DCOFTRIM1;       // Set DCOFTRIM to 3
  CSCTL1 |= DCORSEL_3;       // Setting the DCO range for 8MHz

  CSCTL2 = FLLD_0 + 243;     // DCODIV for 8MHz

  CSCTL3 |= SELREF__XT1CLK;  // Make XT1CLK my FLL reference
  __delay_cycles(3);
  __bic_SR_register(SCG0);   // Re-enable FLL
  Software_Trim();           // I run the software trim to lock the best frequency

  CSCTL4 = SELA__XT1CLK;     // ACLK gets 32768Hz
  CSCTL4 |= SELMS__DCOCLKDIV;// Use DCOCLK as the source for MCLK and SMCLK

  CSCTL5 |= DIVM__1;        // MCLK = DCOCLK = 8MHZ
  CSCTL5 |= DIVS__1;        // SMCLK = MCLK = 8MHz

  PM5CTL0 &= ~LOCKLPM5;      // Finally, disable high-impedance mode so my pins work
}

void Software_Trim(void){
// I use this function to calculate and lock the most accurate DCO tap
  unsigned int oldDcoTap = 0xffff;
  unsigned int newDcoTap = 0xffff;
  unsigned int newDcoDelta = 0xffff;
  unsigned int bestDcoDelta = 0xffff;
  unsigned int csCtl0Copy = 0;
  unsigned int csCtl1Copy = 0;
  unsigned int csCtl0Read = 0;
  unsigned int csCtl1Read = 0;
  unsigned int dcoFreqTrim = 3;
  unsigned char endLoop = 0;

  do{
    CSCTL0 = 0x100;
    do{
      CSCTL7 &= ~DCOFFG;
    }while (CSCTL7 & DCOFFG);

    // I wait a bit for the FLL lock status to stabilize
    __delay_cycles((unsigned int)3000 * MCLK_FREQ_MHZ);
    while((CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1)) &&
         ((CSCTL7 & DCOFFG) == 0));

    csCtl0Read = CSCTL0;
    csCtl1Read = CSCTL1;
    oldDcoTap = newDcoTap;
    newDcoTap = csCtl0Read & 0x01ff;
    dcoFreqTrim = (csCtl1Read & 0x0070)>>4;

    if(newDcoTap < 256){
      newDcoDelta = 256 - newDcoTap;
      if((oldDcoTap != 0xffff) &&
         (oldDcoTap >= 256)){
        endLoop = 1;
      }else{
        dcoFreqTrim--;
        CSCTL1 = (csCtl1Read & (~DCOFTRIM)) | (dcoFreqTrim<<4);
      }
    }else{
      newDcoDelta = newDcoTap - 256;
      if(oldDcoTap < 256){
        endLoop = 1;
      }else{
        dcoFreqTrim++;
        CSCTL1 = (csCtl1Read & (~DCOFTRIM)) | (dcoFreqTrim<<4);
      }
    }
    if(newDcoDelta < bestDcoDelta){
      csCtl0Copy = csCtl0Read;
      csCtl1Copy = csCtl1Read;
      bestDcoDelta = newDcoDelta;
    }
  }while(endLoop == 0);

  CSCTL0 = csCtl0Copy;
  CSCTL1 = csCtl1Copy;
  while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1)); // Hang here until FLL successfully locks
}
