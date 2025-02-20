//keypadscan2024winter.c
//Jianjian Song
//ECE230-01/02
//Date: January 28, 2024
//Test code to verify
//Purpose: Scan a 4x4 keypad to provide key numbers from 1 to 16
//         if a corresponding key is pressed
//         key_number is 0 if no key is pressed
/*
 * MSP432P4111 GPIO -
 * Port 4 Input Interrupt on input pins 4, 5, 6, 7
 *
 *                MSP432P4111
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST    output P4.0|---> key0
 *            |       output P4.1|---> key1
 *            |       output P4.2|---> key2
 *            |       output P4.3|---> key3
 *            | pullup input P4.4|---> key4
 *            | pullup input P4.5|---> key5
 *            | pullup input P4.6|---> key6
 *            | pullup input P4.7|---> key7
 *            |                  |
 *            |                  |
 ***************************************************************/
//To reroute printf() to UART0, add printfOveride.c
//Reference:
// https://43oh.com/2015/05/how-to-add-printf-support-for-the-msp432-launchpad-serial-port/

/* DriverLib Includes */
#include <keypadscan_subroutines_v1.h>
#include <msp.h>
/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "csHFXT.h"

#include "uart_routines2024.h"

enum Status {NO, YES};
extern char NewKeyPressed;
extern char FoundKey;

int main(void)
{
    /* Stop Watchdog timer */
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    configHFXT();
    ConfigureUART_A0();
    printf("keyscan started: press a key on your 4x4 keypad ....\r\n");
    kepadconfiguration();
    
// Enable global interrupt
    __enable_irq();
//clear keypad output pins to be 0 to be ready for input interrupt
//Do not change input pin values
   // KeypadPort->OUT=(KeypadPort->OUT & (~KeypadOutputPins));
        KeypadPort->OUT=(KeypadPort->OUT & (~KeypadOutputPins)) | (KeypadInputPins & KeypadOutputPins);
    while (1)
    {
        if(NewKeyPressed==YES) {
            NewKeyPressed=NO;
            //TODO instead printing foundKey, do something else.
            printf("Key Found: %d \r\n", FoundKey);
            // if PASSKEY:
            // trigger interrupt handler that stops alarm
            // communicate with other msp board
        }
    }
} //end main()
