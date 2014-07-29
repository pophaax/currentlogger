/*
 * custom_board_init.c
 *
 * Created: 6/26/2014 3:56:04 PM
 *  Author: sailbot
 */ 

#include <custom_board_init.h>

void custom_init(void)
{
	/* Initialize OLED board buttons */
	ioport_set_pin_dir(OLED_BUTTON1, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(OLED_BUTTON1, IOPORT_MODE_PULLUP);
	ioport_set_pin_dir(OLED_BUTTON2, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(OLED_BUTTON2, IOPORT_MODE_PULLUP);
	ioport_set_pin_dir(OLED_BUTTON3, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(OLED_BUTTON3, IOPORT_MODE_PULLUP);

	ioport_set_pin_dir(OLED_LED1, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(OLED_LED2, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(OLED_LED3, IOPORT_DIR_OUTPUT);
	
	ioport_set_pin_level(OLED_LED1, true);
	ioport_set_pin_level(OLED_LED2, true);
	ioport_set_pin_level(OLED_LED3, true);

}