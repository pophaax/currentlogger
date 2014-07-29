/*
 * log.h
 *
 * Created: 6/30/2014 3:26:33 PM
 *  Author: sailbot
 */ 

#include <compiler.h>
#include <sysclk.h>
#include <ioport.h>
#include <status_codes.h>
#include <delay.h>

#ifndef LOG_H_
#define LOG_H_

int open_log_file(void);
void close_log_file(void);
int write_log(char*, char*);
void test_mmc(void);

#endif /* LOG_H_ */