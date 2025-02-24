/*  ECE230 Final Project main.c File
 * Created by Vance Steele and John Webb
 * Date: Feb 16, 2025
 *
 * Description: Uses ultrasonic sensor and interrupts to capture and trigger LCD screen,
 *              speaker, and various LEDS, as well as turn a servo motor. A keypad is
 *              then used with interrupts to disable the speaker and LEDs, while also
 *              changing the LCD display and turning the servo motor back to its
 *              starting position. This version does not have the LCD screen
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
#include "servoDriver.h"
#include "keypadscan_subroutines_v1.h"

/**
 * main.c
 */
#define SOUNDSPEED 34300.0*pow(10,-6) //centimeters per microsecond
#define THRESHOLD 30 //centimeters to turn on and off LED

enum Status
{
    NO, YES
};
extern char NewKeyPressed;
extern char FoundKey;

int main(void)
{
#define DELAYTIME 2000000
    unsigned int delaycount;
    float ObjectDistance;
    float PulseWidth;
    volatile uint32_t degreeLoop = 0;
    char Buffer[200];

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
    //Use External 48MHz oscillator; Set MCLK at 48 MHz for CPU; Set SMCLK at 48 MHz
    configHFXT();    //LED1 to indicate distance
    configLFXT();

    for (delaycount = 0; delaycount < DELAYTIME; delaycount++);

    InputCaptureConfiguration_TA02();
    configSpeaker();
    ConfigureServo();
    //UART A0
    ConfigureUART_A0();
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
            }
            speakerBlare();

            if (FoundKey == 1)
            {
                for (degreeLoop = 10; degreeLoop > 0; degreeLoop--)
                {
                    decrementTenDegree();
                }
                speakerOff();
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
