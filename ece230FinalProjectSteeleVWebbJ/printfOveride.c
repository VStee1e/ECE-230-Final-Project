//File Name: printfOveride.c
//Created on: Jan 28, 2024
//Author: Jianjian Song
//Include this file to reroute printf() from console to UART A0
//to reroute to another UART, change UART in this file
//Reference:
// https://43oh.com/2015/05/how-to-add-printf-support-for-the-msp432-launchpad-serial-port/

#include <msp.h>
#include <stdio.h>
#include <string.h>

int fputc(int _c, register FILE *_fp);
int fputs(const char *_ptr, register FILE *_fp);

int fputc(int _c, register FILE *_fp)
{
  while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
  EUSCI_A0->TXBUF = (unsigned char) _c;

  return((unsigned char)_c);
}

int fputs(const char *_ptr, register FILE *_fp)
{
  unsigned int i, len;

  len = strlen(_ptr);

  for(i=0 ; i<len ; i++)
  {
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = (unsigned char) _ptr[i];
  }

  return len;
}

