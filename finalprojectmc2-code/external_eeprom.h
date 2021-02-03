 /******************************************************************************
 *
 * Module: External EEPROM
 *
 * File Name: external_eeprom.h
 *
 * Description: Header file for the External EEPROM Memory
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/


#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_

/* ------------------------------INCLUDES-------------------------------------*/
#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
#define ERROR 0
#define SUCCESS 1

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description : Function to initialize the I2C(TWI) module inside the MC
 */
void EEPROM_init(void);

/*
 * Description : Function to write one byte to EEPROM
 * 	1. Send the Start Bit
 * 	2. Send the device address
 * 	3. Send the required memory location address
 * 	4. write byte to eeprom
 * 	5. Send the Stop Bit
 */
uint8 EEPROM_writeByte(uint16 u16addr,uint8 u8data);

/*
 * Description : Function to read one byte from EEPROM
 * 	1. Send the Start Bit
 * 	2. Send the device address
 * 	3. Send the required memory location address
 * 	4. Send the Repeated Start Bit
 * 	5. Send the device address
 * 	6. Read Byte from Memory without send ACK
 * 	5. Send the Stop Bit
 */
uint8 EEPROM_readByte(uint16 u16addr,uint8 *u8data);
 
#endif /* EXTERNAL_EEPROM_H_ */
