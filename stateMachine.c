/*
 * stateMachine.c
 *
 *  Created on: 07.05.2014
 *      Author: Dave
 */

#include "global.h"


unsigned char stateMachine(unsigned char state){

	switch(state){

	case 0:
		if (uartBuf== 0xAB)	//Preamble (0b10101011)?
			state=1;
		break;

	case 1:
		if (uartBuf== 0xFF){		//Delimiter?
			state=2;
		}
		else{
			state=0;
		}
		break;

	case 2:
		if (uartBuf==deviceID){		//Adress == own Adress?
			state=3;
		}
		else{						//if not discard Telegramm TODOOOOOOOOOOOOO
			state=0;
		}
		break;
	case 3:
		//foo
		break;
	}
return state;
}
