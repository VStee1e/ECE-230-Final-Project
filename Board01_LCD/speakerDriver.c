/*! \file */
/******************************************************************************
 * MSP432 Lab Exercise 4-2
 *
 * Description: Using timers and compare/capture modules to toggle output at
 *               frequency of musical notes to drive speaker. Device configuration
 *               to enable operation at MCLK=48MHz using HFXT source.
 * Author:
 * Last-modified:
 *
 * An external HF crystal between HFXIN & HFXOUT is required for MCLK,SMCLK
 *
 *                MSP432P411x
 *             -----------------
 *         /|\|                 |
 *          | |                 |
 *          --|RST              |
 *            |            P2.4 |--> CCR1 - speaker
 *            |                 |
 *            |            PJ.2 |------
 *            |                 |     |
 *            |                 |    HFXT @ 48MHz
 *            |                 |     |
 *            |            PJ.3 |------
 *
 *******************************************************************************/
#include "msp.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include "csHFXT.h"
#include "speakerDriver.h"
#include "UARTcommsDriver.h"

///* Note defines */
//#define NOTECNT 3
//// Note A4 - 440 Hz, B4 - 493.88 Hz, C5 - 523.26 Hz
#define NOTEA4  27273
#define NOTEB4  24297
#define NOTEC5  22933
////#define notes[] = {NOTEA4, NOTEB4, NOTEC5}
const uint16_t notePeriods[3] = { NOTEA4, NOTEB4, NOTEC5 };

void configSpeaker(void)
{
    configHFXT();
    /* Configure GPIO for speaker */
    P2->DIR |= BIT4;            // set P2.4 as output
    P2->SEL0 |= BIT4;           // P2.4 set to TA0.1
    P2->SEL1 &= ~BIT4;

    /* Configure Timer_A0 and CCRs */
    // Set Period in CCR0 register
    TIMER_A0->CCR[0] = NOTEA4 - 1;
    // Set high pulse-width in CCR1 register (determines duty cycle)
    TIMER_A0->CCR[3] = (NOTEA4 / 2) - 1;
    // Configure CCR1 for Compare mode with Set/Reset output
    //          - sets when Timer_A0 == CCR1
    //          - resets when Timer_A0 == CCR0
    // Configure CCR3
    TIMER_A0->CCTL[3] = 0b01100000;

    // Configure Timer_A0 in Up Mode with source SMCLK prescale 4:1
    //      Tick rate will be (48MHz/4) with rollover at value in CCR0
    // Configure TIMER_A0
    TIMER_A0->CTL = 0b001010010100;
}

void speakerBlare(void)
{
    configHFXT();
    UART_Init();

    volatile uint32_t weakDelay = 0;
    int noteIndex = 0;

    /* Stop Watchdog timer */
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    /* Sets MCLK and SMCLK to 48MHz HFXT */
    configHFXT();

    /* Configure GPIO for speaker */
    P2->DIR |= BIT4;            // set P2.4 as output
    P2->SEL0 |= BIT4;           // P2.4 set to TA0.1
    P2->SEL1 &= ~BIT4;

    /* Configure Timer_A0 and CCRs */
    // Set Period in CCR0 register
    TIMER_A0->CCR[0] = NOTEA4 - 1;
    // Set high pulse-width in CCR1 register (determines duty cycle)
    TIMER_A0->CCR[1] = (NOTEA4 / 2) - 1;
    // Configure CCR1 for Compare mode with Set/Reset output
    //          - sets when Timer_A0 == CCR1
    //          - resets when Timer_A0 == CCR0
    // Configure CCR1
    TIMER_A0->CCTL[1] = 0b01100000;

    // Configure Timer_A0 in Up Mode with source SMCLK prescale 4:1
    //      Tick rate will be (48MHz/4) with rollover at value in CCR0
    // Configure Timer_A0
    TIMER_A0->CTL = 0b001010010100;

    while (message_received == 0)
        {
            for (weakDelay = 1000000; weakDelay > 0; weakDelay--)
            {

            }
            noteIndex = (noteIndex + 1) % NOTECNT;
            TIMER_A0->CCR[0] = notePeriods[noteIndex] - 1;
            TIMER_A0->CCR[1] = (notePeriods[noteIndex] / 2) - 1;
        }
}

void speakerOff(void)
{
    configHFXT();
    UART_Init();

    volatile uint32_t weakDelay = 0;
    int noteIndex = 0;

    /* Stop Watchdog timer */
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    /* Sets MCLK and SMCLK to 48MHz HFXT */
    configHFXT();

    /* Configure GPIO for speaker */
    P2->DIR |= BIT4;            // set P2.4 as output
    P2->SEL0 |= BIT4;           // P2.4 set to TA0.1
    P2->SEL1 &= ~BIT4;

    /* Configure Timer_A0 and CCRs */
    // Set Period in CCR0 register
    TIMER_A0->CCR[0] = NOTEA4 - 1;
    // Set high pulse-width in CCR1 register (determines duty cycle)
    TIMER_A0->CCR[1] = (NOTEA4 / 2) - 1;
    // Configure CCR1 for Compare mode with Set/Reset output
    //          - sets when Timer_A0 == CCR1
    //          - resets when Timer_A0 == CCR0
    // DONE configure CCR1
    TIMER_A0->CCTL[1] = 0b01100000;

    // Configure Timer_A0 in Up Mode with source SMCLK prescale 4:1
    //      Tick rate will be (48MHz/4) with rollover at value in CCR0
    // DONE configure Timer_A0
    TIMER_A0->CTL = 0b001010010100;

    while (message_received == 1)
    {
        for (weakDelay = 1000000; weakDelay > 0; weakDelay--)
        {

        }
        noteIndex = (noteIndex + 1) % NOTECNT;
        TIMER_A0->CCR[0] = offNotes[noteIndex] - 1;
        TIMER_A0->CCR[1] = (offNotes[noteIndex] / 2) - 1;
        /* DONE add note changing code (update compare registers) */

    }
}

void EUSCIA0_IRQHandler(void)
{
    if (EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG)
    { // If data received
        char receivedChar = EUSCI_A0->RXBUF; // Read received character

        if (receivedChar == '\n' || rx_index >= BUFFER_SIZE - 1)
        {
            rx_buffer[rx_index] = '\0'; // Null-terminate the string
            message_received = 1;  // Set flag to indicate message arrival
            rx_index = 0;  // Reset index for next message
        }
        else
        {
            rx_buffer[rx_index++] = receivedChar; // Store character in buffer
        }
    }
}
