//File Name: InputCaptureECE230winter25template.c
//Author: Jianjian Song
//Date: January 29, 2025
//ECE230 Winter 2024-2025
//This is a template file. There is a TODO list to be completed by students.
//Purpose: configure Timer A0 CCR1 to be input capture mode
//on input pin P2.5
// Timer A0 CCR2 Capture Mode Configuration
// Timer A0 CCR2 CCIxA Selects external pin P2.4

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

#include <InputCaptureECE230winter25.h>
#include <msp.h>

//configure Timer A0 CCR2 for input capture on P2.5

void InputCaptureConfiguration_TA02(void) {

//trigger pin on HC-SR04 to start eight 40kHz burst
//DONE - configure InputCaptureTrigger as general output pin
InputCaptureTriggerPort->DIR |= InputCaptureTrigger;
InputCaptureTriggerPort->SEL0 &= ~InputCaptureTrigger;
InputCaptureTriggerPort->SEL1 &= ~InputCaptureTrigger;
//InputCaptureTriggerPort->REN &= ~InputCaptureTrigger;


 //Configuring P2.5 as peripheral input for capture
 //Configure InputCapturePin as secondary-mode input pin with no pull-up or pull-down
InputCapturePort->SEL0 |= InputCapturePin;
InputCapturePort->SEL1 &= ~InputCapturePin;
InputCapturePort->DIR &= ~InputCapturePin;
InputCapturePort->REN &= ~InputCapturePin;

//Timer A0 continuous mode at 48MHz SMCLK and 48 prescaler
//Use SMCLK AT at 48 MHz, continuous mode, interrupt disabled
//bits9-8=0b10 SMCLK, bits7-6=ID=0b11, bits5-4=0b10 CONTINUOUS MODE, bit1=0 NO INTERRUPT
        TIMER_A0->CTL=0x2E4;   //bits7-6=ID
//bits2-0 = TAIDEX = 0b101 = divide by 6
//Prescaler  2^ID(TAIDEX+1)=2^3(5+1)=8X6=48 Timer clock = 48MHz/48=1MHz
//maximum distance is 343meters(10^-6)((2^16)/2)/1MHz=11.239meters
        TIMER_A0->EX0 = 0b101;    //bits2-0=TAIDEX
//Configuring Capture Mode: CCR2, Rising edge, CCIxA pin = P2.5, synchronous, no interrupt
//bits15-14=01 on rising edge bits13-12=00 CCIxA bit8=1 for capture mode.
//bit4=0 to disable interrupt. bit0=CCIFG capture interrupt flag
        TIMER_A0->CCTL[2]=0x4100;
}

#define TimerAClock 1    //MHz
//maximum distance is 343meters(10^-6)((2^16)/2)/1MHz=11.239meters

//Input capture on rising edge and then falling edge
//to find count difference corresponding to echo time
//input capture on Timer A0 CCR2 on P2.5

float getEchoPulse_TA02(void)
{
#define NUMBER_TIMER_CAPTURES 2
static uint_fast16_t timerAcaptureValues[NUMBER_TIMER_CAPTURES];
float period_measured;
unsigned int count;

    //send negative or falling trigger pulse to start measurement.
   //Set output pin HIGH
   InputCaptureTriggerPort->OUT |= InputCaptureTrigger; //trigger output pin
   for (count=0; count<500;count++);  //10 microsecond minimum trigger pulse
   //clear output pin to LOW
   InputCaptureTriggerPort->OUT &= ~InputCaptureTrigger; //trigger output pin
//capture on rising edge first by default
//clear capture flag, i.e., interrupt flag CCIFG
    TIMER_A0->CCTL[1]&=~TIMER_A_CCTLN_CCIFG;
//    TIMER_A0->CCTL[1]=(TIMER_A0->CCTL[1])&0xFFFE;

//first capture, wait for capture to occur when bit 0 CCIFG=1
    while(((TIMER_A0->CCTL[2])&TIMER_A_CCTLN_CCIFG)!=TIMER_A_CCTLN_CCIFG) {};
//clear capture flag, i.e., interrupt flag CCIFG
    TIMER_A0->CCTL[2]&=~TIMER_A_CCTLN_CCIFG;
//get first count value
    timerAcaptureValues[0] = TIMER_A0->CCR[2];

//set to capture on falling edge by toggling bits 15 and 14
   TIMER_A0->CCTL[2]=TIMER_A0->CCTL[2]^0b1100000000000000;
//second capture, wait for falling edge capture to occur when bit 0 CCIFG=1
   while(((TIMER_A0->CCTL[2])&TIMER_A_CCTLN_CCIFG)!=TIMER_A_CCTLN_CCIFG) {};
//clear capture flag, i.e., interrupt flag CCIFG
   TIMER_A0->CCTL[2]&=~TIMER_A_CCTLN_CCIFG;
//get second count value
   timerAcaptureValues[1] = TIMER_A0->CCR[2];
   period_measured = (float) abs(timerAcaptureValues[0]-timerAcaptureValues[1])/TimerAClock;
// return to rising edge by toggling bits 15 and 14
   TIMER_A0->CCTL[2]=TIMER_A0->CCTL[2]^0b1100000000000000;

   return period_measured;
}

