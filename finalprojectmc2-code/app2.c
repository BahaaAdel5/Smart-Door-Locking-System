/*
 * app2.c
 *
 *  Created on: Nov 26, 2020
 *      Author: BAHAA
 */

/* ------------------------------INCLUDES-------------------------------------*/
#include "uart.h"
#include "external_eeprom.h"
#include "timer0.h"

/*-------------------------------VARIABLES------------------------------------*/
uint8 key1; /* 1st pressed key */
uint8 key2; /* 2nd pressed key */
uint8 key3; /* 3rd pressed key */
uint8 key4; /* 4th pressed key */
uint8 key5; /* 5th pressed key */
uint8 key11; /*variable to check for 1st pressed key*/
uint8 key22; /*variable to check for 2nd pressed key*/
uint8 key33; /*variable to check for 3rd pressed key*/
uint8 key44; /*variable to check for 4th pressed key*/
uint8 key55; /*variable to check for 5th pressed key*/
uint8 motorStatus = 0x00; /*variable to hold the byte received from MC2 to know in which direction the motor will rotate*/
uint8 motorCount = 0;
uint8 receivedByte; /*byte to receive what is sent from MC1*/
uint16 g_tick = 0; /*timer0 ticks to check how many times the timer overflowed*/
uint8 buzzerFlag = 0; /*flag to start buzzer when it is = 1 after the timer counted 1 minute*/

Timer0_ConfigType Timer0_Config = {0,0,NORMAL,F_CPU_1024};
UART_ConfigType UART_config = {9600,DISABLED,ONE_BIT,THREE,ZEROO};

/*------------------------------------------------------------------------------
[Function Name]: timer_fn
[Description]: counts 60 seconds after callback from timer0 driver
------------------------------------------------------------------------------*/
void timer_fn(){
	g_tick++; /*increment ticks counter every timer overflow*/

	if(g_tick > 31*60){ /*60 seconds buzzer*/
		buzzerFlag = 1; /*set buzzer flag = 1 after 60 seconds*/
		g_tick = 0; //clear the tick counter again to count a new 60 seconds when needed
	}

}

/*------------------------------------------------------------------------------
[Function Name]: storePassFirst
[Description]: store password entered on keypad @ MC1 here on the EEPROM
------------------------------------------------------------------------------*/
void storePassFirst(){
	key1 = UART_recieveByte(); /*receive byte from MC1 and store it in key1 variable*/
	EEPROM_writeByte(0x0311,key1); /*write key1 byte in location 0x0311 in EEPROM */

	key2 = UART_recieveByte();
	EEPROM_writeByte(0x0312,key2); /*write key2 byte in location 0x0312 in EEPROM */

	key3 = UART_recieveByte();
	EEPROM_writeByte(0x0313,key3); /*write key3 byte in location 0x0313 in EEPROM */

	key4 = UART_recieveByte();
	EEPROM_writeByte(0x0314,key4); /*write key4 byte in location 0x0314 in EEPROM */

	key5 = UART_recieveByte();
	EEPROM_writeByte(0x0315,key5); /*write key5 byte in location 0x0315 in EEPROM */
}

/*------------------------------------------------------------------------------
[Function Name]: storePassCheck
[Description]: Check the entered password with the one stored @ EEPROM and send confirmation to MC1
------------------------------------------------------------------------------*/
void storePassCheck(){
	key1 = UART_recieveByte(); /*receive byte from MC1 and store it in key1 variable*/
	EEPROM_readByte(0x0311, &key11); /*read byte from EEPROM location 0x0311 and store it in key11 variable*/

	key2 = UART_recieveByte();
	EEPROM_readByte(0x0312, &key22); /*read byte from EEPROM location 0x0312 and store it in key22 variable*/

	key3 = UART_recieveByte();
	EEPROM_readByte(0x0313, &key33); /*read byte from EEPROM location 0x0313 and store it in key33 variable*/

	key4 = UART_recieveByte();
	EEPROM_readByte(0x0314, &key44); /*read byte from EEPROM location 0x0314 and store it in key44 variable*/

	key5 = UART_recieveByte();
	EEPROM_readByte(0x0315, &key55); /*read byte from EEPROM location 0x0315 and store it in key55 variable*/

	while(UART_recieveByte()!= 13); /*loop until enter key is pressed @ MC1 keypad*/
	/*check if password entered is matching with the stored on in EEPROM*/
	if(key1 != key11 || key2 != key22 || key3 != key33 || key4 != key44 || key5 != key55){
		UART_sendByte(0x11); /*send byte to MC1 to confirm unmatched password*/
	}else{
		UART_sendByte(0x05); /*send byte to MC1 to confirm matched password*/
	}


}

/*------------------------------------------------------------------------------
[Function Name]: rotateMotor
[Description]: rotate motor CW, CCW and stop
------------------------------------------------------------------------------*/
void rotateMotor(){
	while(motorStatus != 0x04){ /*loop until motor status equals finish operation*/
		motorStatus = UART_recieveByte();
		//motorCount++;
		if(motorStatus == 0x01){ /*check if motorstatus is rotate CW*/
			PORTD &= (~(1<<PD7));
			PORTD |= (1<<PD6);
			//_delay_ms(4000);
		}else if(motorStatus == 0x02){ /*check if motorstatus is rotate CCW*/
			PORTD &= (~(1<<PD6));
			PORTD |= (1<<PD7);
			//_delay_ms(4000);
		}else if(motorStatus == 0x03){ /*check if motorstatus is stop*/
			PORTD &= (~(1<<PD6));
			PORTD &= (~(1<<PD7));
			//_delay_ms(3000);
		}
	}
	motorStatus = 0x00; /*clear motorstatus for the next time*/
	PORTD &= (~(1<<PD6));
	PORTD &= (~(1<<PD7));
}

/*------------------------------------------------------------------------------
[Function Name]: buzzerStart
[Description]: start buzzer after the user entered the password 3 times wrong
------------------------------------------------------------------------------*/
void buzzerStart(){
	PORTC |= (1<<PC7);
	timer0_init_normal_mode(&Timer0_Config); /*initialize timer0*/
	while(buzzerFlag != 1); /*hold the buzzer until 60 seconds has passed*/
	buzzerFlag = 0; /*clear flag for the next time*/
	PORTC &= ~(1<<PC7);
}

int main(){
	DDRD |= (1<<PD6) | (1<<PD7); /*pin 6,7 PORTD output pins*/
	DDRC |= (1<<PC7); /*pin 7 PORTC output pin*/
	timer0_setCallBack(timer_fn); /*set call back function for timer0*/

	UART_init(&UART_config); /*initialize UART*/
	EEPROM_init(); /*initialize EEPROM*/

	while(1){ /*infinite loop*/
		/* 0x21 is for change password
		 * 0x20 is for check password
		 * 0x22 is for rotate motor
		 * 0x55 is for buzzerStart */
		receivedByte = UART_recieveByte();
		if(receivedByte == 0x21){
			storePassFirst();
			while(UART_recieveByte()!= 13);
			storePassCheck();
		}
		if(receivedByte == 0x20){
			storePassCheck();
		}
		if(receivedByte == 0x22){
			rotateMotor();
		}
		if(receivedByte == 0x55){
			buzzerStart();
		}
	}
}
