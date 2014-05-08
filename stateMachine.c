/*
 * stateMachine.c
 *
 *  Created on: 07.05.2014
 *      Author: Dave
 */

#include "global.h"


unsigned char stateMachine(unsigned char state, unsigned char deviceID, unsigned char *pPgeCdn, unsigned char *pOprID){



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
		else{						//if not discard Telegram TODOOOOOOOOOOOOO
			*pPgeCdn=(*pOprID)& 0x3F; //schreibe anzahl der Telegramme in Countdown ____ noch unterscheiden ob daten kommen
			state=0;
		}
		break;
	}
return state;
}
