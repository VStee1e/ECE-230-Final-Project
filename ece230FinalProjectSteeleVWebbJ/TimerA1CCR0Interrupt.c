/*
 * TimerA1CCR0Interrupt.c
 *
 *  Created on: April 19, 2023
 *      Author: song
 */
//Sample two analog channels at 10Hz with Timer A1 Up mode interrupt
#include <ece230w25Project4template.h>

uint16_t POTvalue;  //digital value from POT
uint16_t PhotoCellvalue;  //digital value from POT
#define PHOTOCELL_THRETHOLD 2.5*4096/3.5
PotState POTstatus;

//Timer A1 CCR0 interrupt at 10Hz with ACLK at 32kHz
#define  Frequency20Hz 1600  // 50ms*32/ms = 1600
void ConfigureTimerA1CCROInterrupt(void) {

    /* Configure Timer_A1 and CCRs */
     // Set initial period in CCR0 register. This assumes timer starts at 0
     TIMER_A1->CCR[0] = Frequency20Hz;
     // Configure CCR0 for Compare mode with interrupt enabled (no output mode - 0)
     // TODO configure CCR0
     TIMER_A1->CCTL[0]=0X0010;   //0b0000 0000 0001 CCIE=1 0000
     // Configure Timer_A1 in UP Mode with source ACLK prescale 1:1 and no interrupt
     // TODO configure Timer_A1: ACLK at 32kHz, UP mode, TACLR=1 bit 2
     TIMER_A1->CTL=0x0112;  //0b0000 0001 ACLK 0001 UP 0100

     /* Configure global interrupts and NVIC */
     // TODO enable interrupt by setting IRQ bit in NVIC ISER0 register

     NVIC->ISER[0] |= (1)<<TA1_0_IRQn;
}

// Timer A1 CCR0 interrupt service routine
//This interrupt occurs at 20Hz to update POT value
void TA1_0_IRQHandler(void)
{
    static uint16_t POTvalueOld;
    /* Not necessary to check which flag is set because only one IRQ
     *  mapped to this interrupt vector     */
    if(TIMER_A1->CCTL[0] & TIMER_A_CCTLN_CCIFG)
    {
    // TODO clear timer compare flag in TA3CCTL0
        TIMER_A1->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;  //clear interrupt flag
        POTvalueOld = POTvalue;
        ADC14->CTL0 |=0b11; //start conversion
//ADC14_IFGR0_IFG1 is set when ADC14MEM2 is loaded with conversion result
//The flag is cleared when ADC14MEM2 register is read
        while((ADC14->IFGR0 & ADC14_IFGR0_IFG1)==0) {}; //wait for sequence conversion to be over
        POTvalue = ADC14->MEM[1];   //IFG1 flag is cleared
        PhotoCellvalue = ADC14->MEM[2];
        if(POTvalueOld==POTvalue) POTstatus=NoCHANGE;
        else POTstatus=CHANGED;
    }
}
