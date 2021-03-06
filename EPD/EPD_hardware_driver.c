/**
 * \file
 *
 * \brief The initialization and configuration of COG hardware driver
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

#include <math.h>
//#include "EPD_hardware_driver.h"
#include "EPD.h"
static  uint16_t EPD_Counter;
static uint8_t spi_flag = FALSE;

/**
 * \brief Set up EPD Timer for 1 mSec interrupts
 *
 * \note
 * desired value: 1mSec
 * actual value:  1.000mSec
 */
static void initialize_EPD_timer(void) {
	//------------------Timer A1----------------------------------------
	//set up Timer_A CCR1 as master timer using ACLK	
	TA0CCTL2 &= ~(CCIFG | CCIE); // reset CCIFG Interrupt Flag
	TA0CTL = TASSEL_2 + MC_0 + TACLR + ID_3;
	TA0CCTL2 = OUTMOD_4;
	EPD_Counter = 0;
}

/**
 * \brief Start Timer
 */
void start_EPD_timer(void) {
	initialize_EPD_timer();
	TA0R = 0;
	TA0CCR0 = 990 * 2; // 1ms
	TA0CCTL2 |= CCIE;
	TA0CTL |= MC_1;
	EPD_Counter = 0;
}

/**
 * \brief Stop Timer
 */
void stop_EPD_timer(void) {
	TA0CCTL2 &= ~CCIE;
	TA0CTL &= ~MC_1;
}

/**
 * \brief Get current Timer after starting a new one
 */
uint32_t get_current_time_tick(void) {
	return EPD_Counter;
}
/**
 * \brief Set current Timer after starting a new one
 */
void set_current_time_tick(uint32_t count) {
     EPD_Counter=count;
}
/**
 * \brief Interrupt Service Routine for system tick counter
 */
void SysTick_Handler(void) {
	EPD_Counter++;
}

/**
 * \brief Interrupt Service Routine for Timer A0
 */
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A0(void) {
	switch (__even_in_range(TA0IV, 10)) {
	case 2:
		//LPM3_EXIT;
		break;

	case 4:
		EPD_Counter++;
		LPM3_EXIT;
		break;
	}

}

/**
 * \brief Delay mini-seconds
 * \param ms The number of mini-seconds
 */
void delay_ms(unsigned int ms) {
	while (ms--) {
		__delay_cycles(SMCLK_FREQ / 1000);
	}
}

/**
 * \brief Delay mini-seconds
 * \param ms The number of mini-seconds
 */
void sys_delay_ms(unsigned int ms) {
	delay_ms(ms);
}

static void Wait_10us(void) {
	__delay_cycles(SMCLK_FREQ / 100000);
}

//******************************************************************
//* PWM  Configuration/Control //PWM output : PD3
//******************************************************************

/**
 * \brief The PWM signal starts toggling
 */
void PWM_start_toggle(void) {

}

/**
 * \brief The PWM signal stops toggling.
 */
void PWM_stop_toggle(void) {

}

/**
 * \brief PWM toggling.
 *
 * \param ms The interval of PWM toggling (mini seconds)
 */
void PWM_run(uint16_t ms) {
	start_EPD_timer();
	do {
		EPD_pwm_high();
		__delay_cycles(30);
		EPD_pwm_low();
		__delay_cycles(30);
	} while (get_current_time_tick() < ms); //wait Delay Time
	stop_EPD_timer();
}

//******************************************************************
//* SPI  Configuration
//******************************************************************

/**
 * \brief Configure SPI
 */
void epd_spi_init(void) {
	if (spi_flag)
		return;
	spi_flag = TRUE;
	//config  i/o
	config_gpio_dir_o(SPICLK_PORT, SPICLK_PIN);
	config_gpio_dir_o(SPIMOSI_PORT, SPIMOSI_PIN);
	config_gpio_dir_i(SPIMISO_PORT, SPIMISO_PIN);

	BITSET(SPISEL, SPICLK_PIN + SPIMOSI_PIN + SPIMISO_PIN);
	BITSET(SPISEL2, SPICLK_PIN + SPIMOSI_PIN + SPIMISO_PIN);
	//comfig SPI
	SPICTL0 = UCCKPH | UCMST | UCSYNC | UCMSB;
	SPICTL1 = UCSSEL_2 + UCSWRST;
	SPIBR0 = 2; //16MHz/2=8MHz
	SPIBR1 = 0;

	BITSET(REN (SPIMISO_PORT), SPIMISO_PIN);
	BITCLR(SPICTL1, UCSWRST);
}

/**
 * \brief Initialize SPI
 */
void epd_spi_attach(void) {
	EPD_flash_cs_high();
	EPD_cs_high();
	epd_spi_init();
}

/**
 * \brief Disable SPI and change to GPIO
 */
void epd_spi_detach(void) {
	BITCLR(SPISEL, SPICLK_PIN + SPIMOSI_PIN + SPIMISO_PIN);
	BITCLR(SPISEL2, SPICLK_PIN + SPIMOSI_PIN + SPIMISO_PIN);
	//config_gpio_dir_o(SPICLK_PORT,SPICLK_PIN);
	//config_gpio_dir_o(SPIMOSI_PORT,SPIMOSI_PIN);
	//config_gpio_dir_o(SPIMISO_PORT,SPIMISO_PIN);
	SPIMISO_low();
	SPIMOSI_low();
	SPICLK_low();
	spi_flag = FALSE;
}

/**
 * \brief SPI synchronous write
 */
void epd_spi_write(unsigned char Data) {
	SPITXBUF = Data;
	while (!(SPIIFG & SPITXIFG))
		;
}
/**
 * \brief SPI synchronous read
 */
uint8_t epd_spi_read(unsigned char RDATA) {
	SPITXBUF = RDATA;
	while ((SPISTAT & UCBUSY))
		;
	RDATA = SPIRXBUF;
	return RDATA;
}

/**
 * \brief Send data to SPI with time out feature
 *
 * \param data The data to be sent out
 */
uint8_t epd_spi_write_ex(unsigned char Data) {
	uint8_t cnt = 200;
	uint8_t flag = 1;
	SPITXBUF = Data;
	while (!(SPIIFG & SPITXIFG)) {
		if ((cnt--) == 0) {
			flag = 0;
			break;
		}
	}
	return flag;
}

#if (defined COG_V230_G2)
/**
* \brief SPI command
*
* \param register_index The Register Index as SPI Data to COG
* \param register_data The Register Data for sending command data to COG
* \return the SPI read value
*/
uint8_t SPI_R(uint8_t Register, uint8_t Data) {
	uint8_t result;
	EPD_cs_low ();
	epd_spi_write (0x70); // header of Register Index
	epd_spi_write (Register);

	EPD_cs_high ();
	Wait_10us ();
	EPD_cs_low ();

	epd_spi_write (0x73); // header of Register Data of read command
	result=epd_spi_read (Data);

	EPD_cs_high ();

	return result;
}
#endif

/**
* \brief SPI command if register data is larger than two bytes
*
* \param register_index The Register Index as SPI command to COG
* \param register_data The Register Data for sending command data to COG
* \param length The number of bytes of Register Data which depends on which
* Register Index is selected.
*/
void epd_spi_send (unsigned char register_index, unsigned char *register_data,
               unsigned length) {
	EPD_cs_low ();
	epd_spi_write (0x70); // header of Register Index
	epd_spi_write (register_index);

	EPD_cs_high ();
	Wait_10us ();
	EPD_cs_low ();

	epd_spi_write (0x72); // header of Register Data of write command
	while(length--) {
		epd_spi_write (*register_data++);
	}
	EPD_cs_high ();
}

/**
* \brief SPI command
*
* \param register_index The Register Index as SPI command to COG
* \param register_data The Register Data for sending command data to COG
*/
void epd_spi_send_byte (uint8_t register_index, uint8_t register_data) {
	EPD_cs_low ();
	epd_spi_write (0x70); // header of Register Index
	epd_spi_write (register_index);

	EPD_cs_high ();
	Wait_10us ();
	EPD_cs_low ();
	epd_spi_write (0x72); // header of Register Data
	epd_spi_write (register_data);
	EPD_cs_high ();
}

//******************************************************************
//* Temperature sensor  Configuration
//******************************************************************
// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void) {
	__bic_SR_register_on_exit(CPUOFF);
	// Clear CPUOFF bit from 0(SR)
}
#ifdef __Internal_Temperature_Sensor
/**
 * \brief Get temperature value from ADC
 *
 * \return the Celsius temperature
 */
int16_t get_temperature(void) {
	const uint8_t DegCOffset=0;
	long temp;
	float IntDegC;

	ADC10CTL1 = INCH_10 + ADC10DIV_3; // Temp Sensor ADC10CLK/4
	ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON+ADC10IE;
	ADC10CTL0 |= ENC + ADC10SC;// Sampling and conversion start
	__bis_SR_register(CPUOFF + GIE);// LPM0, ADC10_ISR will force exit
	// oC = ((A10/1024)*1500mV)-986mV)*1/3.55mV = A10*423/1024 - 278
	temp = ADC10MEM;

	ADC10CTL0 |= ENC + ADC10SC;// Sampling and conversion start
	__bis_SR_register(CPUOFF + GIE);// LPM0, ADC10_ISR will force exit
	temp = ADC10MEM;
	ADC10CTL0 |= ENC + ADC10SC;// Sampling and conversion start
	__bis_SR_register(CPUOFF + GIE);// LPM0, ADC10_ISR will force exit
	temp += ADC10MEM;
	temp=temp/2;

	IntDegC =(long)((long)(temp*423)/1024)-(278+DegCOffset);//(long)((long)(temp - 673) * 423) / 1024;

	__no_operation();// SET BREAKPOINT HERE

	return (int8_t) IntDegC;
}
#elif defined __External_Temperature_Sensor

/**
 * \brief Get temperature value from ADC
 *
 * \return the Celsius temperature
 */
int16_t get_temperature(void) {
	float IntDegC;
	const uint8_t DegCOffset = 2;
	long temp;
	ADC10CTL0 |= ENC + ADC10SC; // Sampling and conversion start
	__bis_SR_register(CPUOFF + GIE);
	// LPM0, ADC10_ISR will force exit
	temp = ADC10MEM;

	ADC10CTL0 |= ENC + ADC10SC; // Sampling and conversion start
	__bis_SR_register(CPUOFF + GIE);
	// LPM0, ADC10_ISR will force exit
	temp = ADC10MEM;
	ADC10CTL0 |= ENC + ADC10SC; // Sampling and conversion start
	__bis_SR_register(CPUOFF + GIE);
	// LPM0, ADC10_ISR will force exit
	temp = ADC10MEM;
	ADC10CTL0 |= ENC + ADC10SC; // Sampling and conversion start
	__bis_SR_register(CPUOFF + GIE);
	// LPM0, ADC10_ISR will force exit
	temp = ADC10MEM;

	//org
	/*
	 temp = (ADC10MEM*5)/2;
	 voltage = (float)((float)temp*2.5)/1024.0;			//(2.5/1024)*ADC=Mcu voltage,Temperature voltage=Mcu voltage*2
	 IntDegC=100.0- (((voltage*1000)/10.77)-111.3);		//100-((Temperature voltage-1.199)*1000)/10.77=IntDegC
	 __no_operation();					  // SET BREAKPOINT HERE
	 */
	//adj
	// IntDegC=(203-DegCOffset)-((long)((7*temp)/128)+(temp/2));
	IntDegC = (201 - DegCOffset) - ((long) ((5 * temp) / 128) + (temp / 2));
	return (int16_t) IntDegC;
}

#endif

/**
 * \brief Initialize the temperature sensor
 */
void initialize_temperature(void) {
#ifdef __External_Temperature_Sensor

	ADC10CTL0 = SREF_1 + ADC10SHT_3 + ADC10ON + ADC10IE + REFON + REF2_5V; // ADC10ON, interrupt enabled
	ADC10CTL1 = INCH_4 + ADC10DIV_3; // input A1.4

#endif
}

/**
 * \brief Initialize the EPD hardware setting
 */
void EPD_display_hardware_init(void) {
	EPD_initialize_gpio();
	EPD_Vcc_turn_off();
	epd_spi_init();
	initialize_temperature();
	EPD_cs_low();
	EPD_pwm_low();
	EPD_rst_low();
	EPD_discharge_low();
	EPD_border_low();
	//initialize_EPD_timer();
}

