/*
 * stateMachine.c
 *
 *  Created on: 07.05.2014
 *      Author: Dave
 */
#include <msp430.h>

#include "global.h"
#include "pinDefine.h"


unsigned char stateMachine(unsigned char deviceID, unsigned char *pOprID, unsigned char *payload){

	static unsigned short state=0;

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
		if (uartBuf==deviceID)		//Address == own Address?
			state=3;
		else					//not own address
			state=201;

		break;

	case 3:
		*pOprID= uartBuf;		//write received command to operations ID buffer
		if(!*pOprID & 0xC0){		//if Data will be send
			packageCountdown=(uartBuf)& 0x3F; //...write quantity to count-down
			state=4;
		}
		else
			state=0;
		break;

	case 4:							//TTToOOOOOOOODDDDDOOOOOO schau ob die indexe hier stimmen
		*payload++ = uartBuf;		//
		packageCountdown--;
		if(!packageCountdown)
			state=0;

		break;

//=======================================================
		//branch case 2:
	case 201:			//command is not for our Address
		if(!(uartBuf & 0XC0)){			//check if data will be transmitted
			packageCountdown=(*pOprID)& 0x3F; //...if so, write quantity to count-down to be able to discard them at the ISR
			state=0;
		}
		else
			state=0;


	}
return deviceID;
}
