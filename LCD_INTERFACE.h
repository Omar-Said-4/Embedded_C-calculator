/***********************************************************/
/***********************************************************/
/******** Author  	 : Omar Said           *****************/
/******** File name  : LCD_INTERFACE.h     *****************/
/******** Date	     : ---------           *****************/
/******** Version  	 : V1                  *****************/
/******** Desc  	 : --             **********************/
/***********************************************************/
/***********************************************************/

#ifndef LCD_INTERFACE_H
#define LCD_INTERFACE_H
#include"STD_TYPES.h"
#include"BIT_MATH.h"

/*To send string to the LCD*/
void LCD_VidWrite(string entry);

/*To initialize the LCD*/
void LCD_VidInitialise();

/*To send a command to the LCD*/
void LCD_VidSendCommand(u8 cmd);

/*To clear the display*/
void LCD_VidClear(void);

/*To shift the display cursor to the right*/
void LCD_VidCursorRight();

/*To shift the display cursor to the right*/
void LCD_VidCursorLeft();

/*To shift the display to the right*/
void LCD_VidShiftRight();

/*To clear a specific location in the cgram*/
void LCD_VidCgRamInit(u8 i);

/*To shift the display cursor to the right*/
void LCD_VidShiftLeft();

/*To execute the return home  command*/
void LCD_VidReturnHome();

/*To send an integer to the LCD*/
void LCD_VidSendNumber(s32 num);

/*To send a float to the LCD*/
void LCD_VidPrintFloat(f32 num);

/*To set the cursor to a specific position*/
void LCD_VidSetCursorPosition(u8 row, u8 coloumn);

/*To display a created char in the cgram*/
void LCD_DisplayCreatedChar(u8 cgram_index,u8 row,u8 coloumn);

/*To create a char in the cgram*/
void LCD_VidCreateChar(u8*pattern, u8 cgram_index);
#endif
