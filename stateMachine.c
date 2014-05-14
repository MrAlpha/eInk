/*
 * stateMachine.c
 *
 *  Created on: 07.05.2014
 *      Author: Dave
 */
#include <msp430.h>

#include "global.h"
#include "pinDefine.h"


unsigned char stateMachine(unsigned char deviceID, unsigned char *pOprID, unsigned char *ppayload){

	static unsigned char state=0;

	switch(state){

	case 0:
		if (uartBuf== 0xAB)		//Preamble (0b10101011)?
			state=1;
		else
			state=0;
		break;

	case 1:
		if (uartBuf== 0xFF)		//Delimiter?
			state=2;
		else
			state=0;
		break;

	case 2:
		*pOprID= uartBuf;		//write received command to operations ID buffer
		state=3;
		break;

	case 3:
		if (uartBuf==deviceID)		//Address == own Address?
			state=4;
		else if(!(*pOprID & 0XC0)){		//not own address ->check if data will be transmitted
			packageCountdown=(*pOprID)& 0x3F; //...if so, write quantity to count-down
			state=0;
		}
		else
			state=0;
		break;

	case 4:
		*ppayload= uartBuf;
		P1OUT ^= (1<<pin0);
		break;
	}
return deviceID;
}
