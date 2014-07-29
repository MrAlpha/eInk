/**
 * \file
 *
 * \brief The functions of UART driver
 *
 * Copyright (c) 2012-2014 Pervasive Displays Inc. All rights reserved.
 *
 *  Authors: Pervasive Displays Inc.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "EPD_Kit_Tool_Process.h"
#include "Uart_Driver.h"

static receive_event_handler _RxEventHandle;
uint8_t tx_buf[SERIAL_TX_MAX_LEN];
static uint8_t tx_iptr;
static uint8_t tx_optr;

/** \brief Initialize the Rx event and start USB Device stack
 */
void data_interface_init(receive_event_handler OnRxEventHandle) {
	_RxEventHandle = OnRxEventHandle;
	P1SEL |= BIT1 + BIT2; // P1.1 = RXD, P1.2=TXD
	P1SEL2 |= BIT1 + BIT2; // P1.1 = RXD, P1.2=TXD
	UCA0CTL1 |= UCSSEL_2; // SMCLK
	UCA0BR0 = 0x82; // 16MHz 9600 ,UCA0BRx=1666
	UCA0BR1 = 0x06; // 16MHz 9600
	UCA0MCTL = UCBRS1 + UCBRS0; // Modulation UCBRSx =6
	UCA0CTL1 &= ~UCSWRST; // **Initialize USCI state machine**
	IE2 |= UCA0RXIE; // Enable USCI_A0 RX interrupt
	tx_iptr = 0;
	tx_optr = 0;
}

void data_interface_detach(void) {
	IE2 &= ~UCA0RXIE; //Disable USCI_A0 RX interrup
}

/** \brief Transmit data to UART
 */
void data_transmit(uint8_t *s, uint8_t len) {
	tx_iptr=0;
	tx_optr=0;
	while (len--) {
		tx_buf[tx_iptr] = *s++;
		tx_iptr++;
		tx_iptr &= (SERIAL_TX_MAX_LEN - 1);

	}
	IE2 |= UCA0TXIE; // Enable USCI_A0 TX interrupt
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
	//while (!(IFG2&UCA0RXIFG));				   // USCI_A0 TX buffer ready?
	if (_RxEventHandle != NULL) {
		_RxEventHandle((uint8_t *) &UCA0RXBUF, 1);
	}
	LPM3_EXIT;
}

// USCI A0/B0 Transmit ISR
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void) {
	if (tx_iptr != tx_optr) {
		if (IFG2 & UCA0TXIFG) // USCI_A0 TX buffer ready?
		{
			UCA0TXBUF = tx_buf[tx_optr];
			tx_optr++;
			tx_optr &= (SERIAL_TX_MAX_LEN - 1);
			if (tx_optr == tx_iptr) {
				IE2 &= ~UCA0TXIE; // Disable USCI_A0 TX interrupt
			}
		}
	}

}

