//===========================================================================
// File Name : serial.c
// Description: UART Initialization, Cross-Communication ISRs, and Baud Swapping
//===========================================================================

#include "msp430.h"
#include <string.h>
#include "functions.h"
#include "LCD.h"
#include "macros.h"

extern char display_line[4][11];
extern volatile unsigned char display_changed;

// --- PROJECT 8 VARIABLES ---
char ad2_message[11] = "          ";      // Array to catch the 10 chars
volatile unsigned int ad2_rx_index = 0;     // Tracks how many chars received
volatile char message_ready = 0;            // Flag to tell main loop a message is done

void Init_Serial_UCA0(void){
    // Configure eUSCI_A0 for UART mode (IoT Module - Connected to AD2)
    UCA0CTLW0 = 0;                  // Put eUSCI in reset
    UCA0CTLW0 |= UCSWRST;           // Set UCSWRST
    UCA0CTLW0 |= UCSSEL__SMCLK;     // Set SMCLK as fBRCLK
    UCA0CTLW0 &= ~UCMSB;            // MSB, LSB select
    UCA0CTLW0 &= ~UCSPB;            // 1 stop bit
    UCA0CTLW0 &= ~UCPEN;            // No Parity
    UCA0CTLW0 &= ~UCSYNC;           // Asynchronous mode
    UCA0CTLW0 &= ~UC7BIT;           // 8-bit data
    UCA0CTLW0 |= UCMODE_0;          // UART mode

    // Default to 115,200 baud
    UCA0BRW = 4;
    UCA0MCTLW = 0x5551;

    UCA0CTLW0 &= ~UCSWRST;          // Release from reset
    UCA0IE |= UCRXIE;               // Enable RX interrupt
}

void Init_Serial_UCA1(void){
    // Configure eUSCI_A1 for UART mode (PC Backdoor)
    UCA1CTLW0 = 0;
    UCA1CTLW0 |= UCSWRST;
    UCA1CTLW0 |= UCSSEL__SMCLK;
    UCA1CTLW0 &= ~UCMSB;
    UCA1CTLW0 &= ~UCSPB;
    UCA1CTLW0 &= ~UCPEN;
    UCA1CTLW0 &= ~UCSYNC;
    UCA1CTLW0 &= ~UC7BIT;
    UCA1CTLW0 |= UCMODE_0;

    // Default to 115,200 baud
    UCA1BRW = 4;
    UCA1MCTLW = 0x5551;

    UCA1CTLW0 &= ~UCSWRST;
    UCA1IE |= UCRXIE;               // Enable RX interrupt
}

// Helper functions to swap Baud Rates on the fly
void Change_Baud_115200(void){
    UCA0CTLW0 |= UCSWRST; // Pause UCA0
    UCA0BRW = 4;
    UCA0MCTLW = 0x5551;
    UCA0CTLW0 &= ~UCSWRST; // Resume
    UCA0IE |= UCRXIE;
}

void Change_Baud_57600(void){
    UCA0CTLW0 |= UCSWRST; // Pause UCA0

    // 57600 baud math for 8MHz clock
    // UCBRx = 8, UCFx = 10 (0xA), UCSx = 0xF7, UCOS16 = 1
    UCA0BRW = 8;
    UCA0MCTLW = 0xF7A1;

    UCA0CTLW0 &= ~UCSWRST; // Resume
    UCA0IE |= UCRXIE;
}

// Function to transmit the array out of UCA0 to the AD2
void Transmit_UCA0_String(char* str) {
    int i = 0;
    while (str[i] != '\0' && i < 10) {
        while (!(UCA0IFG & UCTXIFG)); // Wait until buffer is ready
        UCA0TXBUF = str[i];           // Load character into buffer
        i++;
    }

    // THE FIX: Send ONLY the Line Feed (\n) so Waveforms goes to a new line
    // without printing the weird \r symbol!
    while (!(UCA0IFG & UCTXIFG));
    UCA0TXBUF = '\n';
}
// --- UART INTERRUPT SERVICE ROUTINES ---

#pragma vector = EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void){
    char received_char;
    switch(__even_in_range(UCA0IV, 0x08)){
        case 0: break;                  // No interrupt
        case 2:                         // Vector 2 - RXIFG (Message from AD2)
            received_char = UCA0RXBUF;

            // Ignore carriage returns/newlines sent by the AD2 terminal
            if (received_char == '\r' || received_char == '\n') {
                break;
            }

            // Store the character
            ad2_message[ad2_rx_index] = received_char;
            ad2_rx_index++;

            // If we caught all 10 characters, raise the flag for main.c
            if (ad2_rx_index >= 10) {
                ad2_message[10] = '\0'; // Null-terminate just to be safe
                ad2_rx_index = 0;       // Reset index for the next message
                message_ready = 1;      // FLAG: Tell main loop to update LCD
            }
            break;
        case 4: break;                  // Vector 4 - TXIFG
        default: break;
    }
}

#pragma vector = EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void){
    // Not used in Project 8, left empty safely
    switch(__even_in_range(UCA1IV, 0x08)){
        case 0: break;
        case 2: break;
        case 4: break;
        default: break;
    }
}
