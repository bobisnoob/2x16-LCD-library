/*
* File Name	 	 : LCD1602A.c
* PROJECT		 : ST7006 2x16 LCD library
* PROGRAMMERS	 	 : Abdel Alkuor
* SECOND VERSION  	 : 2016-03-13
* Description    	 : This library can be used to drive any 16x2 LCD
			           it uses I2C protocol. The pins are connected as follows:
		        	   SCL--->PA15
			           SDA--->PA14

 						
**/	

#include "LCD1602A.h"


/* Commands */

#define LCD1602A_CLEAR_DISPLAY				0x01
#define LCD1602A_RETURN_HOME	 			0x02
#define LCD1602A_ENTRY_MODE_SET 			0x04
#define LCD1602A_DISPLAY_CONTROL 			0x08
#define LCD1602A_CURSOR_DISPLAY_SHIFT		0x10
#define LCD1602A_FUCNTION_SET				0x20
#define LCD1602A_SET_CGRAM_ADDRESS			0x40		
#define LCD1602A_SET_DDRAM_ADDRESS			0x80



/* Entry Mode Set register bits */
/* NOTE : Set the moving direction of cursor and display */

#define LCD1602A_CURSOR_RIGHT			0x02  // Cursor/blink moves to right and DDRAM address is increased by 1
#define LCD1602A_CURSOR_LEFT			0x00  // Cursor/blink moves to left and DDRAM address is decreased by 1
#define LCD1602A_SHIFT_DISPLAY_LEFT		0x03  // shift the display to the left
#define LCD1602A_SHIFT_DISPLAY_RIGHT	0x01  // shift the display to the right


/* Display control register bits */

#define LCD1602A_DISPLAY_ON				0x04 // entire display on			
#define LCD1602A_DISPLAY_OFF			0x00 // entire display off
#define LCD1602A_CURSOR_ON				0x02 // cursor is turned on
#define LCD1602A_CURSOR_OFF				0x00 // cursor is turned off
#define LCD1602A_CURSOR_BLINK_ON		0x01 // cursor blink is on	
#define LCD1602A_CURSOR_BLINK_OFF		0x00 // cursor blink is off

/* Cursor or Display shift */
/* NOTE : Without writing or reading of display,shift right/left cursor position or display.
      	  This instruction is used to correct or search display data.
       	  When display shift is performed, the contents of address counter are not changed */

#define LCD1602A_SHIFT_CURSOR_LEFT 		0x00 
#define LCD1602A_SHIFT_CURSOR_RIGHT		0x04
#define LCD1602A_SHIFTORDISPLAY_LEFT	0x08
#define LCD1602A_SHIFTORDISPLAY_RIGHT	0x0F

/* Set Function register bits */	
	
#define LCD1602A_8BIT_MODE 				0x10
#define LCD1602A_4BIT_MODE 				0x00 // it needs to transfer 4-bit data by two times. MSB then LSB 
#define LCD1602A_1LINE_DISPLAY			0x00
#define LCD1602A_2LINE_DISPLAY			0x08
#define LCD1602A_5x8DOTS_FORMAT			0x00
#define LCD1602A_5x11DOTS_FORMAT		0x04 // ONLY FOR 1 LINE DISPLAY

extern I2C_HandleTypeDef hi2c2;
uint8_t buffer[1]={0};
uint8_t portLCD=0;


void LCD_WriteByteI2CLCD(uint8_t bt)
{
	buffer[0] = bt;
	HAL_I2C_Master_Transmit(&hi2c2,(uint16_t)0x4E, buffer,1,100);
}


void sendHalfByte(uint8_t c)
{
	c<<=4;
	E_SET();
	HAL_Delay(1);
	LCD_WriteByteI2CLCD(portLCD|c);
	E_RESET();
	HAL_Delay(1);
}

void sendByte(uint8_t c, uint8_t mode)
{
	uint8_t Msbits = 0;
	(mode==0)? RS_RESET():RS_SET();
	Msbits = c>>4;
	sendHalfByte(Msbits);
	sendHalfByte(c);

}

void clearLCD(void)
{
	sendByte(LCD1602A_CLEAR_DISPLAY, 0);
	HAL_Delay(50);
}

void LCD1602A_Data(uint8_t data)
{
	sendByte(data, 1);
}





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
		sendByte( LCD1602A_FUCNTION_SET | LCD1602A_2LINE_DISPLAY | LCD1602A_5x8DOTS_FORMAT, 0 );
	}
	else
	{
		sendByte( LCD1602A_FUCNTION_SET | LCD1602A_1LINE_DISPLAY | LCD1602A_5x8DOTS_FORMAT, 0 );
	}
	
	sendByte( LCD1602A_DISPLAY_CONTROL | LCD1602A_DISPLAY_ON | LCD1602A_CURSOR_ON | LCD1602A_CURSOR_BLINK_ON, 0);
	sendByte( LCD1602A_CLEAR_DISPLAY,0 );
	sendByte( LCD1602A_ENTRY_MODE_SET | LCD1602A_CURSOR_RIGHT,0 );
	
	HAL_Delay(300);
	SET_LED();
	SET_WRITE();

}

void LCD1602A_DisplayOn(void)
{
	sendByte( LCD1602A_DISPLAY_CONTROL | LCD1602A_DISPLAY_ON, 0 );
	HAL_Delay(10);
}

void LCD1602A_DisplayOff(void)
{
	sendByte( LCD1602A_DISPLAY_CONTROL | LCD1602A_DISPLAY_OFF , 0);
	HAL_Delay(10);
}

void LCD1602A_BlinkOn(void)
{
	sendByte( LCD1602A_DISPLAY_CONTROL | LCD1602A_CURSOR_BLINK_ON, 0 );
	HAL_Delay(10);
}

void LCD1602A_BlinkOff(void)
{
	sendByte( LCD1602A_DISPLAY_CONTROL | LCD1602A_CURSOR_BLINK_OFF | LCD1602A_DISPLAY_ON, 0);
	HAL_Delay(10);
}

void LCD1602A_CursorOff(void)
{
	sendByte( LCD1602A_DISPLAY_CONTROL | LCD1602A_CURSOR_OFF | LCD1602A_DISPLAY_ON, 0);
	HAL_Delay(10);
}

void LCD1602A_CursorOn(void)
{
	sendByte( LCD1602A_DISPLAY_CONTROL | LCD1602A_CURSOR_ON, 0 );
	HAL_Delay(10);
}


void LCD1602A_RotateDisplayLeft(void)
{
	sendByte( LCD1602A_SHIFTORDISPLAY_LEFT | LCD1602A_CURSOR_DISPLAY_SHIFT, 0 );
	HAL_Delay(10);
}

void LCD1602A_RotateDisplayRight(void)
{
	sendByte( LCD1602A_SHIFTORDISPLAY_RIGHT | LCD1602A_CURSOR_DISPLAY_SHIFT, 0 );
	HAL_Delay(10);
}

void LCD1602A_ShiftCursorLeft(void)
{
	sendByte( LCD1602A_SHIFT_CURSOR_LEFT | LCD1602A_CURSOR_DISPLAY_SHIFT, 0 );
	HAL_Delay(10);
}

void LCD1602A_ShiftCursorRight(void)
{
	sendByte( LCD1602A_SHIFT_CURSOR_RIGHT | LCD1602A_CURSOR_DISPLAY_SHIFT , 0);
	HAL_Delay(10);
}


void LCD1602A_ReturnHome (void)
{
	sendByte( LCD1602A_RETURN_HOME, 0 );
	HAL_Delay(10);
}

void LCD1602A_WriteLineOne(uint8_t col, char *str)
{	
	sendByte( LCD1602A_SET_DDRAM_ADDRESS | col | 0x00, 0 );
	
	while(*str)
	{
		LCD1602A_Data( *str );
		str++;
	}
	
}

void LCD1602A_WriteLineTwo(uint8_t col, char *str)
{
	sendByte( LCD1602A_SET_DDRAM_ADDRESS | col  | 0x40, 0 );
	while(*str)
	{
		LCD1602A_Data( *str );
		str++;	
	}
	
}




