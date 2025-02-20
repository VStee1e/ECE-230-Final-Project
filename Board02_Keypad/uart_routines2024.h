//uart_routines2024.h
//Jianjian Song
//ECE230-01/02
//Date: January 28, 2024

#include <msp.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

void ConfigureUART_A0(void);
void SendCharArray_A0(char *Buffer);
//get a ASCII character from UART
//this is a blocking call
char GetChar_A0(void);

void ConfigureUART_A1(void);
void SendCharArray_A1(char *Buffer);
//get a ASCII character from UART
//this is a blocking call
char GetChar_A1(void);


void ConfigureUART_A3(void);
void SendCharArray_A3(char *Buffer);
//get a ASCII character from UART
//this is a blocking call
char GetChar_A3(void);

