/*
 * ======== Standard MSP430 includes ========
 */
#include <msp430.h>

#include "pinDefine.h"
#include "global.h"




/*
 * ======== Grace related declaration ========
 */
extern void Grace_init(void);

//========= Globale Variablen deklarationen ===================================

extern volatile unsigned char uartBuf=0;	// ||
extern volatile unsigned char Flag=0;	// used for Flags
											//Bit:
											//		0: INCOMING	new Byte written to uartBuf
											//		1: DISCARD	this and upcoming data Packages are not for our address
											//
extern volatile unsigned char packageCountdown=0;	// states the number of upcoming packages from the uart to be discarded or written to memory


/*
 *  ======== main ========
 */
int main( void )
{
	//************* Funktions deklarationen **************************************************************

	unsigned char stateMachine(unsigned char, unsigned char*, unsigned char* );

	//************* Variablen deklarationen **********************************************************

	//unsigned char state=0;		//Status d. Statemachine
	unsigned char deviceID=0;

	unsigned char operationsID=0;				//buffers OperationsID (last Instruction)

	//unsigned char payload=0;

	unsigned char payload[64];

	//************* Zeiger deklarationen **************************************************

	//unsigned char *pOprID = &operationsID;
	//unsigned char *ppayload =&payload;
	//unsigned char *ppayload= payload;


    Grace_init();                   	// Activate Grace-generated configuration


    while(1){

    	if(Flag & INCOMING){
    		Flag &= ~INCOMING;
    		stateMachine(deviceID, &operationsID, payload);
    	}


    }
    return (0);
}
