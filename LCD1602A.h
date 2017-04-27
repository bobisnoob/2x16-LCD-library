#ifndef __LCD1602A_H
#define __LCD1602A_H

#ifdef __cplusplus
 extern "C" {
#endif 



#include "stm32f3xx_hal.h"
#include <stdint.h>
#define E_SET() LCD_WriteByteI2CLCD(portLCD|=0x04)
#define E_RESET() LCD_WriteByteI2CLCD(portLCD&=~0x04)
#define RS_SET() LCD_WriteByteI2CLCD(portLCD|=0x01)
#define RS_RESET() LCD_WriteByteI2CLCD(portLCD&=~0x01)
#define SET_LED() LCD_WriteByteI2CLCD(portLCD|=0x08)
#define RESET_LED() LCD_WriteByteI2CLCD(portLCD&=~0x08)
#define SET_WRITE() LCD_WriteByteI2CLCD(portLCD&=~0x02)

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
