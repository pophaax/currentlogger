/*
 * ast_timer.c
 *
 * Created: 6/30/2014 1:53:58 PM
 *  Author: sailbot
 */ 

#include <ast_timer.h>

int config_ast(void)
{
	struct ast_config ast_conf;

	/* Enable osc32 oscillator */
	if (!osc_is_ready(OSC_ID_OSC32)) {
		osc_enable(OSC_ID_OSC32);
		osc_wait_ready(OSC_ID_OSC32);
	}

	/* Enable the AST. */
	ast_enable(AST);

	ast_conf.mode = AST_COUNTER_MODE;
	ast_conf.osc_type = AST_OSC_1KHZ;
	ast_conf.psel = AST_PSEL_32KHZ_1HZ;
	ast_conf.counter = 0;
	ast_set_config(AST, &ast_conf);
	ast_stop(AST);

	/* Set periodic 0 to interrupt after 8 second in counter mode. */
	ast_clear_interrupt_flag(AST, AST_INTERRUPT_PER);
	ast_write_periodic0_value(AST, AST_PSEL_32KHZ_1HZ - 5);
	ast_enable_wakeup(AST, AST_WAKEUP_PER);
	/* Set callback for periodic0. */
	ast_set_callback(AST, AST_INTERRUPT_PER, ast_per_callback,
	AST_PER_IRQn, 2);
	
	return 0;
}