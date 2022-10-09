/***********************************************************/
/***********************************************************/
/******** Author  	 : Omar Said           *****************/
/******** File name  : BIT_MATH.h          *****************/
/******** Date	     : 17/9/2022           *****************/
/******** Version  	 : V1                  *****************/
/******** Desc  	 : ..                  *****************/
/***********************************************************/
/***********************************************************/


#ifndef BIT_MATH_H
#define BIT_MATH_H

/*Sets a single bit*/
#define SET_BIT(var , num)    var|=(1<<num)

/*Clears a single bit*/
#define CLR_BIT(var , num)    var&=(~(1<<num))

/*Toggless a single bit*/
#define TOGGLE_BIT(var , num) var^=(1<<num)

/*Gets a specific bit value*/
#define GET_BIT(var , num)   ((var>>num)&1)

/*Checks if a number is a power of two*/
#define POWEROF2(var)        !(var&(var-1))


	
// printf("%hhu", ch); for unsigned chars.
#endif