/*
 * UARTcomms.c
 *
 *  Created on: Feb 24, 2025
 *      Author: steelevw
 */
#include "msp.h"
#include <string.h>

//#define MESSAGE "Hello from Board 1\n"

//void UART_Init(void);
//void UART_SendString(char *str);

volatile uint8_t tx_index = 0;
volatile char message_buffer[] = MESSAGE;

//int main(void) {
//    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // Stop watchdog timer
//    UART_Init();
//
//    while (1) {
//        __delay_cycles(3000000); // Delay before sending again
//        tx_index = 0; // Reset index
//        EUSCI_A0->TXBUF = message_buffer[tx_index++]; // Start transmission
//    }
//}

void UART_Init(void) {
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Put UART in reset mode
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST | EUSCI_A_CTLW0_SSEL__SMCLK; // Use SMCLK

    EUSCI_A0->BRW = 26;  // Baud rate: 9600 (assuming 3MHz clock)
    EUSCI_A0->MCTLW = (0 << EUSCI_A_MCTLW_BRF_OFS) | EUSCI_A_MCTLW_OS16;

    P1->SEL0 |= BIT2 | BIT3; // Set P1.2 as TX and P1.3 as RX
    P1->SEL1 &= ~(BIT2 | BIT3);

    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST; // Initialize UART
    EUSCI_A0->IE |= EUSCI_A_IE_TXIE; // Enable TX interrupt

    NVIC_EnableIRQ(EUSCIA0_IRQn); // Enable interrupt in NVIC
    __enable_irq(); // Global interrupt enable
}

// UART Interrupt Service Routine (ISR)
//void EUSCIA0_IRQHandler(void) {
//    if (EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG) { // TX interrupt flag
//        if (tx_index < strlen(MESSAGE)) {
//            EUSCI_A0->TXBUF = message_buffer[tx_index++]; // Send next character
//        } else {
//            EUSCI_A0->IE &= ~EUSCI_A_IE_TXIE; // Disable TX interrupt after message
//        }
//    }
//}

