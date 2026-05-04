// ------------------------------------------------------------------------------
//  Description: This file contains the Clock Initialization
//  Ezana Enquobahrie - Homework 05
// ------------------------------------------------------------------------------
#include  "msp430.h"
#include  "macros.h"

#define MCLK_FREQ_MHZ           (8)

void Init_Clocks(void);
void Software_Trim(void);

void Init_Clocks(void){
  WDTCTL = WDTPW | WDTHOLD;

  do{
    CSCTL7 &= ~XT1OFFG;
    CSCTL7 &= ~DCOFFG;
    SFRIFG1 &= ~OFIFG;
  } while (SFRIFG1 & OFIFG);
  __bis_SR_register(SCG0);

  CSCTL1 = DCOFTRIMEN_1;
  CSCTL1 |= DCOFTRIM0;
  CSCTL1 |= DCOFTRIM1;
  CSCTL1 |= DCORSEL_3;

  CSCTL2 = FLLD_0 + 243; // DCO = 8MHz

  CSCTL3 |= SELREF__XT1CLK;
  __delay_cycles(3);
  __bic_SR_register(SCG0);
  Software_Trim();

  CSCTL4 = SELA__XT1CLK;
  CSCTL4 |= SELMS__DCOCLKDIV;

  // HW 05: SMCLK = 8MHz / 16 = 500kHz
  CSCTL5 |= DIVM__2;
  CSCTL5 |= DIVS__8;

  PM5CTL0 &= ~LOCKLPM5;
}

void Software_Trim(void){
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
    do{ CSCTL7 &= ~DCOFFG; } while (CSCTL7 & DCOFFG);
    __delay_cycles((unsigned int)3000 * MCLK_FREQ_MHZ);
    while((CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1)) && ((CSCTL7 & DCOFFG) == 0));
    csCtl0Read = CSCTL0;
    csCtl1Read = CSCTL1;
    oldDcoTap = newDcoTap;
    newDcoTap = csCtl0Read & 0x01ff;
    dcoFreqTrim = (csCtl1Read & 0x0070)>>4;
    if(newDcoTap < 256){
      newDcoDelta = 256 - newDcoTap;
      if((oldDcoTap != 0xffff) && (oldDcoTap >= 256)){ endLoop = 1; }
      else{ dcoFreqTrim--; CSCTL1 = (csCtl1Read & (~DCOFTRIM)) | (dcoFreqTrim<<4); }
    }else{
      newDcoDelta = newDcoTap - 256;
      if(oldDcoTap < 256){ endLoop = 1; }
      else{ dcoFreqTrim++; CSCTL1 = (csCtl1Read & (~DCOFTRIM)) | (dcoFreqTrim<<4); }
    }
    if(newDcoDelta < bestDcoDelta){ csCtl0Copy = csCtl0Read; csCtl1Copy = csCtl1Read; bestDcoDelta = newDcoDelta; }
  }while(endLoop == 0);
  CSCTL0 = csCtl0Copy;
  CSCTL1 = csCtl1Copy;
  while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));
}
