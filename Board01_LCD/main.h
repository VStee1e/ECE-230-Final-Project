/*
 * main.h
 *
 *  Created on: Feb 25, 2025
 *      Author: webbjb
 */

#ifndef MAIN_H_
#define MAIN_H_

#ifdef __cplusplus
extern "C"
{
#endif


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

/**
 * main.c
 */
#define SOUNDSPEED 34300.0*pow(10,-6) //centimeters per microsecond
#define THRESHOLD 30 //centimeters to turn on and off LED
#define BUFFER_SIZE 50

extern void EUSCIA0_IRQHandler(void);

#endif /* MAIN_H_ */
