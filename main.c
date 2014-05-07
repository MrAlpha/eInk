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


extern volatile unsigned char uartBuf=0;
extern volatile unsigned char intrFlag=0;
extern unsigned char deviceID=0;


/*
 *  ======== main ========
 */
int main( void )
{
	unsigned char stateMachine(unsigned char);

	unsigned char state=0;

    Grace_init();                   	// Activate Grace-generated configuration


    while(1){

    	if(intrFlag==1){
    		intrFlag=0;
    		stateMachine(state);
    	}


    }
    return (0);
}
