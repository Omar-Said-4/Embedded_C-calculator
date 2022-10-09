/***********************************************************/
/***********************************************************/
/******** Author  	 : Omar Said           *****************/
/******** File name  : DIO_INTERFACE.h     *****************/
/******** Date	     : ---------           *****************/
/******** Version  	 : V1                  *****************/
/******** Desc  	 : --             **********************/
/***********************************************************/
/***********************************************************/

#ifndef DIO_INTERFACE_H
#define DIO_INTERFACE_H
#include"STD_TYPES.h"
#define OUTPUT 1
#define INPUT 0
#define HIGH 1
#define LOW 0
#define PORT_OUTPUT 255
#define PORT_INPUT 0
/*Sets direction of a specific pin in a specicfic port*/
void DIO_VidSetPinDirection(u8 port, u8 pin, u8 dir);

/*Sets direction of a specicfic port*/
void DIO_VidSetPortDirection(u8 port, u8 dir);

/*Sets output value of a specific pin in a specicfic port*/
void DIO_VidSetPinValue(u8 port, u8 pin, u8 val);

/*Sets output value of a specicfic port*/
void DIO_VidSetPortValue(u8 port,u8 val);

/*Gets input value of a specific pin in a specicfic port*/
u8 DIO_U8GetPinValue(u8 port, u8 pin);

/*Toggles output value of a specific pin in a specicfic port*/
void DIO_VidTogglePinValue(u8 port,u8 pin);

#endif