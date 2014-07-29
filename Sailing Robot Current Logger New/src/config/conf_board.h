/**
 * \file
 *
 * \brief SAM4L Xplained Pro board configuration
 *
 * Copyright (C) 2013 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 * 
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 * 
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 */

#ifndef CONF_BOARD_H_INCLUDED
#define CONF_BOARD_H_INCLUDED

//#define CONF_BOARD_KEEP_WATCHDOG_AT_INIT

#define CONF_BOARD_COM_PORT
#define CONF_BOARD_SD_MMC_SPI
#define CONF_BOARD_SPI_NPCS0

/* OLED1 board butttons*/
#define OLED_BUTTON1 IOPORT_CREATE_PIN(IOPORT_GPIOA, 6)
#define OLED_BUTTON2 IOPORT_CREATE_PIN(IOPORT_GPIOB, 3)
#define OLED_BUTTON3 IOPORT_CREATE_PIN(IOPORT_GPIOB, 4)

#define OLED_LED1 IOPORT_CREATE_PIN(IOPORT_GPIOA, 8)
#define OLED_LED2 IOPORT_CREATE_PIN(IOPORT_GPIOA, 9)
#define OLED_LED3 IOPORT_CREATE_PIN(IOPORT_GPIOC, 16)

#endif /* CONF_BOARD_H_INCLUDED */
