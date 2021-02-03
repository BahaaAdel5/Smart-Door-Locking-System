 /******************************************************************************
 *
 * Module: I2C(TWI)
 *
 * File Name: i2c.h
 *
 * Description: Header file for the I2C(TWI) AVR driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/ 


#ifndef I2C_H_
#define I2C_H_

/* ------------------------------INCLUDES-------------------------------------*/
#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	ZERO_PRE,ONE_PRE,TWO_PRE,THREE_PRE
}I2C_Prescale;

typedef struct
{
	uint8 Address;
	uint8 bitRate;
	I2C_Prescale prescale;

}I2C_ConfigType;

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
/* I2C Status Bits in the TWSR Register */
#define TW_START         0x08 // start has been sent
#define TW_REP_START     0x10 // repeated start 
#define TW_MT_SLA_W_ACK  0x18 // Master transmit ( slave address + Write request ) to slave + Ack received from slave
#define TW_MT_SLA_R_ACK  0x40 // Master transmit ( slave address + Read request ) to slave + Ack received from slave
#define TW_MT_DATA_ACK   0x28 // Master transmit data and ACK has been received from Slave.
#define TW_MR_DATA_ACK   0x50 // Master received data and send ACK to slave
#define TW_MR_DATA_NACK  0x58 // Master received data but doesn't send ACK to slave

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description : Function to initialize the I2C driver
 * 	1. Set the master address in case it is a slave.
 * 	2. Set the required bit rate.
 * 	3. Set the required prescale.
 */
void TWI_init(const I2C_ConfigType * I2C_ConfigPtr);

/*
 * Description: Function to send start bit
 */
void TWI_start(void);

/*
 * Description: Function to send stop bit
 */
void TWI_stop(void);

/*
 * Description: Function to send one byte
 */
void TWI_write(uint8 data);

/*
 * Description: Function to read with send Ack
 */
uint8 TWI_readWithACK(void);

/*
 * Description: Function to read without send Ack
 */
uint8 TWI_readWithNACK(void);

/*
 * Description: Function to return current status
 */
uint8 TWI_getStatus(void);


#endif /* I2C_H_ */
