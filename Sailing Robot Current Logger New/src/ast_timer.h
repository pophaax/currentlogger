/*
 * ast_timer.h
 *
 * Created: 6/30/2014 1:53:22 PM
 *  Author: sailbot
 */ 
#include <compiler.h>
#include <sysclk.h>
#include <ioport.h>
#include <status_codes.h>
#include <delay.h>

#include <conf_custom.h>

#include <ast.h>

#ifndef AST_TIMER_H_
#define AST_TIMER_H_

void ast_per_callback(void);
int config_ast(void);

#endif /* AST_TIMER_H_ */