/*
 * timer0.c
 *
 *  Created on: Dec 1, 2020
 *      Author: BAHAA
 */

#include "timer0.h"

static volatile void (*g_callBackPtr)(void) = NULL_PTR;

ISR(TIMER0_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

void timer0_init_normal_mode(const Timer0_ConfigType * Timer0_ConfigPtr)
{
	TCNT0 = Timer0_ConfigPtr->initialValue; //Set Timer initial value to 0
	OCR0 = Timer0_ConfigPtr->compareValue;
	TIMSK |= (1<<TOIE0); // Enable Timer0 Overflow Interrupt
	/* Configure the timer control register
	 * 1. Non PWM mode FOC0=1
	 * 2. Normal Mode WGM01=0 & WGM00=0
	 * 3. Normal Mode COM00=0 & COM01=0
	 * 4. clock = F_CPU/1024 CS00=1 CS01=0 CS02=1
	 */
	//TCCR0 = (1<<FOC0) | (1<<CS02) | (1<<CS00);
	TCCR0 = (TCCR0 & 0b00000111) | ((Timer0_ConfigPtr->mode)<<3);
	TCCR0 = (TCCR0 & 0b11111000) | (Timer0_ConfigPtr->prescale);
	SREG  |= (1<<7);
}

void timer0_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr = a_ptr;
}
