//File Name: InputCaptureECE230winter25.h
//Author: Jianjian Song
//Date: January 29, 2025
//ECE230 Winter 2024-2025

#include <msp.h>

/*
*                MSP432P4111
*             ----------------------
*         /|\|                      |
*          | |                      |
*          --|RST         P1.0      |---> P1.0 LED, sampling and distance indicator
*            |                      |
*            |            P2.5      |--- TA0.2 SRF04 Echo Pulse, CCR2 input capture
*            |            P1.7      |---> Trigger pulse on SRF04
*            |                      |
*
 */
//P1.6
#define InputCaptureTriggerPort P1
#define InputCaptureTrigger BIT7

//P2.4/PM_TA0.1/L23 20
//P2.5/PM_TA0.2/L22 21
//P2.6/PM_TA0.3/L21 22
//P2.7/PM_TA0.4/L2

#define InputCapturePort P2
#define InputCapturePin BIT5

void InputCaptureConfiguration_TA02(void);
float getEchoPulse_TA02(void);
