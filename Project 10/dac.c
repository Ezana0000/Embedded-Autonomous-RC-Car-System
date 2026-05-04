//------------------------------------------------------------------------------
//
//  Description: Initialization for the Digital-to-Analog Converter (DAC).
//  Configures the reference voltage and sets the initial DAC data output.
//
//  Ezana
//  April 2026
//  Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------

#include  "functions.h"
#include  "msp430.h"
#include  "macros.h"
#include "ports.h"

unsigned int DAC_data;

void Init_DAC(void){
  DAC_data = 4000;
  SAC3DAT  = DAC_data;                  // Initial DAC data
  SAC3DAC  = DACSREF_0;                 // Select AVCC as DAC reference
  SAC3DAC |= DACLSEL_0;                 // DAC latch loads when DACDAT written

  SAC3OA   = NMUXEN;                    // SAC Negative input MUX controL
  SAC3OA  |= PMUXEN;                    // SAC Positive input MUX control
  SAC3OA  |= PSEL_1;                    // 12-bit reference DAC source selected
  SAC3OA  |= NSEL_1;                    // Select negative pin input
  SAC3OA  |= OAPM;                      // Select low speed and low power mode
  SAC3PGA  = MSEL_1;                    // Set OA as buffer mode
  SAC3OA  |= SACEN;                     // Enable SAC
  SAC3OA  |= OAEN;                      // Enable OA

  P3OUT   &= ~DAC_CNTL;                 // Set output to Low
  P3DIR   &= ~DAC_CNTL;                 // Set direction to input
  P3SELC  |=  DAC_CNTL;                 // DAC_CNTL DAC operation
  SAC3DAC |=  DACEN;                    // Enable DAC
}
