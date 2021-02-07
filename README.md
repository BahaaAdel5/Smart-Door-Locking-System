# Smart-Door-Locking-System

 The Project Objective is to implement a smart door locking system. 
 
 Project Scenarios:
 - At startup, the user is asked to enter a new passowrd twice. This password is then sent to MC2 to be saves in EEPROM using I2C driver.
 - If the user entered a wrong password, he is asked to enter it again.
 - If passwrod is entered correctly, the user is asked to choose, whether to change password or to open door.
 - To open the door, the user is aked to enter password, if it matches the saved one, then the motor starts rotating CW for 15 seconds followed by a stop for 3 seconds then a 15 seconds CCW movement.
 - While motor rotates, the LCD module displays the door status (locking or unlocking) and the seconds spent.
 - If change password option is chosen by the user, then he is asked first to enter the old password. If correct, then the user is allowed to change password.
 - At any time if the password entered is wrong 3 times in a row, the program stops and the buzzer starts.
 
 
 #### ATmega16 Drivers:
 - Keypad 4x4
 - LCD 2x16
 - Timer0
 - UART
 - I2C
 - EEPROM
