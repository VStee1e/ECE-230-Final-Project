/*  ECE230 Final Project main.c File
 * Created by Vance Steele and John Webb
 * Date: Feb 16, 2025
 *
 * Description: Uses ultrasonic sensor and interrupts to capture and trigger LCD screen,
 *              speaker, and various LEDS, as well as turn a servo motor. A keypad is
 *              then used with interrupts to disable the speaker and LEDs, while also
 *              changing the LCD display and turning the servo motor back to its
 *              starting position.
 *
 */

#include "msp.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "csHFXT.h"
#include "csLFXT.h"
#include <InputCaptureECE230winter25.h>
#include "speakerDriver.h"
#include "servoDriverTemplate_vws.h"
#include "keypadscan_subroutines_v1.h"
#include "UARTcommsDriver.h"
//#include "lcd8bits.h"

//TEST COMMIT!!
/**
 * main.c
 */
#define SOUNDSPEED 34300.0*pow(10,-6) //centimeters per microsecond
#define THRESHOLD 30 //centimeters to turn on and off LED
#define MESSAGE "Hello from Board 1\n"

enum Status
{
    NO, YES
};
extern char NewKeyPressed;
extern char FoundKey;

//void UART_Init(void);
//void UART_SendString(char *str);

int main(void)
{
#define DELAYTIME 2000000
    unsigned int delaycount;
    float ObjectDistance;
    float PulseWidth;
    volatile uint32_t degreeLoop = 0;
    char Buffer[200];
    volatile uint8_t tx_index = 0;
    volatile char message_buffer[] = MESSAGE;

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
    //Use External 48MHz oscillator; Set MCLK at 48 MHz for CPU; Set SMCLK at 48 MHz
    configHFXT();    //LED1 to indicate distance
    configLFXT();

    for (delaycount = 0; delaycount < DELAYTIME; delaycount++)
        ;

    InputCaptureConfiguration_TA02();
    configSpeaker();
    ConfigureServo();
    //UART A0
    ConfigureUART_A0();
    UART_Init();
    //8-bit LCD
//    lcd8bits_init();
    printf("keyscan started: press a key on your 4x4 keypad ....\r\n");
    kepadconfiguration();

    __enable_irq();

    KeypadPort->OUT = (KeypadPort->OUT & (~KeypadOutputPins))
            | (KeypadInputPins & KeypadOutputPins);

    while (1)
    {
        PulseWidth = getEchoPulse_TA02();
        ObjectDistance = (float) SOUNDSPEED * PulseWidth / 2.0;

        if (NewKeyPressed == YES)
        {
            NewKeyPressed = NO;
            //DONE instead printing foundKey, do something else.
            printf("Key Found: %d \r\n", FoundKey);
            // if PASSKEY:
            // trigger interrupt handler that stops alarm
            // communicate with other msp board
        }

        if (ObjectDistance < THRESHOLD)
        {
            printf("\r\n object distance in %4.1f (cm)  pulse width %4.1f (us)",
                   ObjectDistance, PulseWidth);
            printf("\r\n The distance is less than %d cm.\r\n", THRESHOLD); //Replace w interrupt flag

            for (degreeLoop = 10; degreeLoop > 0; degreeLoop--)
            {
                incrementTenDegree();
//                i++;
            }
            speakerBlare();

            if (FoundKey == 1)
            {
                void UART_Init(void);
                void UART_SendString(char *str);
                speakerOff();
                for (degreeLoop = 10; degreeLoop > 0; degreeLoop--)
                {
                    decrementTenDegree();
                    //                i++;
                }
                __delay_cycles(3000000); // Delay before sending again
                tx_index = 0; // Reset index
                EUSCI_A0->TXBUF = message_buffer[tx_index++]; // Start transmission
            }
        }
        else
        {
            printf("\r\n object distance in %4.1f (cm)  pulse width %4.1f (us)",
                   ObjectDistance, PulseWidth);
            printf("\r\n The distance is more than %d cm.\r\n", THRESHOLD); //Replace w interrupt flag

        }
        for (delaycount = 0; delaycount < DELAYTIME; delaycount++)
            ;

    }; //end while(1)
} //end main()

void EUSCIA0_IRQHandler(void) {
    if (EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG) { // TX interrupt flag
        if (tx_index < strlen(MESSAGE)) {
            EUSCI_A0->TXBUF = message_buffer[tx_index++]; // Send next character
        } else {
            EUSCI_A0->IE &= ~EUSCI_A_IE_TXIE; // Disable TX interrupt after message
        }
    }
}
