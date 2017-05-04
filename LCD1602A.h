#ifndef __LCD1602A_H
#define __LCD1602A_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stm32f3xx_hal.h"
#include <stdint.h>
  
extern I2C_HandleTypeDef hi2c2;   
  
 /* Commands */
#define LCD1602A_CLEAR_DISPLAY			0x01
#define LCD1602A_RETURN_HOME	 		0x02
#define LCD1602A_ENTRY_MODE_SET 		0x04
#define LCD1602A_DISPLAY_CONTROL 		0x08
#define LCD1602A_CURSOR_DISPLAY_SHIFT		0x10
#define LCD1602A_FUCNTION_SET			0x20
#define LCD1602A_SET_CGRAM_ADDRESS		0x40		
#define LCD1602A_SET_DDRAM_ADDRESS		0x80

/* Entry Mode Set register bits */
/* NOTE : Set the moving direction of cursor and display */

#define LCD1602A_CURSOR_RIGHT			0x02  // Cursor/blink moves to right and DDRAM address is increased by 1
#define LCD1602A_CURSOR_LEFT			0x00  // Cursor/blink moves to left and DDRAM address is decreased by 1
#define LCD1602A_SHIFT_DISPLAY_LEFT		0x03  // shift the display to the left
#define LCD1602A_SHIFT_DISPLAY_RIGHT		0x01  // shift the display to the right
/* Display control register bits */

#define LCD1602A_DISPLAY_ON			0x04 // entire display on			
#define LCD1602A_DISPLAY_OFF			0x00 // entire display off
#define LCD1602A_CURSOR_ON			0x02 // cursor is turned on
#define LCD1602A_CURSOR_OFF			0x00 // cursor is turned off
#define LCD1602A_CURSOR_BLINK_ON		0x01 // cursor blink is on	
#define LCD1602A_CURSOR_BLINK_OFF		0x00 // cursor blink is off

/* Cursor or Display shift */
/* NOTE : Without writing or reading of display,shift right/left cursor position or display.
      	  This instruction is used to correct or search display data.
       	  When display shift is performed, the contents of address counter are not changed */

#define LCD1602A_SHIFT_CURSOR_LEFT 		0x00 
#define LCD1602A_SHIFT_CURSOR_RIGHT		0x04
#define LCD1602A_SHIFTORDISPLAY_LEFT		0x08
#define LCD1602A_SHIFTORDISPLAY_RIGHT		0x0F

/* Set Function register bits */	
	
#define LCD1602A_8BIT_MODE 			0x10
#define LCD1602A_4BIT_MODE 			0x00 // it needs to transfer 4-bit data by two times. MSB then LSB 
#define LCD1602A_1LINE_DISPLAY			0x00
#define LCD1602A_2LINE_DISPLAY			0x08
#define LCD1602A_5x8DOTS_FORMAT			0x00
#define LCD1602A_5x11DOTS_FORMAT		0x04 // ONLY FOR 1 LINE DISPLAY

/* Control macros */
#define E_SET() LCD_WriteByteI2CLCD(portLCD|=0x04)
#define E_RESET() LCD_WriteByteI2CLCD(portLCD&=~0x04)
#define RS_SET() LCD_WriteByteI2CLCD(portLCD|=0x01)
#define RS_RESET() LCD_WriteByteI2CLCD(portLCD&=~0x01)
#define SET_LED() LCD_WriteByteI2CLCD(portLCD|=0x08)
#define RESET_LED() LCD_WriteByteI2CLCD(portLCD&=~0x08)
#define SET_WRITE() LCD_WriteByteI2CLCD(portLCD&=~0x02)
  
/* LCD functions prototypes */
  
void LCD1602A_Command(uint8_t cmd);
void LCD1602A_Command_4Bit(uint8_t cmd);
void LCD1602A_Data(uint8_t data);
void LCD1602A_init(uint8_t numberOfLine);
void LCD1602A_DisplayOn(void);
void LCD1602A_DisplayOff(void);
void LCD1602A_BlinkOn(void);
void LCD1602A_BlinkOff(void);
void LCD1602A_CursorOff(void);
void LCD1602A_CursorOn(void);
void LCD1602A_RotateDisplayLeft(void);
void LCD1602A_RotateDisplayRight(void);
void LCD1602A_ShiftCursorLeft(void);
void LCD1602A_ShiftCursorRight(void);
void LCD1602A_ReturnHome (void);
void LCD1602A_WriteLineOne(uint8_t col, char *str);
void LCD1602A_WriteLineTwo(uint8_t col, char *str);
void clearLCD(void);
#endif
