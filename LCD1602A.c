/**************************************************************
* File Name	 	 : LCD1602A.c
* PROJECT		 : ST7006 2x16 LCD library
* PROGRAMMERS	 	 : Abdel Alkuor
* SECOND VERSION  	 : 2016-03-13
* Description    	 : This library can be used to drive any 16x2 LCD
			           it uses I2C1 protocol. The pins are connected as following for STM32F3 discovery board:
		        	   SCL--->PA15
			           SDA--->PA14
						
**************************************************************/	
#include "LCD1602A.h"

uint8_t buffer[1]={0};
uint8_t portLCD=0;

/* This fucntion is used to send a byte to the LCD whether data or command
* @param: bt: byte to be sent
*/
void LCD_WriteByteI2CLCD(uint8_t bt)
{
	buffer[0] = bt;
	HAL_I2C_Master_Transmit(&hi2c2,(uint16_t)0x4E, buffer,1,100); // 0x4E is the address of the I2C expansion IC you might need to
                                                                  // change it to the address of the I2C expansion IC you are using
                                                                  // consult the datasheet of the IC you are using
}

/* This fucntion is used to send half a byte (a command) to the LCD 
* @param: c: command to be sent
*/
void sendHalfByte(uint8_t c)
{
	c<<=4;
	E_SET();
	HAL_Delay(1);
	LCD_WriteByteI2CLCD(portLCD|c);
	E_RESET();
	HAL_Delay(1);
}

/* This fucntion is used to send full byte to the LCD whether data or command
* @param: c: byte to be sent
*/
void sendByte(uint8_t c, uint8_t mode)
{
	uint8_t Msbits = 0;
	(mode==0)? RS_RESET():RS_SET();
	Msbits = c>>4;
	sendHalfByte(Msbits);
	sendHalfByte(c);

}

/* This function can be used to clear the whole LCD display */
void clearLCD(void)
{
	sendByte(LCD1602A_CLEAR_DISPLAY, 0);
	HAL_Delay(50);
}

void LCD1602A_Data(uint8_t data)
{
	sendByte(data, 1);
}
/* This function should be called in the main() function to initialize the
* LCD display.
* @param: numberOfline: To display one line choose either 1 or 0
                        To display 2 lines choose number > 1
*/
void LCD1602A_init(uint8_t numberOfLine)
{
	HAL_Delay(15);
	
	/*1st try to set 4bit mode */
	sendHalfByte(0x03);
	HAL_Delay(1);
	
	/*2nd try to set 4bit mode */
	sendHalfByte(0x03);
	HAL_Delay(1);
	
	/*3rd try to set 4bit mode */
	sendHalfByte(0x03);
	HAL_Delay(1);
	sendHalfByte(0x02);
	
	if (numberOfLine>1)
	{
		sendByte( LCD1602A_FUCNTION_SET | LCD1602A_2LINE_DISPLAY | LCD1602A_5x8DOTS_FORMAT, 0);
	}
	else
	{
		sendByte( LCD1602A_FUCNTION_SET | LCD1602A_1LINE_DISPLAY | LCD1602A_5x8DOTS_FORMAT, 0);
	}
	
	sendByte( LCD1602A_DISPLAY_CONTROL | LCD1602A_DISPLAY_ON | LCD1602A_CURSOR_ON | LCD1602A_CURSOR_BLINK_ON, 0);
	sendByte( LCD1602A_CLEAR_DISPLAY,0 );
	sendByte( LCD1602A_ENTRY_MODE_SET | LCD1602A_CURSOR_RIGHT,0 );
	
	HAL_Delay(300);
	SET_LED();
	SET_WRITE();

}

/* this function is used to turn the display ON */
void LCD1602A_DisplayOn(void)
{
	sendByte( LCD1602A_DISPLAY_CONTROL | LCD1602A_DISPLAY_ON, 0);
	HAL_Delay(10);
}
/* this function is used to turn the display OFF */
void LCD1602A_DisplayOff(void)
{
	sendByte( LCD1602A_DISPLAY_CONTROL | LCD1602A_DISPLAY_OFF , 0);
	HAL_Delay(10);
}
/* this function is used to have blinking cursor ON */
void LCD1602A_BlinkOn(void)
{
	sendByte( LCD1602A_DISPLAY_CONTROL | LCD1602A_CURSOR_BLINK_ON, 0);
	HAL_Delay(10);
}

/* this function is used to have blinking cursor OFF */
void LCD1602A_BlinkOff(void)
{
	sendByte( LCD1602A_DISPLAY_CONTROL | LCD1602A_CURSOR_BLINK_OFF | LCD1602A_DISPLAY_ON, 0);
	HAL_Delay(10);
}

/* this function is used to turn the cursor OFF */
void LCD1602A_CursorOff(void)
{
	sendByte( LCD1602A_DISPLAY_CONTROL | LCD1602A_CURSOR_OFF | LCD1602A_DISPLAY_ON, 0);
	HAL_Delay(10);
}

/* This function is used to turn the cursor on */
void LCD1602A_CursorOn(void)
{
	sendByte( LCD1602A_DISPLAY_CONTROL | LCD1602A_CURSOR_ON, 0 );
	HAL_Delay(10);
}

/* This function is used to rotate the display left */
void LCD1602A_RotateDisplayLeft(void)
{
	sendByte( LCD1602A_SHIFTORDISPLAY_LEFT | LCD1602A_CURSOR_DISPLAY_SHIFT, 0 );
	HAL_Delay(10);
}

/* This function is used to rotate the display right*/
void LCD1602A_RotateDisplayRight(void)
{
	sendByte( LCD1602A_SHIFTORDISPLAY_RIGHT | LCD1602A_CURSOR_DISPLAY_SHIFT, 0 );
	HAL_Delay(10);
}

/* This function is used to shift the cursor left*/
void LCD1602A_ShiftCursorLeft(void)
{
	sendByte( LCD1602A_SHIFT_CURSOR_LEFT | LCD1602A_CURSOR_DISPLAY_SHIFT, 0 );
	HAL_Delay(10);
}
/* This function is used to shift the cursor right*/
void LCD1602A_ShiftCursorRight(void)
{
	sendByte( LCD1602A_SHIFT_CURSOR_RIGHT | LCD1602A_CURSOR_DISPLAY_SHIFT , 0);
	HAL_Delay(10);
}

/* This function is used to return the cursor at the beginning of the display*/
void LCD1602A_ReturnHome (void)
{
	sendByte( LCD1602A_RETURN_HOME, 0);
	HAL_Delay(10);
}

/* This function is used to write on the LCD display on the first row*/
/* @param: col: # between 0-15 to where to start writing
           str: The string you would like to display on the LCD 
*/
void LCD1602A_WriteLineOne(uint8_t col, char *str)
{	
	sendByte( LCD1602A_SET_DDRAM_ADDRESS | col | 0x00, 0);// 0x00 is the address of the first column in the first row of The LCD
	
	while(*str)
	{
		LCD1602A_Data( *str );
		str++;
	}
	
}

/* This function is used to write on the LCD display on the second row*/
/* @param: col: # between 0-15 to where to start writing
           str: The string you would like to display on the LCD 
*/
void LCD1602A_WriteLineTwo(uint8_t col, char *str)
{
	sendByte( LCD1602A_SET_DDRAM_ADDRESS | col | 0x40, 0 );// 0x40 is the address of the first column in the second row of the LCD
	while(*str)
	{
		LCD1602A_Data( *str );
		str++;	
	}
	
}




