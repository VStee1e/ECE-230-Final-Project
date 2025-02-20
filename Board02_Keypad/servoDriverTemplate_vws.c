/*! \file */
/*!
 * servoDriverTemplate_jjs.c
 * ECE230-01/02 Winter 2023-2024
 * Date: January 11, 2024
 * Description: Servo motor driver for MSP432P4111 Launchpad.
 *              Assumes SMCLK configured with 48MHz HFXT as source.
 *              Uses TIMER_A2 and P5.6 (TA2.1)
 *
 *                 MSP432P4111
 *             -------------------
 *         /|\|                   |
 *          | |                   |
 *          --|RST                |
 *            |      (TA2.1) P5.6 |----> Servo
 *            |                   |
 *            |              PJ.2 |------
 *       S1-->|P1.1               |     |
 *            |                   |    HFXT @ 48MHz
 *            |                   |     |
 *            |              PJ.3 |------
 *
 */

#include <servoDriverTemplate_vws.h>
#include "msp.h"

/* Global Variables  */
uint16_t pulseWidthTicks = SERVO_MIN_ANGLE;


void ConfigureServo(void) {
    // DONE configure servo pin (P5.6) for primary module function (TA2.1),
    //  output, initially LOW
    ServoControl_Port->DIR |= ServoControl_Pin;
    ServoControl_Port->SEL0 |= ServoControl_Pin;
    ServoControl_Port->SEL1 &= ~(ServoControl_Pin);
    ServoControl_Port->OUT &= ~(ServoControl_Pin);

    /* Configure TIMER_A2 and CCR1 */
    // Set period of TIMER_A2 in CCR0 register for Up Mode
    TIMER_A2->CCR[0] = SERVO_TMR_PERIOD;
    // Set initial positive pulse-width of PWM in CCR1 register
    TIMER_A2->CCR[1] = SERVO_MIN_ANGLE;

    // DONE configure TA0CCR5 for Compare mode, Reset/Set output mode, with interrupt disabled
    TIMER_A2->CCTL[1] = 0x00E4;
    // Configure TIMER_A2 in Up Mode, with source SMCLK, prescale 16:1, and
    //  interrupt disabled  -  tick rate will be 3MHz (for SMCLK = 48MHz)
    // DONE configure TIMER_A2 (requires setting control AND expansion register)
    TIMER_A2->CTL = 0x0290;
    TIMER_A2->EX0 = 0x07;
}

void incrementTenDegree(void) {
    // update pulse-width for <current angle> + <10 degrees>
    pulseWidthTicks += TEN_DEGREE_TICKS;
    if (pulseWidthTicks > SERVO_MAX_ANGLE) {
        pulseWidthTicks = SERVO_MIN_ANGLE;
    }
    // DONE update CCR1 register to set new positive pulse-width
    TIMER_A2->CCR[1] = pulseWidthTicks;
}

void decrementTenDegree(void){
    pulseWidthTicks -= TEN_DEGREE_TICKS;
        if (pulseWidthTicks < SERVO_MIN_ANGLE) {
            pulseWidthTicks = SERVO_MAX_ANGLE;
        }
        // DONE update CCR1 register to set new positive pulse-width
        TIMER_A2->CCR[1] = pulseWidthTicks;
}

#define minRPM 1.0
#define maxRPM 10.0

void setServoAngle(float rpm) {
    float angle_count;
//DONE calculate the angle
//    angle_count = (180/(SERVO_MAX_ANGLE_ANGLE - SERVO_MIN_ANGLE))*(pulseWidthTicks - SERVO_MIN_ANGLE);
    angle_count = (float) SERVO_MIN_ANGLE + (rpm-minRPM)*(SERVO_MAX_ANGLE-SERVO_MIN_ANGLE)/(float)(maxRPM-minRPM);
//DONE set servo angle count value. Cast integers to float to calculate float values correctly
    pulseWidthTicks = (int) angle_count;
    TIMER_A2->CCR[1] = pulseWidthTicks;
       //set servo position
}

