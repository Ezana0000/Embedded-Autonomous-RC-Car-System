//------------------------------------------------------------------------------
//
//  Description: Serial communication logic for UCA0 and UCA1.
//  Handles the ring buffers, string parsing, and sending data to the ESP32.
//
//  Ezana
//  April 2026
//  Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------

#include "msp430.h"
#include <string.h>
#include "macros.h"

volatile char IOT_Ring_Rx[32];
volatile unsigned int iot_rx_wr = 0;

volatile char ssid_display[11] = "Connecting";
volatile unsigned int ssid_index = 0;
volatile char ssid_record_flag = 0;
volatile char ip_display1[11] = "          ";
volatile char ip_display2[11] = "          ";
volatile unsigned int ip_record_flag = 0;
volatile unsigned int period_count = 0;
volatile unsigned int ip_index1 = 0;
volatile unsigned int ip_index2 = 0;

void Transmit_UCA0_String(char *str) {
    while (*str) {
        while (!(UCA0IFG & UCTXIFG)); // Wait for TX buffer clearance
        UCA0TXBUF = *str;
        str++;
    }
}

//------------------------------------------------------------------------------
// UART Interface Instantiation
//------------------------------------------------------------------------------
void Init_Serial_UCA0(void){
    UCA0CTLW0 = UCSWRST;            // Put eUSCI in reset
    UCA0CTLW0 |= UCSSEL__SMCLK;     // Set SMCLK as source
    UCA0BRW = 4;                    // 115,200 Baud
    UCA0MCTLW = 0x5551;
    UCA0CTLW0 &= ~UCSWRST;          // Release from reset
    UCA0IE |= UCRXIE;               // Enable RX interrupt
}

void Init_Serial_UCA1(void){
    UCA1CTLW0 = UCSWRST;            // Put eUSCI in reset
    UCA1CTLW0 |= UCSSEL__SMCLK;     // Set SMCLK as source
    UCA1BRW = 4;                    // 115,200 Baud
    UCA1MCTLW = 0x5551;
    UCA1CTLW0 &= ~UCSWRST;          // Release from reset
    UCA1IE |= UCRXIE;               // Enable RX interrupt
}

//------------------------------------------------------------------------------
// Serial Data Service Routines
//------------------------------------------------------------------------------
#pragma vector = EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void){
    if (UCA0IFG & UCRXIFG) {
        char temp = UCA0RXBUF;

        // Route module serial text stream to USB terminal
        while (!(UCA1IFG & UCTXIFG));
        UCA1TXBUF = temp;

        IOT_Ring_Rx[iot_rx_wr] = temp;

        // Monitor stream for SSID
        if (iot_rx_wr >= 1 && IOT_Ring_Rx[(iot_rx_wr-1)%32] == ':' && temp == '"') {
            ssid_record_flag = 1; ssid_index = 0;
        } else if (ssid_record_flag) {
            if (temp == '"' || ssid_index >= 10) ssid_record_flag = 0;
            else ssid_display[ssid_index++] = temp;
        }

        // Monitor stream for Device IP Allocation
        if (ip_record_flag == 0 && iot_rx_wr >= 2) {
            if (IOT_Ring_Rx[(iot_rx_wr-2)%32] == 'P' && IOT_Ring_Rx[(iot_rx_wr-1)%32] == ',' && temp == '"') {
                ip_record_flag = 1; period_count = 0; ip_index1 = 0; ip_index2 = 0;
            }
        } else if (ip_record_flag) {
            if (temp == '"') ip_record_flag = 0;
            else {
                if (temp == '.') period_count++;
                if (period_count < 2 && ip_index1 < 10) ip_display1[ip_index1++] = temp;
                else if (period_count >= 2 && ip_index2 < 10) ip_display2[ip_index2++] = temp;
            }
        }
        iot_rx_wr = (iot_rx_wr + 1) % 32;
    }
}

#pragma vector = EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void){
    if (UCA1IFG & UCRXIFG) {
        char temp = UCA1RXBUF;

        while (!(UCA0IFG & UCTXIFG));
        UCA0TXBUF = temp;

        IOT_Ring_Rx[iot_rx_wr] = temp;
        iot_rx_wr = (iot_rx_wr + 1) % 32;
    }
}
