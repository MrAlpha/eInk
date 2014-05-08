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

extern volatile unsigned char uartBuf=0;	//
extern volatile unsigned char intrFlag=0;	// Für Datenübergabe aus ISR



/*
 *  ======== main ========
 */
int main( void )
{
	//************* Funktions deklarationen **************************************************************

	void stateMachine(unsigned char, unsigned char, unsigned char*, unsigned char* );

	//************* Variablen deklarationen **********************************************************

	//unsigned char state=0;		//Status d. Statemachine
	unsigned char deviceID=0;
	unsigned char packageCowntdown=0;	// if !=0 states the number of upcomming packages from the uart to be discarded
	unsigned char operationsID=0;				//buffers OperationsID (last Instruction)

	//************* Zeiger deklarationen **************************************************

	unsigned char *pPgeCdn = &packageCowntdown;
	unsigned char *pOprID = &operationsID;
	unsigned char state=0;



    Grace_init();                   	// Activate Grace-generated configuration


    while(1){

    	if(intrFlag==1){
    		intrFlag=0;
    		stateMachine(state, deviceID, pPgeCdn,pOprID);
    	}


    }
    return (0);
}
