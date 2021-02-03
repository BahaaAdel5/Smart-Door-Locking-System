 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

/* ------------------------------INCLUDES-------------------------------------*/
#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	DISABLED,RESERVED,EVEN_PARITY,ODD_PARITY
}UART_parityMode;

typedef enum
{
	ONE_BIT,TWO_BITS
}UART_stopBits;

typedef enum
{
	ZERO,ONE,TWO,THREE
}UART_UCSZ_1_0;

typedef enum
{
	ZEROO,ONEE
}UART_UCSZ_2;

typedef struct
{
	uint16 baudRate;
	UART_parityMode parityMode;
	UART_stopBits stopBits;
	UART_UCSZ_1_0 UCSZ_1_0;
	UART_UCSZ_2 UCSZ_2;
}UART_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description : Function to initialize the UART driver
 * 	1. Set the transmission speed.
 * 	2. Set the bits data mode.
 * 	3. Set baud rate prescale.
 * 	4. Set stop bits
 * 	5. Set parity mode
 */
void UART_init(const UART_ConfigType * UART_ConfigPtr);

/*
 *  Description: Function to send one byte
 */
void UART_sendByte(const uint8 data);

/*
 *  Description: Function to receive one byte
 */
uint8 UART_recieveByte(void);

/*
 *  Description: Function to send string
 */
void UART_sendString(const uint8 *Str);

/*
 *  Description: Function to receive string
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
