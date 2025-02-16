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
#include "csHFXT.h"
#include "csLFXT.h"


//TEST COMMIT!!
/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
}
