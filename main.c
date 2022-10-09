/*
 * main.c
 *
 *  Created on: Oct 2, 2022
 *      Author: saziz
 */
#include"STD_TYPES.h"
#include"BIT_MATH.h"
#include"KEYPAD_INTERFACE.h"
#include<avr/delay.h>
#include"LCD_INTERFACE.h"
#include"DFS.h"

/*current state of calculator*/
states curr_state=empty;

/*previous state of calculator*/
states prev_state=none;

/*to check for error*/
bool  err=false;

/*answer*/
f32 ans=0;

/*array of entered numbers*/
f32 nums[21]={0};

/*array of chosen operators*/
u8 chars[19]={0};

/*number of operands*/
u8 ni=0;

/*Number of  times the display was shifted*/
u8 shifts=0;

/*number of operators*/
u8 charsi=0;

/*Cursor position*/
u8 cursori=0;

/*array of negative numbers indices*/
bool negindices[19]={0};

/*number of operators*/
u8 nopsc=1;

/*invert the sign of numbers at negative indices*/
void negatives()
{
	for(int i=0;i<19;i++){
		if(negindices[i])
		{
			nums[i]=-nums[i];
		}
	}
}

/*remove a number from nums*/
void shiftnums(u8 ind)
{
	for(int i=ind;i<ni;i++)
	{
		nums[i]=nums[i+1];
	}
	ni--;
}

/*remove an operator from chars*/
void shiftcharss(u8 ind)
{
	for(int i=ind;i<charsi;i++)
	{
		chars[i]=chars[i+1];
	}
	charsi--;
}

/*Result calculating funcction*/
void calc()
{
	f32 tmpans=0;
	negatives();
	for(int i=0;i<charsi;i++)
	{
		if(chars[i]=='*')
		{
			tmpans=nums[i]*nums[i+1];
			nums[i]=tmpans;
			shiftnums(i+1);
			shiftcharss(i);
			i=-1;
		}
		else if(chars[i]=='/')
		{
			if(nums[i+1]==0)
			{err=true;
			return;}
			tmpans=nums[i]/nums[i+1];
			//if(tmpans<0)
			//{

				//tmpans=-tmpans;
				//tmpans/=nums[i+1];
				//tmpans=-tmpans;
			//}
			//else
			//{
				//tmpans/=nums[i+1];
			//}
			nums[i]=tmpans;
			shiftnums(i+1);
			shiftcharss(i);
			i=-1;
		}
	}
	for(int i=0;i<charsi;i++)
	{
		if(chars[i]=='+')
		{
			tmpans=nums[i]+nums[i+1];
			nums[i]=tmpans;

			shiftnums(i+1);
			shiftcharss(i);
			i=-1;
		}
		else if(chars[i]=='-')
		{
			tmpans=nums[i]-(nums[i+1]);
			nums[i]=tmpans;
			shiftnums(i+1);
			shiftcharss(i);
			i=-1;
		}
	}
	ans=nums[0];
}

/*checks if the display needed to be shifted*/
void checkShift()
{
	if(cursori>15)
	{
		LCD_VidShiftLeft();
		shifts++;
	}
}

/*clearing all the arrays*/
void cleararrs()
{
	for(int i=0;i<21;i++)
		nums[i]=0;
	for(int i=0;i<19;i++){
		chars[i]=0;
		negindices[i]=0;
	}
	ni=0;
	charsi=0;
}

/*implementation of the finite state machine*/
void getstate()
{
	if(curr_state!=none)
		prev_state=curr_state;
	u8 x=KEYPAD_U8GetPressedKey();
	switch(x)
	{
	case 0: curr_state= one; break;
	case 1: curr_state= two; break;
	case 2: curr_state= three; break;
	case 3: curr_state= plus; break;
	case 4: curr_state= four; break;
	case 5: curr_state= five; break;
	case 6: curr_state= six; break;
	case 7: curr_state= minus; break;
	case 8: curr_state= seven; break;
	case 9: curr_state= eight; break;
	case 10: curr_state= nine; break;
	case 11: curr_state= multiply; break;
	case 12: curr_state= clear; break;
	case 13: curr_state= zero; break;
	case 14: curr_state= equal; break;
	case 15: curr_state= divide; break;
	default: curr_state=none;break;
	}
}

/*checking if an error happened*/
void error()
{
	if(err){
		LCD_VidClear();
		LCD_VidWrite("Error");
		_delay_ms(1000);
		err=false;
		cursori=0;
		curr_state=clear;
	}
}

/*checking if too many operators are consecutive*/
void checkoverops()
{
	if(nopsc>2)
		err=true;
}

/*Processing user inputs*/
void proceed()
{
	if(curr_state==clear)
	{
		LCD_VidClear();
		LCD_VidReturnHome();
		curr_state=empty;
		nopsc=1;
		cursori=0;
		cleararrs();
		shifts=0;
	}
	else if(curr_state==one)
	{
		if(prev_state==none||prev_state==empty)
		{
			checkShift();
			LCD_VidSendNumber(1);
			cursori++;
			nums[ni]=nums[ni]*10+1;
		}
		else if(prev_state==equal)
		{
			LCD_VidClear();
			cleararrs();
			LCD_VidSendNumber(1);
			nums[ni]=nums[ni]*10+1;
			cursori=1;
		}
		else if(prev_state==one||prev_state==two||prev_state==three||prev_state==four||prev_state==five||prev_state==six||prev_state==seven||prev_state==eight||prev_state==nine||prev_state==0)
		{
			checkShift();
			LCD_VidSendNumber(1);
			nums[ni]=nums[ni]*10+1;
			cursori++;
		}
		else if(prev_state==multiply||prev_state==divide||prev_state==plus||prev_state==minus)
		{
			checkShift();
			ni++;
			LCD_VidSendNumber(1);
			nums[ni]=nums[ni]*10+1;
			cursori++;
		}
	}
	else if(curr_state==two)
	{
		if(prev_state==none||prev_state==empty)
		{
			checkShift();
			cursori++;
			LCD_VidSendNumber(2);
			nums[ni]=nums[ni]*10+2;
		}
		else if(prev_state==equal)
		{
			LCD_VidClear();
			cleararrs();
			cursori=1;
			LCD_VidSendNumber(2);
			nums[ni]=nums[ni]*10+2;
		}
		else if(prev_state==one||prev_state==two||prev_state==three||prev_state==four||prev_state==five||prev_state==six||prev_state==seven||prev_state==eight||prev_state==nine||prev_state==0)
		{
			checkShift();
			cursori++;
			LCD_VidSendNumber(2);
			nums[ni]=nums[ni]*10+2;
		}
		else if(prev_state==multiply||prev_state==divide||prev_state==plus||prev_state==minus)
		{
			checkShift();
			cursori++;
			ni++;
			LCD_VidSendNumber(2);
			nums[ni]=nums[ni]*10+2;
		}
	}
	else if(curr_state==three)
	{
		if(prev_state==none||prev_state==empty)
		{
			checkShift();
			cursori++;
			LCD_VidSendNumber(3);
			nums[ni]=nums[ni]*10+3;
		}
		else if(prev_state==equal)
		{
			cursori=1;
			LCD_VidClear();
			cleararrs();
			LCD_VidSendNumber(3);
			nums[ni]=nums[ni]*10+3;
		}
		else if(prev_state==one||prev_state==two||prev_state==three||prev_state==four||prev_state==five||prev_state==six||prev_state==seven||prev_state==eight||prev_state==nine||prev_state==0)
		{
			checkShift();
			cursori++;
			LCD_VidSendNumber(3);
			nums[ni]=nums[ni]*10+3;
		}
		else if(prev_state==multiply||prev_state==divide||prev_state==plus||prev_state==minus)
		{
			checkShift();
			ni++;
			cursori++;
			LCD_VidSendNumber(3);
			nums[ni]=nums[ni]*10+3;
		}
	}
	else if(curr_state==four)
	{
		if(prev_state==none||prev_state==empty)
		{
			checkShift();
			cursori++;
			LCD_VidSendNumber(4);
			nums[ni]=nums[ni]*10+4;
		}
		else if(prev_state==equal)
		{
			cursori=1;
			LCD_VidClear();
			cleararrs();
			LCD_VidSendNumber(4);
			nums[ni]=nums[ni]*10+4;
		}
		else if(prev_state==one||prev_state==two||prev_state==three||prev_state==four||prev_state==five||prev_state==six||prev_state==seven||prev_state==eight||prev_state==nine||prev_state==0)
		{
			checkShift();
			cursori++;
			LCD_VidSendNumber(4);
			nums[ni]=nums[ni]*10+4;
		}
		else if(prev_state==multiply||prev_state==divide||prev_state==plus||prev_state==minus)
		{
			checkShift();
			cursori++;
			ni++;
			LCD_VidSendNumber(4);
			nums[ni]=nums[ni]*10+4;
		}
	}
	else if(curr_state==five)
	{
		if(prev_state==none||prev_state==empty)
		{
			checkShift();
			cursori++;
			LCD_VidSendNumber(5);
			nums[ni]=nums[ni]*10+5;
		}
		else if(prev_state==equal)
		{
			cursori=1;
			LCD_VidClear();
			cleararrs();
			LCD_VidSendNumber(5);
			nums[ni]=nums[ni]*10+5;
		}
		else if(prev_state==one||prev_state==two||prev_state==three||prev_state==four||prev_state==five||prev_state==six||prev_state==seven||prev_state==eight||prev_state==nine||prev_state==0)
		{
			checkShift();
			cursori++;
			LCD_VidSendNumber(5);
			nums[ni]=nums[ni]*10+5;
		}
		else if(prev_state==multiply||prev_state==divide||prev_state==plus||prev_state==minus)
		{
			checkShift();
			cursori++;
			ni++;
			LCD_VidSendNumber(5);
			nums[ni]=nums[ni]*10+5;
		}
	}
	else if(curr_state==six)
	{
		if(prev_state==none||prev_state==empty)
		{
			checkShift();
			cursori++;
			LCD_VidSendNumber(6);
			nums[ni]=nums[ni]*10+6;
		}
		else if(prev_state==equal)
		{
			cursori=1;
			LCD_VidClear();
			cleararrs();
			LCD_VidSendNumber(6);
			nums[ni]=nums[ni]*10+6;
		}
		else if(prev_state==one||prev_state==two||prev_state==three||prev_state==four||prev_state==five||prev_state==six||prev_state==seven||prev_state==eight||prev_state==nine||prev_state==0)
		{
			cursori++;
			checkShift();
			LCD_VidSendNumber(6);
			nums[ni]=nums[ni]*10+6;
		}
		else if(prev_state==multiply||prev_state==divide||prev_state==plus||prev_state==minus)
		{
			checkShift();
			cursori++;
			ni++;
			LCD_VidSendNumber(6);
			nums[ni]=nums[ni]*10+6;
		}
	}
	else if(curr_state==seven)
	{
		if(prev_state==none||prev_state==empty)
		{
			checkShift();
			cursori++;
			LCD_VidSendNumber(7);
			nums[ni]=nums[ni]*10+7;
		}
		else if(prev_state==equal)
		{
			cursori=1;
			LCD_VidClear();
			cleararrs();
			LCD_VidSendNumber(7);
			nums[ni]=nums[ni]*10+7;
		}
		else if(prev_state==one||prev_state==two||prev_state==three||prev_state==four||prev_state==five||prev_state==six||prev_state==seven||prev_state==eight||prev_state==nine||prev_state==0)
		{
			checkShift();
			cursori++;
			LCD_VidSendNumber(7);
			nums[ni]=nums[ni]*10+7;
		}
		else if(prev_state==multiply||prev_state==divide||prev_state==plus||prev_state==minus)
		{
			checkShift();
			cursori++;
			ni++;
			LCD_VidSendNumber(7);
			nums[ni]=nums[ni]*10+7;
		}
	}
	else if(curr_state==eight)
	{
		if(prev_state==none||prev_state==empty)
		{
			checkShift();
			cursori++;
			LCD_VidSendNumber(8);
			nums[ni]=nums[ni]*10+8;
		}
		else if(prev_state==equal)
		{
			cursori=1;
			LCD_VidClear();
			cleararrs();
			LCD_VidSendNumber(8);
			nums[ni]=nums[ni]*10+8;
		}
		else if(prev_state==one||prev_state==two||prev_state==three||prev_state==four||prev_state==five||prev_state==six||prev_state==seven||prev_state==eight||prev_state==nine||prev_state==0)
		{
			checkShift();
			cursori++;
			LCD_VidSendNumber(8);
			nums[ni]=nums[ni]*10+8;
		}
		else if(prev_state==multiply||prev_state==divide||prev_state==plus||prev_state==minus)
		{
			checkShift();
			cursori++;
			ni++;
			LCD_VidSendNumber(8);
			nums[ni]=nums[ni]*10+8;
		}
	}
	else if(curr_state==nine)
	{
		if(prev_state==none||prev_state==empty)
		{
			checkShift();
			cursori++;
			LCD_VidSendNumber(9);
			nums[ni]=nums[ni]*10+9;
		}
		else if(prev_state==equal)
		{
			cursori=1;
			LCD_VidClear();
			cleararrs();
			LCD_VidSendNumber(9);
			nums[ni]=nums[ni]*10+9;
		}
		else if(prev_state==one||prev_state==two||prev_state==three||prev_state==four||prev_state==five||prev_state==six||prev_state==seven||prev_state==eight||prev_state==nine||prev_state==0)
		{
			checkShift();
			cursori++;
			LCD_VidSendNumber(9);
			nums[ni]=nums[ni]*10+9;
		}
		else if(prev_state==multiply||prev_state==divide||prev_state==plus||prev_state==minus)
		{
			checkShift();
			cursori++;
			ni++;
			LCD_VidSendNumber(9);
			nums[ni]=nums[ni]*10+9;
		}
	}
	else if(curr_state==zero)
	{
		if(prev_state==none||prev_state==empty)
		{
			checkShift();
			cursori++;
			LCD_VidSendNumber(0);
			nums[ni]=nums[ni]*10;
		}
		else if(prev_state==equal)
		{
			cursori=1;
			LCD_VidClear();
			cleararrs();
			LCD_VidSendNumber(0);
			nums[ni]=nums[ni]*10;
		}
		else if(prev_state==one||prev_state==two||prev_state==three||prev_state==four||prev_state==five||prev_state==six||prev_state==seven||prev_state==eight||prev_state==nine||prev_state==0)
		{
			checkShift();
			cursori++;
			LCD_VidSendNumber(0);
			nums[ni]=nums[ni]*10;
		}
		else if(prev_state==multiply||prev_state==divide||prev_state==plus||prev_state==minus)
		{
			checkShift();
			cursori++;
			ni++;
			LCD_VidSendNumber(0);
			nums[ni]=nums[ni]*10;
		}
	}
	else if(curr_state==plus)
	{
		if(prev_state==one||prev_state==two||prev_state==three||prev_state==four||prev_state==five||prev_state==six||prev_state==seven||prev_state==eight||prev_state==nine||prev_state==0)
		{
			checkShift();
			cursori++;
			LCD_VidSendData('+');
			chars[charsi]='+';
			charsi++;
			nopsc=1;
		}
		else if(prev_state==multiply||prev_state==divide||prev_state==plus||prev_state==minus)
		{
			checkShift();
			cursori++;
			LCD_VidSendData('+');
			negindices[ni+1]=0;
			nopsc++;
		}
		else if(prev_state==empty||prev_state==none)
		{
			checkShift();
			cursori++;
			LCD_VidSendData('+');
			negindices[0]=0;
			curr_state=empty;
			nopsc++;
		}
		else if(prev_state==equal)
		{
			cursori=4;
			LCD_VidClear();
			cleararrs();
			nums[ni]=ans;
			LCD_VidWrite("ANS");
			LCD_VidSendData('+');
			nopsc=1;
			chars[charsi++]='+';
		}
	}
	else if(curr_state==minus)
	{
		if(prev_state==one||prev_state==two||prev_state==three||prev_state==four||prev_state==five||prev_state==six||prev_state==seven||prev_state==eight||prev_state==nine||prev_state==0)
		{
			checkShift();
			cursori++;
			LCD_VidSendData('-');
			chars[charsi++]='-';
			nopsc=1;
		}
		else if(prev_state==multiply||prev_state==divide||prev_state==plus||prev_state==minus)
		{
			checkShift();
			cursori++;
			LCD_VidSendData('-');
			negindices[ni+1]=1;
			nopsc++;
		}
		else if(prev_state==empty||prev_state==none)
		{
			cursori++;
			LCD_VidSendData('-');
			negindices[0]=1;
			curr_state=empty;
			nopsc++;
		}
		else if(prev_state==equal)
		{
			cursori=4;
			LCD_VidClear();
			cleararrs();
			nums[ni]=ans;
			LCD_VidWrite("ANS");
			LCD_VidSendData('-');
			nopsc=1;
			chars[charsi++]='-';
		}
	}
	else if(curr_state==multiply)
	{
		if(prev_state==one||prev_state==two||prev_state==three||prev_state==four||prev_state==five||prev_state==six||prev_state==seven||prev_state==eight||prev_state==nine||prev_state==0)
		{
			checkShift();
			cursori++;
			LCD_VidSendData('*');
			chars[charsi++]='*';
			nopsc=1;
		}
		else if(prev_state==multiply||prev_state==divide||prev_state==plus||prev_state==minus||prev_state==empty||prev_state==none)
		{
			cursori++;
			err=true;
		}

		else if(prev_state==equal)
		{
			cursori=4;
			LCD_VidClear();
			cleararrs();
			nums[ni]=ans;
			LCD_VidWrite("ANS");
			LCD_VidSendData('*');
			nopsc=1;
			chars[charsi++]='*';
		}
	}
	else if(curr_state==divide)
	{
		if(prev_state==one||prev_state==two||prev_state==three||prev_state==four||prev_state==five||prev_state==six||prev_state==seven||prev_state==eight||prev_state==nine||prev_state==0)
		{
			checkShift();
			cursori++;
			LCD_VidSendData('/');
			chars[charsi++]='/';
			nopsc=1;
		}
		else if(prev_state==multiply||prev_state==divide||prev_state==plus||prev_state==minus||prev_state==empty||prev_state==none)
		{
			cursori++;
			err=true;
		}

		else if(prev_state==equal)
		{
			cursori=4;
			LCD_VidClear();
			cleararrs();
			LCD_VidWrite("ANS");
			nums[ni]=ans;
			LCD_VidSendData('/');
			nopsc=1;
			chars[charsi++]='/';
		}
	}
	else if(curr_state==equal)
	{
		if(prev_state==multiply||prev_state==divide||prev_state==plus||prev_state==minus)
		{
			err=true;
		}
		else{
			calc();
			LCD_VidSetCursorPosition(1,0);
			LCD_VidPrintFloat(ans);
			for(int i=0;i<shifts;i++)
				LCD_VidShiftRight();
		}
	}
}

void main()
{

    /*initializing the LCD and the KEypad*/
	LCD_VidInitialise();
	KEYPAD_VidInit();
	while(1)
	{
		/*Calculator steps*/
		getstate();
		checkoverops();
		error();
		proceed();
	}
}

