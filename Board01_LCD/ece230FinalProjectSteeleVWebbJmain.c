/*  ECE230 Final Project main.c File
 * Created by Vance Steele and John Webb
 * Date: Feb 24, 2025
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
#include "servoDriver.h"
#include "lcd8bits.h"
#include "ButtonDriver.h"

/**
 * main.c
 */
#define SOUNDSPEED 34300.0*pow(10,-6) //centimeters per microsecond
#define THRESHOLD 30 //centimeters to turn on and off LED
#define BUFFER_SIZE 50

void UART_Init(void);
void InitializeSwitches(void);
SwitchState CheckSwitch1(void);

int main(void)
{
#define DELAYTIME 2000000
    unsigned int delaycount;
    float ObjectDistance;
    float PulseWidth;
    volatile uint32_t degreeLoop = 0;
    char Buffer[200];
    SwitchState S1Status;

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
    // Use External 48MHz oscillator; Set MCLK at 48 MHz for CPU; Set SMCLK at 48 MHz
    configHFXT();
    configLFXT();
    InitializeSwitches();

    S1Status = NotPressed;

    for (delaycount = 0; delaycount < DELAYTIME; delaycount++)
        ;

    InputCaptureConfiguration_TA02();
    configSpeaker();
    ConfigureServo();
    UART_Init();
    lcd8bits_init();

    __enable_irq();  // Enable global interrupts

    while (1)
    {
        PulseWidth = getEchoPulse_TA02();
        ObjectDistance = (float) SOUNDSPEED * PulseWidth / 2.0;

        if (ObjectDistance < THRESHOLD)
        {
            printf("\r\n object distance in %4.1f (cm)  pulse width %4.1f (us)",
                   ObjectDistance, PulseWidth);
            printf("\r\n The distance is less than %d cm.\r\n", THRESHOLD);

            lcd_SetLineNumber(FirstLine);
            sprintf(Buffer, "INTRUDER ALERT!!");
            lcd_puts(Buffer);
            sprintf(Buffer, "FEAR FOR YO LIFE");
            lcd_SetLineNumber(SecondLine);
            lcd_puts(Buffer);

            for (degreeLoop = 10; degreeLoop > 0; degreeLoop--)
            {
                incrementTenDegree();
            }
            speakerBlare();
        }

        S1Status = CheckSwitch1();
        if (S1Status == Pressed)
        {
            //set S1Status as NotPressed
            //turn on or off CurrentLED
            S1Status = NotPressed;
            // Check if a message was received via UART
            for (degreeLoop = 10; degreeLoop > 0; degreeLoop--)
            {
                decrementTenDegree();
            }
            lcd_SetLineNumber(FirstLine);
            sprintf(Buffer, "CORRECT CODE IN!");
            lcd_puts(Buffer);
            sprintf(Buffer, "ENJOY THE DAY!! :>");
            lcd_SetLineNumber(SecondLine);
            lcd_puts(Buffer);
            speakerOff();
    }

    for (delaycount = 0; delaycount < DELAYTIME; delaycount++)
        ;       ;
}; //end while(1)
} //end main()
