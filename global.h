/*
 * global.h
 *
 *  Created on: 07.05.2014
 *      Author: Dave
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

extern volatile unsigned char uartBuf;		//
extern volatile unsigned char Flag;		// Für Datenübergabe aus ISR
extern volatile unsigned char packageCountdown;

//========= defines for Flag Word ============================
#define INCOMING 	(1<<0)
#define DISCARD		(1<<1)

#endif /* GLOBAL_H_ */
