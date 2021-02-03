/*
 * app1.c
 *
 *  Created on: Nov 25, 2020
 *      Author: BAHAA
 */

/* ------------------------------INCLUDES-------------------------------------*/
#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "timer0.h"

/*-------------------------------VARIABLES------------------------------------*/
uint8 key1; /* 1st pressed key */
uint8 key2; /* 2nd pressed key */
uint8 key3; /* 3rd pressed key */
uint8 key4; /* 4th pressed key */
uint8 key5; /* 5th pressed key */
uint8 key6; /* 6th pressed key (ENTER) */
uint8 passCount = 0; /* Flag to check whether password is 1st time to enter or 2nd time (re-enter)*/
uint8 keyChoice; /* to store user choice '-' or '+' */
uint8 matchCheck; /* receive byte from MC2 to get if password is matching or not*/
uint8 trial = 0; /* Flag to check if it is 1st time to enter code or not*/
uint8 error = 0; /* to store how many times the user entered wrong password*/

uint16 g_tick = 0; /*timer0 ticks to check how many times the timer overflowed*/
uint8 fifteenSecondsFlag = 0; /* Flag to run motor*/
uint8 threeSecondsFlag = 0; /* Flag to stop motor*/
uint8 fifteen = 0; /*Flag to check at beginning of ISR whether it will count 15 or 3 seconds*/
uint8 three = 0; /*Flag to check at beginning of ISR whether it will count 15 or 3 seconds*/

Timer0_ConfigType Timer0_Config = {0,0,NORMAL,F_CPU_1024};  /*Timer0 Structure*/
UART_ConfigType UART_config = {9600,DISABLED,ONE_BIT,THREE,ZEROO}; /*UART Structure*/

/*------------------------------------------------------------------------------
[Function Name]: timer_fn
[Description]: counts 15 or 3 seconds after callback from timer0 driver
------------------------------------------------------------------------------*/
void timer_fn(){
	g_tick++; /*increment ticks counter every timer overflow*/

	if(fifteen == 1){ /*check if it is required to count 15 seconds*/
		if(g_tick > 31*15){ /*15 seconds = 465 ticks*/
			fifteenSecondsFlag = 1; /*run motor for 15 seconds*/
			g_tick = 0; /*clear the tick counter again to count a new 15 seconds when needed*/
		}
	}else if(three ==1){ /*check if it is required to count 3 seconds*/
		if(g_tick > 31*3){ /*3 seconds = 93 ticks*/
			threeSecondsFlag = 1; /*stop motor for 3 seconds*/
			g_tick = 0; /*clear the tick counter again to count a new 3 seconds when needed*/
		}
	}
}

/*------------------------------------------------------------------------------
[Function Name]: enterPassFirst
[Description]: Get entered password from user and send it to MC2 to be stored in EEPROM
------------------------------------------------------------------------------*/
void enterPassFirst(){
	if(passCount == 0){ /*check if it is the 1st time to enter password*/
		LCD_clearScreen(); /*clear LCD screen*/
		LCD_displayStringRowColumn(0, 0,"Please enter new"); /*display string @row 0 col 0*/
		LCD_displayStringRowColumn(1, 0,"pass:");  /*display string @row 1 col 0*/
	}

	key1 = KeyPad_getPressedKey(); /*get the 1st pressed key from keypad*/
	//LCD_intgerToString(key1);
	LCD_displayCharacter('*'); /*display '*'*/
	UART_sendByte(key1); /*send entered key from keypad to MC2*/
	_delay_ms(2000); /*delay 2 seconds to get only one number from one press from user*/

	key2 = KeyPad_getPressedKey(); /*get the 2nd pressed key from keypad*/
	//LCD_intgerToString(key2);
	LCD_displayCharacter('*');
	UART_sendByte(key2);
	_delay_ms(2000);

	key3 = KeyPad_getPressedKey(); /*get the 3rd pressed key from keypad*/
	//LCD_intgerToString(key3);
	LCD_displayCharacter('*');
	UART_sendByte(key3);
	_delay_ms(2000);

	key4 = KeyPad_getPressedKey(); /*get the 4th pressed key from keypad*/
	//LCD_intgerToString(key4);
	LCD_displayCharacter('*');
	UART_sendByte(key4);
	_delay_ms(2000);

	key5 = KeyPad_getPressedKey(); /*get the 5th pressed key from keypad*/
	//LCD_intgerToString(key5);
	LCD_displayCharacter('*');
	UART_sendByte(key5);
	_delay_ms(2000);

	key6 = KeyPad_getPressedKey(); /*get the 6th pressed key from keypad*/
	_delay_ms(2000);
	if(key6 == 13 && passCount == 0){ /*check if enter is pressed and it is the 1st time to enter new password*/
		passCount++; /*increment counter by 1*/
		LCD_clearScreen(); /*clear LCD screen*/
		LCD_displayStringRowColumn(0, 0,"Please re-enter"); /*display string @row 0 col 0*/
		LCD_displayStringRowColumn(1, 0,"new pass:"); /*display string @row 1 col 0*/
		UART_sendByte(key6); /*send enter key to MC2*/
		enterPassFirst(); /*enter function again to re-enter password*/
	}else if(key6 == 13 && passCount == 1){ /*check if enter is pressed and it is the 2nd time to enter new password*/
		UART_sendByte(key6); /*send enter key to MC2*/
	}
}

/*------------------------------------------------------------------------------
[Function Name]: enterPass
[Description]: Get entered password from user and send it to MC2 to be checked with the stored one in EEPROM
------------------------------------------------------------------------------*/
void enterPass(){
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0,"Please enter"); /*display string @row 0 col 0*/
	LCD_displayStringRowColumn(1, 0,"pass:"); /*display string @row 1 col 0*/

	key1 = KeyPad_getPressedKey(); /*get the 1st pressed key from keypad*/
	//LCD_intgerToString(key1);
	LCD_displayCharacter('*'); /*display '*'*/
	UART_sendByte(key1); /*send entered key from keypad to MC2*/
	_delay_ms(2000); /*delay 2 seconds to get only one number from one press from user*/

	key2 = KeyPad_getPressedKey();
	//LCD_intgerToString(key2);
	LCD_displayCharacter('*');
	UART_sendByte(key2);
	_delay_ms(2000);

	key3 = KeyPad_getPressedKey();
	//LCD_intgerToString(key3);
	LCD_displayCharacter('*');
	UART_sendByte(key3);
	_delay_ms(2000);

	key4 = KeyPad_getPressedKey();
	//LCD_intgerToString(key4);
	LCD_displayCharacter('*');
	UART_sendByte(key4);
	_delay_ms(2000);

	key5 = KeyPad_getPressedKey();
	//LCD_intgerToString(key5);
	LCD_displayCharacter('*');
	UART_sendByte(key5);
	_delay_ms(2000);

	key6 = KeyPad_getPressedKey();
	_delay_ms(2000);
	UART_sendByte(key6);
}

/*------------------------------------------------------------------------------
[Function Name]: rotateMotor
[Description]: This function is responsible to rotate motor CW and CCW and to stop it
------------------------------------------------------------------------------*/
void rotateMotor(){
	UART_sendByte(0x01); /*send byte to MC2 to rotate motor CW*/
	LCD_clearScreen(); /*clear LCD screen*/
	LCD_displayString("Motor Rotating"); /*display string at 1st empty place in LCD*/
	LCD_displayStringRowColumn(1, 4, "CW"); /*display string @ row 1 col 4*/
	fifteen = 1; /*turn flag = 1 to start counting 15 seconds in ISR*/
	g_tick = 0; /*clear ticks counter*/
	timer0_init_normal_mode(&Timer0_Config); /*initialize timer0*/
	while(fifteenSecondsFlag != 1){ /*loop until flag = 1*/
		LCD_goToRowColumn(1, 8); /*set LCD cursor @ row1 col 8*/
		LCD_intgerToString(g_tick/31);	/*display seconds on LCD*/
	}
	fifteen = 0; /*clear flag*/
	fifteenSecondsFlag = 0; /*clear flag*/

	UART_sendByte(0x03); /*send byte to MC2 to stop the motor*/
	LCD_clearScreen(); /*clear screen*/
	LCD_displayString("Motor Stopped"); /*display string on LCD @ 1st empty place*/
	three = 1; /*turn flag = 1 to start counting 3 seconds in the ISR*/
	timer0_init_normal_mode(&Timer0_Config); /*initialize timer0 again to reset values*/
	while(threeSecondsFlag != 1){ /*loop until flag = 1*/
		LCD_goToRowColumn(1, 8); /*set LCD cursor @ row1 col 8*/
		LCD_intgerToString(g_tick/31); /*display seconds on LCD*/
	}
	three = 0; /*clear flag*/
	threeSecondsFlag = 0; /*clear flag*/

	UART_sendByte(0x02); /*send byte to MC2 to rotate the motor CCW*/
	LCD_clearScreen();
	LCD_displayString("Motor Rotating");
	LCD_displayStringRowColumn(1, 4, "CCW");
	fifteen = 1;
	g_tick = 0;
	timer0_init_normal_mode(&Timer0_Config);
	while(fifteenSecondsFlag != 1){
		LCD_goToRowColumn(1, 8);
		LCD_intgerToString(g_tick/31);
	}
	fifteen = 0;
	fifteenSecondsFlag = 0;

	UART_sendByte(0x04); /*send byte to MC2 to inform that the motor function has ended*/
	_delay_ms(2000); /*delay 2 seconds*/
}

/*------------------------------------------------------------------------------
[Function Name]: keyChoice_fn
[Description]: This function is responsible for getting the required action from user and execute it using the suitable functions
------------------------------------------------------------------------------*/
void keyChoice_fn(){
	LCD_clearScreen(); /*clear LCD screen*/
	LCD_displayStringRowColumn(0, 2, "+: Change Pass"); /*display string @ row 0 col 2*/
	LCD_displayStringRowColumn(1, 2, "-: Open Door"); /*display string @ row 1 col 2*/
	keyChoice = KeyPad_getPressedKey(); /*get pressed key from keypad*/
	_delay_ms(2000); /*delay 2 seconds*/

	/* '+': change pass
	 * '-': rotate motor*/
	if(keyChoice == '+'){ /*check if '+' is pressed on keypad to change password*/
		UART_sendByte(0x20); /*send byte to MC2 to ask user to enter password*/
		enterPass(); /*cal enterPass function to check for user's pass*/
		matchCheck = UART_recieveByte(); /*receive a check byte from MC2*/
		while(matchCheck != 0x05){ /*loop until password entered is correct*/
			error++; /*increment error counter every time entered password is wrong*/
			if(error == 3){ /*check if user entred password 3 times wrong*/
				LCD_clearScreen(); /*clear LCD screen*/
				LCD_displayStringRowColumn(0, 1, "Error ya haramy"); /*display error messege on LCD @ row 0 col 1*/
				UART_sendByte(0x55); /*send byte to MC2 to start buzzer*/
				while(1); /*hold the code*/
			}
			/*if it is not the 3rd time wrong pass just ask the user for a try again*/
			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 3, "Try again");
			LCD_displayStringRowColumn(1, 0, "Remaining: ");
			LCD_intgerToString(3-error);
			_delay_ms(5000);
			/*check password entered*/
			UART_sendByte(0x20);
			enterPass();
			matchCheck = UART_recieveByte();
		}
		error = 0; /*clear error count for further usage*/
		/*start changing password by user*/
		UART_sendByte(0x21);
		enterPassFirst();
		passCount=0;
		matchCheck = UART_recieveByte();
	}else if(keyChoice == '-'){ /*check if '-' is pressed on keypad to open door*/
		/*ask user to enter password*/
		UART_sendByte(0x20);
		enterPass();
		matchCheck = UART_recieveByte(); /*check for password*/
		while(matchCheck != 0x05){
			error++;
			if(error == 3){
				LCD_clearScreen();
				LCD_displayStringRowColumn(0, 1, "Error ya haramy");
				UART_sendByte(0x55); //buzzer
				while(1);
			}
			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 3, "Try again");
			LCD_displayStringRowColumn(1, 0, "Remaining: ");
			LCD_intgerToString(3-error);
			_delay_ms(5000);
			UART_sendByte(0x20);
			enterPass();
			matchCheck = UART_recieveByte();
		}
		error = 0;
		/*start rotating motor*/
		UART_sendByte(0x22); /*send byte to MC2 to rotate motor*/
		rotateMotor();
	}
}


int main(){
	timer0_setCallBack(timer_fn); /*timer0 set call back function*/
	UART_init(&UART_config); /*initialize UART driver*/
	LCD_init(); /*initialize LCD driver*/
	while(1){ /*infinite loop*/
		if(trial == 0){ /*check if it is the 1st time to enter new password*/
			UART_sendByte(0x21); /*send byte to MC2 to enter password*/
			enterPassFirst(); /*call function to enter password for the 1st time and to re-enter it*/
			passCount = 0; /*clear counter*/
			trial++; /*increment counter by 1*/
			matchCheck = UART_recieveByte(); /*check that the password and its re-entered one is matching*/
		}

		if(matchCheck == 0x11){ /*check for unmatched new password*/
			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 3,"Unmatched");
			LCD_displayStringRowColumn(1, 3, "Try Again");
			_delay_ms(5000);
			UART_sendByte(0x21); /*send byte to MC2 to create new password again*/
			enterPassFirst();
			passCount = 0;
			matchCheck = UART_recieveByte(); /*check that the password and its re-entered one is matching*/
		}else if(matchCheck == 0x05){ /*check for matched new password*/
			keyChoice_fn(); /*go ask user for which function he wants whether to open door or to change password*/
		}


	}
}
