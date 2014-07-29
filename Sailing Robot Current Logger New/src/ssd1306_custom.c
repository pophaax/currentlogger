/*
 * ssd1306_custom.c
 *
 * Created: 6/30/2014 10:36:42 AM
 *  Author: sailbot
 */ 
#include "ssd1306_custom.h"
#include "pins.h"
#include <math.h>

int ssd1306_custom_init(void)
{
	return set_pins(USART2_SPI_pins);
}

void ssd1306_clear_char(int num)
{
	for(int a = 0; a < num; a++)
	ssd1306_write_data(0x00);
}

void ssd1306_draw_graph(uint8_t col, uint8_t page, uint8_t width, uint8_t height, uint8_t *tab, uint32_t offset)
{
	uint8_t page_start;
	uint8_t i, j, k, s;
	uint8_t scale;

	for (i = col; i < width; ++i) {
		for (page_start = page; page_start <= height; ++page_start) {
			ssd1306_write_command(SSD1306_CMD_SET_PAGE_START_ADDRESS(page_start));
			ssd1306_set_column_address(i);
			j = log( tab[i+offset] ) * 5;
			scale = 8 * (height - page_start + 1);
			if (j > scale)
			j = 8;
			else
			j -= (scale - 8);

			for (k = 0, s = j; j > 0; --j)
			k = (k << 1) + 1;
			for (s = 8 - s; s > 0; --s)
			k <<= 1;
			ssd1306_write_data(k);
		}
	}
}
