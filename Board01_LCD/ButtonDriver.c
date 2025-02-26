/*
 * ButtonDriver.c
 * Code reused and modified from Project 2
 *  Created on: Feb 25, 2025
 *      Author: steelevw
 */

#include "msp.h"
#include "ButtonDriver.h"

void debounce(void)
{
unsigned int Counter2;
Counter2 = 1825;
while (Counter2 != 0)
{
    Counter2--;
}
//deley time for debouncing switches
} //end debounce()

SwitchState CheckSwitch1(void)
{
//If pressed, debounce, turn off the current LED and
//check for button release, debounce, return Pressed
//If not pressed, return NotPressed
if (SwitchPort->IN & Switch1)
    return NotPressed;
debounce();
while ((SwitchPort->IN & Switch1) == 0)
{
}
debounce();
return Pressed;

} // end CheckS1()

void InitializeSwitches(void)
{
SwitchPort->SEL0 &= ~(Switch1);
SwitchPort->SEL1 &= ~(Switch1);
SwitchPort->DIR |= ~(Switch1);

SwitchPort->OUT |= (Switch1);
SwitchPort->REN |= (Switch1);

//set S1 to be intput and pullup

} //end InitializeSwitches(void)
