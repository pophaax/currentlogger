/*
 * pins.h
 *
 * Created: 6/30/2014 10:29:53 AM
 *  Author: sailbot
 */ 

#include <compiler.h>
#include <sysclk.h>
#include <ioport.h>
#include <status_codes.h>
#include <delay.h>

#ifndef PINS_H_
#define PINS_H_

enum {
	TC10_pins = 1,
	USART0_pins,
	USART1_pins,
	USART2_SPI_pins,
	ADC0_pin,
	ADC1_pin,
	ADC2_pin,
	ADC3_pin,
	
} module_pin;

int set_pins(int module);


#endif /* PINS_H_ */