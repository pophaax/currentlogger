/*
 * ssd1306_custom.h
 *
 * Created: 6/30/2014 10:36:08 AM
 *  Author: sailbot
 */ 

#include <compiler.h>
#include <sysclk.h>
#include <ioport.h>
#include <status_codes.h>
#include <delay.h>

#include <ssd1306.h>

#ifndef SSD1306_CUSTOM_H_
#define SSD1306_CUSTOM_H_

int ssd1306_custom_init(void);
void ssd1306_clear_char(int);
void ssd1306_draw_graph(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t*, uint32_t);

#endif /* SSD1306_CUSTOM_H_ */