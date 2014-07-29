/*
 * adc.h
 *
 * Created: 6/30/2014 10:22:18 AM
 *  Author: sailbot
 */ 

#include <compiler.h>
#include <sysclk.h>
#include <ioport.h>
#include <status_codes.h>
#include <delay.h>

#ifndef ADC_H_
#define ADC_H_

int init_adc(void);
void start_adc(void);
void enable_adc(void);
void disable_adc(void);
void adcife_read_conv_result(void);

#endif /* ADC_H_ */