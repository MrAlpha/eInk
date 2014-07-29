/*
 * ======== Standard MSP430 includes ========
 */
#include <msp430.h>

#include "pinDefine.h"
#include "eInk.h"
#include "EPD.h"




/*
 * ======== Grace related declaration ========
 */
extern void Grace_init(void);

//========= Globale Variablen deklarationen ===================================

extern volatile unsigned char uartBuf=0;	// ||
extern volatile unsigned char Flag=0;	// used for Flags 0bxxxxxxxx <-LSB
											//Bit:
											//		0: INCOMING	new Byte written to uartBuf
											//		1: DISCARD	this and upcoming data Packages are not for our address
											//
extern volatile unsigned char packageCountdown=0;	// states the number of upcoming packages from the uart to be discarded or written to memory

//*****************************************toolkit init***********************
#include "Pervasive_Displays_small_EPD.h"
#if (defined EPD_KIT_TOOL_FUNCTIONS)
#include "EPD_Kit_Tool_Process.h"
#else
#include "image_data.h"
#endif

#define DCO_1MHz  0
#define DCO_8MHz  1
#define DCO_12MHz 2
#define DCO_16MHz 3

/**
 * \brief Set CPU work Frequency
 *
 * \param select_index The defined CPU frequency
 * */
void set_MCU_frequency(uint8_t select_index) {

	switch (select_index) {
	case DCO_1MHz:
		if (CALBC1_1MHZ == 0xFF || CALDCO_1MHZ == 0xFF) {
			while (1)
				; // If calibration constants erased, do not load, trap CPU.
		}
		BCSCTL1 = CALBC1_1MHZ; // Set range
		DCOCTL = CALDCO_1MHZ; // Set DCO step + modulation */
		break;
	case DCO_8MHz:
		if (CALBC1_8MHZ == 0xFF || CALDCO_8MHZ == 0xFF) {
			while (1)
				;
		}
		BCSCTL1 = CALBC1_8MHZ;
		DCOCTL = CALDCO_8MHZ;
		break;
	case DCO_12MHz:
		if (CALBC1_12MHZ == 0xFF || CALDCO_12MHZ == 0xFF) {
			while (1)
				;
		}
		BCSCTL1 = CALBC1_12MHZ;
		DCOCTL = CALDCO_12MHZ;
		break;
	case DCO_16MHz:
		if (CALBC1_16MHZ == 0xFF || CALDCO_16MHZ == 0xFF) {
			//Info is missing, guess at a good value.
			BCSCTL1 = 0x8f; //CALBC1_16MHZ at 0x10f9
			DCOCTL = 0x9C;  //CALDCO_16MHZ at 0x10f8
		} else {
			BCSCTL1 = CALBC1_16MHZ;
			DCOCTL = CALDCO_16MHZ;
		}
		break;
	}
}

/**
 * \brief LaunchPad initialization
 */
void system_init(void) {
	WDTCTL = WDTPW + WDTHOLD; // Stop Watchdog Timer
	set_MCU_frequency(DCO_16MHz); //set CPU work Frequency
	__delay_cycles(1600000);
	__delay_cycles(1600000);

}

//*************************************end Toolkitinit
/*
 *  ======== main ========
 */
int main( void )
{
	//************* Funktions deklarationen **************************************************************

	unsigned char stateMachine(unsigned char, unsigned char*, unsigned char*, unsigned char*);

	//************* Variablen deklarationen **********************************************************

	//unsigned char state=0;		//Status d. Statemachine
	unsigned char deviceID=0;

	unsigned char operationsID=0;				//buffers OperationsID (last Instruction)

	//unsigned char payload=0;

	unsigned char payload[64]={1};
	unsigned char check[64]={0};

	unsigned char packages=0;

	//************* Zeiger deklarationen **************************************************

	//unsigned char *pOprID = &operationsID;
	//unsigned char *ppayload =&payload;
	//unsigned char *ppayload= payload;


    Grace_init();                   	// Activate Grace-generated configuration


    CMD_PP(0,payload,64);
    flash_cmd_read(0,check,64);



    while(1){

    	if(Flag & INCOMING){
    		Flag &= ~INCOMING;
    		stateMachine(deviceID, &operationsID, payload, &packages);
    	}



    }
    return (0);
}
