/***********************************************************/
/***********************************************************/
/******** Author  	 : Omar Said           *****************/
/******** File name  : KEYPAD_INTERFACE.h  *****************/
/******** Date	     : 2/10/2022           *****************/
/******** Version  	 : V1                  *****************/
/******** Desc  	 : ..                  *****************/
/***********************************************************/
/***********************************************************/
#ifndef KEYPAD_INTERFACE_H
#define KEYPAD_INTERFACE_H
#include"STD_TYPES.h"

/* For initializing Keypad*/
void KEYPAD_VidInit();

/*For getting the keypad pressed key*/
u8 KEYPAD_U8GetPressedKey();

#endif
