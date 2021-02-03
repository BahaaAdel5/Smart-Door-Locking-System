/*
 * timer0.h
 *
 *  Created on: Nov 30, 2020
 *      Author: BAHAA
 */

#ifndef TIMER0_H_
#define TIMER0_H_

/* ------------------------------INCLUDES-------------------------------------*/
#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	NORMAL = 16,CTC = 19,FASTPWM = 13
}Timer_Mode;

typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}Timer_Prescale;

typedef struct
{
	uint8 initialValue;
	uint8 compareValue;
	Timer_Mode mode;
	Timer_Prescale prescale;
}Timer0_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description : Function to initialize the TIMER0 driver
 *	1.set timer initial value
 *	2.set compare value
 *	3.configure timer control registers
 *	4.enable global interrupt
 */
void timer0_init_normal_mode(const Timer0_ConfigType * Timer0_ConfigPtr);

/*
 * Description: Function to set the Call Back function address.
 */
void timer0_setCallBack(void(*a_ptr)(void));

#endif /* TIMER0_H_ */
