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
extern volatile unsigned char intrFlag=0;	// Für Datenübergabe aus ISR
extern volatile unsigned char packageCountdown=0;	// if !=0 it states the number of upcomming packages from the uart to be discarded


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

	unsigned char payload=0;

	//************* Zeiger deklarationen **************************************************

	unsigned char *pOprID = &operationsID;
	unsigned char *ppayload =&payload;



    Grace_init();                   	// Activate Grace-generated configuration


    while(1){

    	if(intrFlag==1){
    		intrFlag=0;
    		stateMachine(deviceID, pOprID, ppayload);
    	}


    }
    return (0);
}
