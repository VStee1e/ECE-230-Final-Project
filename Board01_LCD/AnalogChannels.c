/*
 * AnalogChannels.c
 * ATD configuration functions
 * Created on: January 26, 2025
 * Author: Jianjian Song
 * ECE230 Winter 2024-2025
 */
#include <ece230w25Project4template.h>

//Potentiometer definition
//channel bits 4-0 = 0b00001 for A1, P5.4
//#define PotPort P5
//#define PotPin   BIT4    //P5.4
//#define PotChannelNumber 0b00001
//#define PotResultMemory  0b00001

//channel bits 4-0 = 0b01111 for A15, P6.0
//#define PhotocellPort P6
//#define PhotocellPin   BIT0    //P6.0

//Photocell definition
//#define PhotocellPort P6
//#define PhotocellPin   BIT1    //P6.1

//Configure ADC14 to get voltages from one analog channel
void ConfigureAnalogChannel(DIO_PORT_Odd_Interruptable_Type * AnalogPort, char AnalogPin, char Analog_Channel, char Memory_Address) {

    AnalogPort->DIR &= ~AnalogPin;
    AnalogPort->SEL0 |= AnalogPin;
    AnalogPort->SEL1 |= AnalogPin;

     /* Configure ADC (CTL0 and CTL1) registers for:
      *      clock source - default MODCLK, clock prescale 1:1,
      *      sample input signal (SHI) source - software controlled (ADC14SC),
      *      Pulse Sample mode with sampling period of 16 ADC14CLK cycles,
      *      Single-channel, single-conversion mode, 12-bit resolution,
      *      ADC14 conversion start address ADC14MEM1, and Low-power mode
      */

     ADC14->CTL0 = ADC14_CTL0_SHP                // Pulse Sample Mode
                     | ADC14_CTL0_SHT0__16       // 16 cycle sample-and-hold time (for ADC14MEM1)
                     | ADC14_CTL0_PDIV__1        // Predivide by 1
                     | ADC14_CTL0_DIV__1         // /1 clock divider
                     | ADC14_CTL0_SHS_0          // ADC14SC bit sample-and-hold source select
                     | ADC14_CTL0_SSEL__MODCLK   // clock source select MODCLK
                     | ADC14_CTL0_CONSEQ_0       // Single-channel, single-conversion mode
  //                   | ADC14_CTL0_CONSEQ_1   /*!< Sequence-of-channels */
                     | ADC14_CTL0_ON;            // ADC14 on

     ADC14->CTL1 = ADC14_CTL1_RES__12BIT         // 12-bit conversion results
             | (Memory_Address << ADC14_CTL1_CSTARTADD_OFS) //start address of conversion memory
             | ADC14_CTL1_PWRMD_2;               // Low-power mode

     // TODO Configure ADC14MCTL1 as storage register for result
     //          Single-ended mode with Vref+ = Vcc and Vref- = Vss,
     //          Input channel - A1, and comparator window disabled

//     ADC14->MCTL[1] = 0x0000 | Analog_Channel;
     ADC14->MCTL[1] |= 0x0001;
}

//Configure ADC14 to get voltages from two channels
//void TA1_0_IRQHandler(void) will read the two channels and return their ATD values
void ConfigureTwoAnalogInputChannels(void) {

    PotPort->DIR &= ~PotPin;
    PotPort->SEL0 |= PotPin;
    PotPort->SEL1 |= PotPin;

    PhotocellPort->DIR &= ~PhotocellPin;
    PhotocellPort->SEL0 |= PhotocellPin;
    PhotocellPort->SEL1 |= PhotocellPin;

    /* Configure ADC (CTL0 and CTL1) registers for:
     *      clock source - default MODCLK, clock prescale 1:1,
     *      sample input signal (SHI) source - software controlled (ADC14SC),
     *      Pulse Sample mode with sampling period of 16 ADC14CLK cycles,
     *      Single-channel, single-conversion mode, 12-bit resolution,
     *      ADC14 conversion start address ADC14MEM1, and Low-power mode
     */
    ADC14->CTL0 = ADC14_CTL0_SHP                // Pulse Sample Mode
                     | ADC14_CTL0_SHT0__16       // 16 cycle sample-and-hold time (for ADC14MEM1)
                     | ADC14_CTL0_PDIV__1        // Predivide by 1
                     | ADC14_CTL0_DIV__1         // /1 clock divider
                     | ADC14_CTL0_SHS_0          // ADC14SC bit sample-and-hold source select
                     | ADC14_CTL0_SSEL__MODCLK   // clock source select MODCLK
   //                  | ADC14_CTL0_CONSEQ_0       // Single-channel, single-conversion mode
                     | ADC14_CTL0_ON;            // ADC14 on

    ADC14->CTL0 |= ADC14_CTL0_MSC;  /*!< ADC14 multiple sample and conversion */

    ADC14->CTL0 |= ADC14_CTL0_CONSEQ_1;         // Sequence of channels

    ADC14->CTL1 = ADC14_CTL1_RES__12BIT         // 12-bit conversion results
            | (0x1 << ADC14_CTL1_CSTARTADD_OFS) // ADC14MEM1 - conversion start address
            | ADC14_CTL1_PWRMD_2;               // Low-power mode

    // TODO Configure ADC14MCTL1 as storage register for result
    //          Single-ended mode with Vref+ = Vcc and Vref- = Vss,
    //          Input channel - A15, and comparator window disabled
    //P5.4
    ADC14->MCTL[1] = PotChannelNumber;
    ADC14->MCTL[1] = 0x000F;    //channel bits 4-0 = 0b01111 for A15, P6.0
    ADC14->MCTL[2] = 0x000E;    //channel bits 4-0 = 0b10000 for A14, P6.1

    ADC14->MCTL[2] |= 0b10000000;   //turn on End of Sequence bit
//    ADC14->MCTL[2] |= ADC14_MCTLN_EOS;
}
