/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * This is a bare minimum user application template.
 *
 * For documentation of the board, go \ref group_common_boards "here" for a link
 * to the board-specific documentation.
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to board_init()
 * -# Basic usage of on-board LED and button
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>

#include <custom_board_init.h>
#include <math.h>
#include <ssd1306_custom.h>
#include <conf_custom.h>
#include <ast_timer.h>

#include <pins.h>
#include <adc.h>
#include <log.h>

#include <string.h>

#define STRING_EOL    "\r"
#define STRING_HEADER "-- Current logger --\r\n" \
"-- "BOARD_NAME" --\r\n" \
"-- Compiled: "__DATE__" "__TIME__" --" STRING_EOL

#define SAMPLING_MODE 1
#define REPLAY_MODE 2
#define STARTUP_MODE 3

int8_t current[CURRENT_BUFFER_SIZE];

/** Conversion value */
uint16_t adc_sample_data[SAMPLE_BUFFER_SIZE];
uint32_t adc_sample_index;
uint64_t mean_sample = 0;

/** ADC instance */
struct adc_dev_inst adc_instance;

/** ADCIFE test mode structure */
struct {
	uint8_t uc_trigger_mode;
	uint8_t uc_pdc_en;
	uint8_t uc_gain_en;
} g_adc_test_mode;

uint8_t mode;
uint8_t mode_changing;

uint16_t timer;				// stores how long time the sampling has been going on
int16_t cursor;				// cursor used for displaying data when replaying
uint16_t cursor_speed_up;	// If cursor buttons are held, speed up the movement

FIL log_file_object;

bool update_replay; // update data when moving cursor...

// mode changing button
void OLED_BUTTON1_handler(void)
{
	if( mode == SAMPLING_MODE)
	{
		mode = REPLAY_MODE;
		mode_changing = 1;
	}
	else if(mode == REPLAY_MODE)
	{
		mode = SAMPLING_MODE;
		mode_changing = 1;
		start_adc();
		
		// Draw rolling graph
		ssd1306_draw_graph(1, 1, DISPLAY_BUFFER_SIZE, 3, &current, CURRENT_BUFFER_SIZE - DISPLAY_BUFFER_SIZE - cursor);		
	}
		
	else
	{
		mode = SAMPLING_MODE;
		mode_changing = 1;
		start_adc();
		
		// Draw rolling graph
		ssd1306_draw_graph(1, 1, DISPLAY_BUFFER_SIZE, 3, &current, CURRENT_BUFFER_SIZE - DISPLAY_BUFFER_SIZE - cursor);		
	}
		
	// wait for button to be released before allowing another interrupt	
	gpio_disable_pin_interrupt(OLED_BUTTON1);
}

// move cursor left button
void OLED_BUTTON2_handler(void)
{
	update_replay = true; // update display
	cursor_speed_up = 0; // reset speedup "timer"
		
	// wait for button to be released before allowing another interrupt		
	gpio_disable_pin_interrupt(OLED_BUTTON2);
}

// move cursor right button
void OLED_BUTTON3_handler(void)
{
	update_replay = true; // update display
	cursor_speed_up = 0; // reset speedup "timer"
	
 	// wait for button to be released before allowing another interrupt
	gpio_disable_pin_interrupt(OLED_BUTTON3);
}

static int configure_OLED_pins(void)
{
	/* Configure Button1 interrupt*/
	gpio_enable_pin_interrupt(OLED_BUTTON1);
	gpio_set_pin_callback(OLED_BUTTON1, OLED_BUTTON1_handler, 3);
	
	/* Configure Button2 interrupt*/
	gpio_enable_pin_interrupt(OLED_BUTTON2);
	gpio_set_pin_callback(OLED_BUTTON2, OLED_BUTTON2_handler, 3);
	
	/* Configure Button3 interrupt*/
	gpio_enable_pin_interrupt(OLED_BUTTON3);
	gpio_set_pin_callback(OLED_BUTTON3, OLED_BUTTON3_handler, 3);
	
	return 0;
}

/**
 * AST interrupt handler
 * Asynchronous timer, used to generate the time measurement
 */
void ast_per_callback(void)
{
	// stop timer	
	//ast_stop(AST);
	ast_clear_interrupt_flag(AST, AST_INTERRUPT_PER);	
	
	timer++;
	
	// Draw rolling graph
	ssd1306_draw_graph(1, 1, DISPLAY_BUFFER_SIZE, 3, &current, CURRENT_BUFFER_SIZE - DISPLAY_BUFFER_SIZE);
		
	/*Shift buffer.*/
	for (int i = 0; i < (CURRENT_BUFFER_SIZE - 1); ++i)
	{
		current[i] = current[i + 1];
	}
	
}

// console for debugging purpose
void configure_console(void)
{

	const usart_serial_options_t usart_serial_options = {
		.baudrate = CONSOLE_USART_BAUDRATE,
		.charlength = CONSOLE_USART_CHAR_LENGTH,
		.paritytype = CONSOLE_USART_PARITY,
		.stopbits = CONSOLE_USART_STOP_BITS,
	};

	stdio_serial_init(CONSOLE_USART, &usart_serial_options);
}

// converts raw data to comma separated value
char* data_to_csv(int8_t* data, uint32_t size)
{
	char* csv_string = malloc(size*4 + 1);
	
	memset(csv_string, 0, size*4 + 1);
	
	for(int a = (CURRENT_BUFFER_SIZE - timer); a < CURRENT_BUFFER_SIZE; a++)
	{
		sprintf(csv_string, "%s%d,", csv_string, data[a]);
	}
	
	sprintf(csv_string, "%s\0", csv_string);
		
	return csv_string;
}


int main (void)
{
	// Initiate clocks
	sysclk_init();
	
	// Board init
	board_init();
	custom_init();
	
	configure_console();
	printf( STRING_HEADER );
	
	printf("Configuring OLED button pins...");	
	if( configure_OLED_pins() == 0)
		printf("OK\n\r");
	else
		printf("Error when configuring OLED button pins\n\r");	
	
	printf("Configuring ssd1306 pins... ");
	if( ssd1306_custom_init() == 0)
		printf("OK\n\r");
	else
		printf("Error when configuring ssd1306 pins\n\r");
	
	printf("Configuring ssd1306 display...");
	ssd1306_init();
	printf("OK\n\r");
	
	delay_us(100);
	ssd1306_clear();
	
	printf("Initiating analog to digital converter... ");
	if( init_adc() == 0)
		printf("OK\n\r");
	else
		printf("Error when initiating ADC\n\r");
	
	printf("Initiating asynchronous timer... ");
	if( config_ast() == 0)
		printf("OK\n\r");
	else
		printf("Error when initiating Asynchronous timer\n\r");
		
	timer = 0; 
	cursor = 0;
	update_replay = false;

	static uint32_t count = 0;
	
	mode = STARTUP_MODE;
	mode_changing = 1;
	
 	irq_initialize_vectors();
 	cpu_irq_enable();
	
	printf("Initiating SD MMC... ");
	sd_mmc_init();
	printf("OK\n\r");
	
	ioport_set_pin_dir(PIN_PA16, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(PIN_PA15, IOPORT_DIR_OUTPUT);
		
	while (1) {
		
		// Is OLEDbutton1 released?
		if (ioport_get_pin_level(OLED_BUTTON1) == 1) {
			// Yes, so turn interrupt back on.
			gpio_enable_pin_interrupt(OLED_BUTTON1);
		}
		
		// Is OLEDbutton2 released?
		if (ioport_get_pin_level(OLED_BUTTON2) == 1) {
			// Yes, so turn interrupt back on.
			gpio_enable_pin_interrupt(OLED_BUTTON2);
		}
		
		// Is OLEDbutton3 released?
		if (ioport_get_pin_level(OLED_BUTTON3) == 1) {
			// Yes, so turn interrupt back on.
			gpio_enable_pin_interrupt(OLED_BUTTON3);
		}
		
		if( mode == SAMPLING_MODE)
		{
			if(mode_changing == 1)
			{			
				printf("Initiating Sampling Mode... ");
				
				mode_changing = 0;
				
				adc_enable_interrupt(&adc_instance, ADC_SEQ_SEOC);
			
				ssd1306_clear();
				ast_start(AST);
			
				printf("OK\n\r");
			}		
									
			adc_disable_interrupt(&adc_instance, ADC_SEQ_SEOC);
			ast_disable_interrupt(AST, AST_INTERRUPT_PER);
					
			char buffer[40];
		
			ssd1306_set_column_address(0);
			ssd1306_set_page_address(0);	// row
	
			sprintf(buffer, "Sampling %0.2gA     %3dm%2ds   ", (mean_sample - 2048.0)/102.4, timer/60, timer%60);
		
			ssd1306_write_text( buffer );
			
			adc_enable_interrupt(&adc_instance, ADC_SEQ_SEOC);
			ast_enable_interrupt(AST, AST_INTERRUPT_PER);
			
			delay_ms(300);			
		}
		
		// in replay mode, the current sampled can be viewed by moving the cursor with button 2 and 3	
		else if( mode == REPLAY_MODE)
		{
			// Only when changing from sampling to replay mode
			if(mode_changing == 1)
			{			
				printf("Initiating Replay Mode... ");
					
				// stop timer	
				ast_stop(AST);
				
				// disable adc interrupt (TODO disable ADC?)
				adc_disable_interrupt(&adc_instance, ADC_SEQ_SEOC);
				
				// clear oled display
				ssd1306_clear();	
				
				// set first column, first row
				ssd1306_set_column_address(0);
				ssd1306_set_page_address(0);	// row
								
				// try to write data to SD card
				ssd1306_write_text("Writing data to SD card...");
				
				if( write_log( data_to_csv( &current, timer), "current_data") == 0 )
					ssd1306_write_text("OK");
				else
					ssd1306_write_text("FAIL");
					
				// show if success or not 1 second so the user have time to see if it worked.	
				delay_ms(1000);
				
				// clear oled display
				ssd1306_clear();	
				
				printf("OK\n\r");
			}
			
			// following runs when changing from sampling to replay mode AND also when pressing a cursor button
			if( (mode_changing == 1) || update_replay )
			{
				mode_changing = 0;
							
				update_replay = false;
			
				char buffer[80];
		
				ssd1306_set_column_address(0);
				ssd1306_set_page_address(0);	// row
	
				// display current value
				sprintf(buffer, "Replaying %0.2fA   ", current[CURRENT_BUFFER_SIZE - 1 - cursor] / 127.0 * 20.0);
				ssd1306_write_text( buffer );
			
				// display time
				ssd1306_set_column_address(85);
				sprintf(buffer, " %3dm%2ds", (timer - cursor)/60, (timer - cursor)%60);
			
				ssd1306_write_text( buffer );
			
				// Draw rolling graph
				ssd1306_draw_graph(1, 1, DISPLAY_BUFFER_SIZE, 3, &current, CURRENT_BUFFER_SIZE - DISPLAY_BUFFER_SIZE - cursor);	
				
			}	
			
			delay_ms(100);
			
			if( ioport_get_pin_level(OLED_BUTTON2) == 0)
			{
				// move cursor left
				if( cursor <= timer)
				{
					if(cursor_speed_up < 10)
						cursor++;
					else if( (cursor_speed_up >= 10) && (cursor_speed_up < 20) )
						cursor += 5;
					else if(cursor_speed_up > 20)
						cursor += 10;
						
					cursor = min(cursor, timer);
						
					cursor_speed_up++;
					
					update_replay = true;
				}
			}
			
			if( ioport_get_pin_level(OLED_BUTTON3) == 0)
			{
				// move cursor right
				if(cursor > 0)
				{
					if(cursor_speed_up < 10)
						cursor--;
					else if( (cursor_speed_up >= 10) && (cursor_speed_up < 20) )
						cursor -= 5;
					else if(cursor_speed_up > 20)
						cursor -= 10;
						
					cursor = max(cursor, 0);
						
					cursor_speed_up++;						
					
					update_replay = true;
				}
			}
			
		}
		
		// Startup mode is only used when starting up
		else if( mode == STARTUP_MODE)
		{
			if(mode_changing == 1)
			{
				mode_changing = 0;
				
				adc_disable_interrupt(&adc_instance, ADC_SEQ_SEOC);
				ssd1306_clear();
				
				ssd1306_set_column_address(0);
				ssd1306_set_page_address(0);	// row
				ssd1306_write_text("Current logger");
			
				ssd1306_set_column_address(0);
				ssd1306_set_page_address(1);	// row
				ssd1306_write_text("Press Button 1 to start");
			
				ssd1306_set_column_address(0);
				ssd1306_set_page_address(2);	// row
				ssd1306_write_text("Display update rate: 1Hz");
				
				ssd1306_set_column_address(0);
				ssd1306_set_page_address(3);	// row
				char buffer[60];
				sprintf(buffer, "Sampling frequency: %dHz", SAMPLING_FREQUENCY);
				ssd1306_write_text( buffer );
			}	
			
			
		}
	
	}
}
