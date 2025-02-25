/*
 * UARTcommsDriver.h
 *
 *  Created on: Feb 25, 2025
 *      Author: steelevw
 */

#ifndef UARTCOMMSDRIVER_H_
#define UARTCOMMSDRIVER_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "msp.h"
#include <string.h>

extern void UART_Init(void);

extern void process_received_data(void);

#endif /* UARTCOMMSDRIVER_H_ */
