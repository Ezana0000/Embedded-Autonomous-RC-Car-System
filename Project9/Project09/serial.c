//===========================================================================
// File Name : serial.c
// Description: Project 9 Passthrough, Parsing, and Boot Logic
//===========================================================================

#include "msp430.h"
#include <string.h>
#include "functions.h"
#include "LCD.h"
#include "macros.h"

// --- Project 9 Global Variables ---
volatile char pc_ready = 0; // Starts OFF to protect the USB driver

// --- Phase 3 IP Parsing Variables ---
volatile char IOT_Ring_Rx[32];
volatile unsigned int iot_rx_wr = 0;
volatile char ip_display1[11] = "          ";
volatile char ip_display2[11] = "          ";
volatile unsigned int ip_record_flag = 0;
volatile unsigned int period_count = 0;
volatile unsigned int ip_index1 = 0;
volatile unsigned int ip_index2 = 0;

// Helper Function: Allows main.c to send strings to the ESP32
void Transmit_UCA0_String(char *str) {
    while (*str) {
        while (!(UCA0IFG & UCTXIFG)); // Wait for TX buffer to be ready
        UCA0TXBUF = *str;             // Send character
        str++;                        // Move to next character
    }
}

// Initialize IoT Port (ESP32)
void Init_Serial_UCA0(void){
    UCA0CTLW0 = 0;
    UCA0CTLW0 |= UCSWRST;
    UCA0CTLW0 |= UCSSEL__SMCLK;
    UCA0CTLW0 &= ~UCMSB;
    UCA0CTLW0 &= ~UCSPB;
    UCA0CTLW0 &= ~UCPEN;
    UCA0CTLW0 &= ~UCSYNC;
    UCA0CTLW0 &= ~UC7BIT;
    UCA0CTLW0 |= UCMODE_0;

    // 115,200 Baud Rate Math
    UCA0BRW = 4;
    UCA0MCTLW = 0x5551;

    UCA0CTLW0 &= ~UCSWRST;
    UCA0IE |= UCRXIE;
}

// Initialize PC Port (USB)
void Init_Serial_UCA1(void){
    UCA1CTLW0 = 0;
    UCA1CTLW0 |= UCSWRST;
    UCA1CTLW0 |= UCSSEL__SMCLK;
    UCA1CTLW0 &= ~UCMSB;
    UCA1CTLW0 &= ~UCSPB;
    UCA1CTLW0 &= ~UCPEN;
    UCA1CTLW0 &= ~UCSYNC;
    UCA1CTLW0 &= ~UC7BIT;
    UCA1CTLW0 |= UCMODE_0;

    // 115,200 Baud Rate Math
    UCA1BRW = 4;
    UCA1MCTLW = 0x5551;

    UCA1CTLW0 &= ~UCSWRST;
    UCA1IE |= UCRXIE;
}

// =======================================================================
// INTERRUPT SERVICE ROUTINES
// =======================================================================

// UCA0 ISR: Receives data from the IoT Module
#pragma vector = EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void){
    char temp;
    switch(__even_in_range(UCA0IV, 0x08)){
        case 0: break;
        case 2: // Character Received!
            temp = UCA0RXBUF;

            // 1. PASSTHROUGH TO PC
            if (pc_ready == 1) {
                while (!(UCA1IFG & UCTXIFG)); // Wait for PC TX buffer
                UCA1TXBUF = temp;             // Send to PC
            }

            // 2. PARSE THE IP ADDRESS FOR THE LCD
            IOT_Ring_Rx[iot_rx_wr] = temp; // Save to ring buffer

            // Check if we found the start of the IP (STAIP,"10.)
            if (ip_record_flag == 0 && iot_rx_wr >= 2) {
                if (IOT_Ring_Rx[iot_rx_wr - 2] == 'P' &&
                    IOT_Ring_Rx[iot_rx_wr - 1] == ',' &&
                    temp == '"') {
                    ip_record_flag = 1; // Start recording!
                    period_count = 0;
                    ip_index1 = 0;
                    ip_index2 = 0;
                }
            }
            // We are currently recording the IP address
            else if (ip_record_flag == 1) {
                if (temp == '"') {
                    ip_record_flag = 0; // Stop recording at the end quote

                    // Pad the rest of the arrays with spaces
                    while(ip_index1 < 10) ip_display1[ip_index1++] = ' ';
                    while(ip_index2 < 10) ip_display2[ip_index2++] = ' ';
                }
                else {
                    if (temp == '.') period_count++;

                    // Split the IP across two lines
                    if (period_count < 2 && ip_index1 < 10) {
                        ip_display1[ip_index1++] = temp;
                    } else if (period_count >= 2 && ip_index2 < 10) {
                        ip_display2[ip_index2++] = temp;
                    }
                }
            }

            // Advance buffer index
            iot_rx_wr++;
            if(iot_rx_wr >= 32) iot_rx_wr = 0;
            break;

        case 4: break;
        default: break;
    }
}

// UCA1 ISR: Receives data from your PC Keyboard (Termite)
#pragma vector = EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void){
    char temp;
    switch(__even_in_range(UCA1IV, 0x08)){
        case 0: break;
        case 2: // Character Received from PC!
            temp = UCA1RXBUF;

            // BYPASS HACK: Shove the PC keystrokes directly into the parser buffer
            // so we can test the state machine without the Wi-Fi module.
            IOT_Ring_Rx[iot_rx_wr++] = temp;

            // Advance buffer index
            if(iot_rx_wr >= 32) {
                iot_rx_wr = 0;
            }
            break;
        case 4: break;
        default: break;
    }
}
