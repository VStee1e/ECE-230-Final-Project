/*
 * UARTcommsDriver.c
 *
 *  Created on: Feb 25, 2025
 *      Author: steelevw
 */
#include "msp.h"
#include <stdio.h>

#define BUFFER_SIZE 50

void UART_Init(void);
void process_received_data(void);

volatile char rx_buffer[BUFFER_SIZE];
volatile uint8_t rx_index = 0;

int main(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // Stop watchdog timer
    UART_Init();

    while (1) {
        // Main loop does nothing; data is received via interrupt
    }
}

void UART_Init(void) {
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Put UART in reset mode
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST | EUSCI_A_CTLW0_SSEL__SMCLK; // Use SMCLK

    EUSCI_A0->BRW = 26; // Baud rate: 9600
    EUSCI_A0->MCTLW = (0 << EUSCI_A_MCTLW_BRF_OFS) | EUSCI_A_MCTLW_OS16;

    P1->SEL0 |= BIT2 | BIT3; // Set P1.2 as TX and P1.3 as RX
    P1->SEL1 &= ~(BIT2 | BIT3);

    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST; // Initialize UART
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE; // Enable RX interrupt

    NVIC_EnableIRQ(EUSCIA0_IRQn); // Enable interrupt in NVIC
    __enable_irq(); // Global interrupt enable
}

//// UART Interrupt Service Routine (ISR)
//void EUSCIA0_IRQHandler(void) {
//    if (EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG) { // RX interrupt flag
//        char receivedChar = EUSCI_A0->RXBUF;
//        if (rx_index < BUFFER_SIZE - 1) {
//            rx_buffer[rx_index++] = receivedChar;
//            if (receivedChar == '\n') { // End of message
//                rx_buffer[rx_index] = '\0'; // Null-terminate string
//                process_received_data(); // Process received message
//                rx_index = 0; // Reset buffer index
//            }
//        }
//    }
//}

// Function to handle received data (print to console for now)
void process_received_data(void) {
    printf("Received: %s", rx_buffer);
}


