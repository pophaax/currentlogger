/*
 * pins.c
 * This file is used for simple configuration of pins to modules
 *
 * Created: 6/30/2014 10:29:42 AM
 *  Author: sailbot
 */ 

#include <pins.h>

#define ioport_set_pin_peripheral_mode(pin, mode) \
do {\
	ioport_set_pin_mode(pin, mode);\
	ioport_disable_pin(pin);\
} while (0)

int set_pins(int module)
{
	switch (module)
	{
		case USART0_pins:
		ioport_set_pin_peripheral_mode(PIN_PB00B_USART0_RXD, MUX_PB00B_USART0_RXD);
		ioport_set_pin_peripheral_mode(PIN_PB01B_USART0_TXD, MUX_PB01B_USART0_TXD);
		break;
		
		case USART1_pins:
		ioport_set_pin_peripheral_mode(EXT1_PIN_UART_RX, EXT1_UART_RX_MUX);
		ioport_set_pin_peripheral_mode(EXT1_PIN_UART_TX, EXT1_UART_TX_MUX);
		break;
		
		case USART2_SPI_pins:
		ioport_set_pin_peripheral_mode(PIN_PA20A_USART2_TXD, MUX_PA20A_USART2_TXD); // MOSI
		ioport_set_pin_peripheral_mode(PIN_PA19A_USART2_RXD, MUX_PA19A_USART2_RXD); // MISO
		ioport_set_pin_peripheral_mode(PIN_PA18A_USART2_CLK, MUX_PA18A_USART2_CLK); // SCK
		ioport_set_pin_peripheral_mode(PIN_PA17A_USART2_RTS, MUX_PA17A_USART2_RTS); // SS
		
		ioport_set_pin_dir(PIN_PC15, IOPORT_DIR_OUTPUT); // Data/Command
		ioport_set_pin_mode(PIN_PC15, IOPORT_MODE_PULLUP);
		ioport_set_pin_dir(PIN_PA10, IOPORT_DIR_OUTPUT); // Reset
		ioport_set_pin_mode(PIN_PA10, IOPORT_MODE_PULLUP);
		break;
		
		case TC10_pins:
		ioport_set_pin_peripheral_mode(PIN_PC00D_TC1_A0, MUX_PC00D_TC1_A0);
		ioport_set_pin_peripheral_mode(PIN_PC01D_TC1_B0, MUX_PC01D_TC1_B0);
		break;
		
		case ADC0_pin:
		ioport_set_pin_peripheral_mode(PIN_PA04A_ADCIFE_AD0, MUX_PA04A_ADCIFE_AD0);
		break;
		
		case ADC1_pin:
		ioport_set_pin_peripheral_mode(PIN_PA05A_ADCIFE_AD1, MUX_PA05A_ADCIFE_AD1);
		break;
		
		case ADC2_pin:
		ioport_set_pin_peripheral_mode(PIN_PA07A_ADCIFE_AD2, MUX_PA07A_ADCIFE_AD2);
		break;
		
		case ADC3_pin:
		ioport_set_pin_peripheral_mode(PIN_PB02A_ADCIFE_AD3, MUX_PB02A_ADCIFE_AD3);
		break;
		
		default:
			return 1;
	}
	
	return 0;
}

void reset_pins(int module)
{
	switch (module)
	{
		case USART0_pins:
		ioport_enable_pin(PIN_PB00B_USART0_RXD);
		ioport_enable_pin(PIN_PB01B_USART0_TXD);
		break;
		
		case USART1_pins:
		ioport_enable_pin(EXT1_PIN_UART_RX);
		ioport_enable_pin(EXT1_PIN_UART_TX);
		break;
		
		case USART2_SPI_pins:
		ioport_enable_pin(PIN_PA20A_USART2_TXD); // MOSI
		ioport_enable_pin(PIN_PA19A_USART2_RXD); // MISO
		ioport_enable_pin(PIN_PA18A_USART2_CLK); // SCK
		ioport_enable_pin(PIN_PA17A_USART2_RTS); // SS
		
		break;
		
		case TC10_pins:
		ioport_enable_pin(PIN_PC00D_TC1_A0);
		ioport_enable_pin(PIN_PC01D_TC1_B0);
		break;
		
	}
}