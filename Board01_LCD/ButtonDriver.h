/*
 * ButtonDriver.h
 *
 *  Created on: Feb 25, 2025
 *      Author: steelevw
 */

#ifndef BUTTONDRIVER_H_
#define BUTTONDRIVER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define SwitchPort P1   //Port 1
#define Switch1   0b00100000    //P1.5

typedef enum _SwitchState
{
    NotPressed, Pressed
} SwitchState;

extern void debounce(void);

extern SwitchState CheckSwitch1(void);

extern void InitializeSwitches(void);

#endif /* BUTTONDRIVER_H_ */
