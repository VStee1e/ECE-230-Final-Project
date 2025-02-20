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
#include "lcd8bits.h"

//TEST COMMIT!!
/**
 * main.c
 */
#define SOUNDSPEED 34300.0*pow(10,-6) //centimeters per microsecond
#define THRESHOLD 30 //centimeters to turn on and off LED

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

    for (delaycount = 0; delaycount < DELAYTIME; delaycount++)
        ;

    InputCaptureConfiguration_TA02();
    configSpeaker();
    ConfigureServo();
    //UART A0
    ConfigureUART_A0();
    //8-bit LCD
    lcd8bits_init();

    __enable_irq();

    while (1)
    {
        PulseWidth = getEchoPulse_TA02();
        ObjectDistance = (float) SOUNDSPEED * PulseWidth / 2.0;

        if (ObjectDistance < THRESHOLD)
        {
            printf("\r\n object distance in %4.1f (cm)  pulse width %4.1f (us)",
                   ObjectDistance, PulseWidth);
            printf("\r\n The distance is less than %d cm.\r\n", THRESHOLD); //Replace w interrupt flag

            lcd_SetLineNumber(FirstLine);
            sprintf(Buffer, "INTRUDER");
            lcd_puts(Buffer);
            sprintf(Buffer, "ALERT!!");
            lcd_SetLineNumber(SecondLine);
            lcd_puts(Buffer);

            for (degreeLoop = 10; degreeLoop > 0; degreeLoop--)
            {
                incrementTenDegree();
//                i++;
            }
            speakerBlare();
        }
        else
        {
            printf("\r\n object distance in %4.1f (cm)  pulse width %4.1f (us)",
                   ObjectDistance, PulseWidth);
            printf("\r\n The distance is more than %d cm.\r\n", THRESHOLD); //Replace w interrupt flag

            lcd_SetLineNumber(FirstLine);
            sprintf(Buffer, "I <3 YOU");
            lcd_puts(Buffer);
            sprintf(Buffer, "SAMMOUD");
            lcd_SetLineNumber(SecondLine);
            lcd_puts(Buffer);

        }
        for (delaycount = 0; delaycount < DELAYTIME; delaycount++)
            ;
    }; //end while(1)
} //end main()

//void TA2_N_IRQHandler() {
//
//    if()
//
//}

