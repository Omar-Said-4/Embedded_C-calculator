/***********************************************************/
/***********************************************************/
/******** Author  	 : Omar Said           *****************/
/******** File name  : KEYPAD_PROG.h       *****************/
/******** Date	     : 2/10/2022           *****************/
/******** Version  	 : V1                  *****************/
/******** Desc  	 : ..                  *****************/
/***********************************************************/
/***********************************************************/
#include"STD_TYPES.h"
#include"KEYPAD_CFG.h"
#include"DIO_INTERFACE.h"
#include<avr/delay.h>
void KEYPAD_VidInit()
{
	/*Setting whole Port to Output with Output value HIGH*/
	DIO_VidSetPortDirection(KEYPAD_PORT,0b11110000);
	DIO_VidSetPortValue(KEYPAD_PORT,255);
}

u8 KEYPAD_U8GetPressedKey()
{
	/*Keypad algorithm to get the pressed key as an enumarated value*/
	u8 key=50;
	for(int col=0;col<KEYPAD_COLN;col++)
	{
		DIO_VidSetPinValue(KEYPAD_PORT,col+4,0);
		for(int row=0;row<KEYPAD_ROWN;row++)
		{
			if(DIO_U8GetPinValue(KEYPAD_PORT,row)==0)
			{
				_delay_ms(2);
				key=(row*KEYPAD_COLN)+col;
				while(!DIO_U8GetPinValue(KEYPAD_PORT,row));
			}
		}
		DIO_VidSetPinValue(KEYPAD_PORT,col+4,1);
	}
	return key;
}
