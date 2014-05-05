/*
 * ======== Standard MSP430 includes ========
 */
#include <msp430.h>

/*
 * ======== Grace related declaration ========
 */
extern void Grace_init(void);

/*
 *  ======== main ========
 */
int main( void )
{
    Grace_init();                   	// Activate Grace-generated configuration
   // UCA0CTL1 &=~(UCSWRST);				//USCI reset disabled --> wird bereits in der initialisierungsrutine erledigt
   // UCA0RXIE
    while(1){
    // >>>>> Fill-in user code here <<<<<
    }
    return (0);
}
