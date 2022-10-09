/***********************************************************/
/***********************************************************/
/******** Author  	 : Omar Said           *****************/
/******** File name  : KEYPAD_CFG.h        *****************/
/******** Date	     : ---------           *****************/
/******** Version  	 : V1                  *****************/
/******** Desc  	 : ..                  *****************/
/***********************************************************/
/***********************************************************/

#include"STD_TYPES.h"
#include"BIT_MATH.h"
#include"DIO_INTERFACE.h"
#include"LCD_CONFIG.h"
#include"LCD_INTERFACE.h"
#include<avr/delay.h>
//static bool initialise_once=false;
void LCD_VidPrintFloat(f32 num)
{
	if(num<0)
	{LCD_VidSendData('-');
	num=-num;}
	LCD_VidSendNumber(num);
	if(num>0)
		num-=(u32)num;
	if(num==0)
		return;
	while((u32)num==0||(u32)num%10!=0)
		num*=10;
	num/=10;
	LCD_VidSendData('.');
	LCD_VidSendNumber(num);
}
void LCD_VidSendNumber(s32 num)
{
	u8 x=0;
	s32 rev =0;
	bool neg=false;
	if(num<0)
	{neg=true;
	num=-num;}
	if(num==0)
	{
		LCD_VidSendData(48);
		return;
	}
	if(neg)
		LCD_VidSendData('-');
	while(num)
	{
		rev=(rev*10)+(num%10);
		num/=10;
		x++;
	}

	while(x)
	{
		u8 p=rev%10+48;
		LCD_VidSendData(p);
		rev/=10;
		x--;
	}
}
void LCD_VidEdge()
{
	DIO_VidSetPinValue(LCD_EN,1);
	_delay_us(100);
	DIO_VidSetPinValue(LCD_EN,0);
	_delay_us(100);
}
void LCD_VidInitialise()
{
	DIO_VidSetPinDirection(LCD_RS,1);
	//DIO_VidSetPinDirection(CONTROL_PORT,RW,1);
	DIO_VidSetPinDirection(LCD_EN,1);
	_delay_ms(40);
#if (LCD_DNUM==8)
	DIO_VidSetPortDirection(LCD_PORT,255);
	LCD_VidSendCommand(0x38);
	LCD_VidSendCommand(0x0F);
#elif (LCD_DNUM==4)
	DIO_VidSetPinDirection(LCD_PORT,LCD_D4,1);
	DIO_VidSetPinDirection(LCD_PORT,LCD_D5,1);
	DIO_VidSetPinDirection(LCD_PORT,LCD_D6,1);
	DIO_VidSetPinDirection(LCD_PORT,LCD_D7,1);
	LCD_VidSendCommand(0x02);
	LCD_VidSendCommand(0x28);
	//LCD_VidSendCommand(0x80);
	_delay_us(500);
	LCD_VidSendCommand(0x0C);
	//LCD_VidSendCommand(0xF0);
	_delay_us(500);
	//LCD_VidSendCommand(0x00);
	//initialise_once=true;
#endif
	LCD_VidClear();

}
void LCD_VidSendData(u8 val)
{
	/*RS=1*/
	DIO_VidSetPinValue(LCD_RS,1);
	/*RW=0 -> Write operation* or connect to ground/
	DIO_VidSetPinValue(CONTROL_PORT,RW,0);*/
#if (LCD_DNUM==8)
	/*Send Character Ascii Code*/
	DIO_VidSetPortValue(LCD_PORT,val);
	/* Enable Sequence */
	LCD_VidEdge();
#elif (LCD_DNUM==4)
	DIO_VidSetPinValue(LCD_PORT,LCD_D7,GET_BIT(val,7));
	DIO_VidSetPinValue(LCD_PORT,LCD_D6,GET_BIT(val,6));
	DIO_VidSetPinValue(LCD_PORT,LCD_D5,GET_BIT(val,5));
	DIO_VidSetPinValue(LCD_PORT,LCD_D4,GET_BIT(val,4));
	LCD_VidEdge();
	DIO_VidSetPinValue(LCD_PORT,LCD_D7,GET_BIT(val,3));
	DIO_VidSetPinValue(LCD_PORT,LCD_D6,GET_BIT(val,2));
	DIO_VidSetPinValue(LCD_PORT,LCD_D5,GET_BIT(val,1));
	DIO_VidSetPinValue(LCD_PORT,LCD_D4,GET_BIT(val,0));
	LCD_VidEdge();
#endif

}
void LCD_VidSendCommand(u8 cmd)
{
	/*RS=0*/
	DIO_VidSetPinValue(LCD_RS,0);
	/*RW=0 -> Write operation* or connect to ground/
	DIO_VidSetPinValue(CONTROL_PORT,RW,0);*/
#if (LCD_DNUM==8)
	/*Send Character Ascii Code*/
	DIO_VidSetPortValue(LCD_PORT,cmd);
	/* Enable Sequence */
	LCD_VidEdge();
#elif (LCD_DNUM==4)
	DIO_VidSetPinValue(LCD_PORT,LCD_D7,GET_BIT(cmd,7));
	DIO_VidSetPinValue(LCD_PORT,LCD_D6,GET_BIT(cmd,6));
	DIO_VidSetPinValue(LCD_PORT,LCD_D5,GET_BIT(cmd,5));
	DIO_VidSetPinValue(LCD_PORT,LCD_D4,GET_BIT(cmd,4));
	LCD_VidEdge();
	//if(initialise_once){
	DIO_VidSetPinValue(LCD_PORT,LCD_D7,GET_BIT(cmd,3));
	DIO_VidSetPinValue(LCD_PORT,LCD_D6,GET_BIT(cmd,2));
	DIO_VidSetPinValue(LCD_PORT,LCD_D5,GET_BIT(cmd,1));
	DIO_VidSetPinValue(LCD_PORT,LCD_D4,GET_BIT(cmd,0));
	LCD_VidEdge();
	//	}
#endif
}
void LCD_VidWrite(string entry)
{
	for (int i = 0; entry[i] != '\0'; ++i)
	{
		LCD_VidSendData(entry[i]);
	}

}
void LCD_VidClear(void)
{
	LCD_VidSendCommand(LCD_CLEAR);
	_delay_ms(2);
}
void LCD_VidCursorLeft()
{
	LCD_VidSendCommand(LCD_CURSOR_LEFT);
}
void LCD_VidCursorRight()
{
	LCD_VidSendCommand(LCD_CURSOR_RIGHT);
}

void LCD_VidShiftLeft()
{
	LCD_VidSendCommand(SHIFT_LEFT);
}
void LCD_VidShiftRight()
{
	LCD_VidSendCommand(SHIFT_RIGHT);
}
void LCD_VidSetCursorPosition(u8 row, u8 coloumn)
{
	u8 address=0x40;
	if(row<2&&row>=0&&coloumn<16&&coloumn>=0)
	{
		address*=row;
		address+=coloumn;
		SET_BIT(address,7);
		LCD_VidSendCommand(address);
	}

}
void LCD_VidCreateChar(u8*pattern, u8 cgram_index)
{
	u8 address;
	if(cgram_index<8)
	{
		address=(cgram_index*8);
		CLR_BIT(address,7);
		SET_BIT(address,6);
		LCD_VidSendCommand(address);
		for(int i=0;i<8;i++)
			LCD_VidSendData(pattern[i]);

	}
	//LCD_VidReturnHome();
}
void LCD_VidCgRamInit(u8 i)
{
	u8 patt[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	LCD_VidCreateChar(patt,i);

}
void LCD_DisplayCreatedChar(u8 cgram_index,u8 row,u8 coloumn)
{
	LCD_VidSetCursorPosition(row,coloumn);
	LCD_VidSendData(cgram_index);
}
void LCD_VidReturnHome()
{
	LCD_VidSendCommand(RETURN_HOME);
	_delay_ms(2);
}
