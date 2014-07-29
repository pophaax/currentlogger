/*
 * adc.c
 *
 * Created: 6/30/2014 10:21:55 AM
 *  Author: sailbot
 */ 

#include <adc.h>
#include <conf_custom.h>

#include <ssd1306_custom.h>
#include <adcife.h>
#include <pins.h>

uint8_t current[DISPLAY_BUFFER_SIZE];

/** Conversion value */
uint16_t adc_sample_data[SAMPLE_BUFFER_SIZE];
uint32_t adc_sample_index;
extern uint64_t mean_sample;

/** Conversion done flag */
volatile uint8_t conversion_done_flag = 0;

/** ADC instance */
extern struct adc_dev_inst adc_instance;

/** ADCIFE test mode structure */
struct {
	uint8_t uc_trigger_mode;
	uint8_t uc_pdc_en;
	uint8_t uc_gain_en;
} g_adc_test_mode;

/** ADCIFE trigger modes */
enum {
	TRIGGER_MODE_SOFTWARE = 0,
	TRIGGER_MODE_ITIMER,
	TRIGGER_MODE_CON,
} e_trigger_mode;

int init_adc(void)
{
	g_adc_test_mode.uc_pdc_en = 0;
	g_adc_test_mode.uc_trigger_mode = TRIGGER_MODE_ITIMER;
	g_adc_test_mode.uc_gain_en = 0;
	
	if( set_pins(ADC2_pin) == 1)
		return 1;
		
	if( set_pins(ADC3_pin) == 1)
		return 1;
	
	adc_sample_index = 0;
	
	return 0;
}

void start_adc(void)
{
	struct adc_config adc_cfg = {
		/* System clock division factor is 8 */
		.prescal = ADC_PRESCAL_DIV8,
		/* The APB clock is used */
		.clksel = ADC_CLKSEL_APBCLK,
		/* Max speed is 150K */
		.speed = ADC_SPEED_150K,
		/* ADC Reference voltage is 0.625*VCC */
		.refsel = ADC_REFSEL_1,
		/* Enables the Startup time */
		.start_up = CONFIG_ADC_STARTUP
	};
	struct adc_seq_config adc_seq_cfg = {
		/* Select Vref for shift cycle */
		.zoomrange = ADC_ZOOMRANGE_0,
		/* Pad Ground */
		.muxneg = ADC_MUXNEG_3,
		/* DAC internal */
		.muxpos = ADC_MUXPOS_2,
		/* Enables the internal voltage sources */
		.internal = ADC_INTERNAL_0,
		/* Disables the ADC gain error reduction */
		.gcomp = ADC_GCOMP_DIS,
		/* Disables the HWLA mode */
		.hwla = ADC_HWLA_DIS,
		/* 12-bits resolution */
		.res = ADC_RES_12_BIT,
		/* Enables the single-ended mode */
		.bipolar = ADC_BIPOLAR_DIFFERENTIAL
	};
	struct adc_ch_config adc_ch_cfg = {
		.seq_cfg = &adc_seq_cfg,
		/* Internal Timer Max Counter */
		.internal_timer_max_count = sysclk_get_pba_hz() / (8 * SAMPLING_FREQUENCY),
		/* Window monitor mode is off */
		.window_mode = 0,
		.low_threshold = 0,
		.high_threshold = 0,
	};
	
	if(adc_init(&adc_instance, ADCIFE, &adc_cfg) != STATUS_OK) {
		//puts("-F- ADC Init Fail!\n\r");
		while(1);
	}
	if(adc_enable(&adc_instance) != STATUS_OK) {
		//puts("-F- ADC Enable Fail!\n\r");
		while(1);
	}

	if (g_adc_test_mode.uc_pdc_en) {
		//adc_disable_interrupt(&g_adc_inst, ADC_SEQ_SEOC);
		//adc_pdca_set_config(&g_adc_pdca_cfg);
		//pdca_channel_set_callback(CONFIG_ADC_PDCA_RX_CHANNEL, pdca_transfer_done,
		//		PDCA_0_IRQn, 1, PDCA_IER_TRC);
		} else {
		pdca_channel_disable_interrupt(CONFIG_ADC_PDCA_RX_CHANNEL,
		PDCA_IDR_TRC);
		pdca_channel_disable_interrupt(CONFIG_ADC_PDCA_TX_CHANNEL,
		PDCA_IDR_TRC);
		adc_ch_set_config(&adc_instance, &adc_ch_cfg);
		adc_set_callback(&adc_instance, ADC_SEQ_SEOC, adcife_read_conv_result,
		ADCIFE_IRQn, 1);
	}

	/* Configure trigger mode and start convention. */
	switch (g_adc_test_mode.uc_trigger_mode) {
		case TRIGGER_MODE_SOFTWARE:
		adc_configure_trigger(&adc_instance, ADC_TRIG_SW);
		break;
		case TRIGGER_MODE_CON:
		adc_configure_trigger(&adc_instance, ADC_TRIG_CON);
		break;
		case TRIGGER_MODE_ITIMER:
		adc_configure_trigger(&adc_instance, ADC_TRIG_INTL_TIMER);
		adc_configure_itimer_period(&adc_instance,
		adc_ch_cfg.internal_timer_max_count);
		adc_start_itimer(&adc_instance);
		break;
		default:
		break;
	}

	if (g_adc_test_mode.uc_gain_en) {
		adc_configure_gain(&adc_instance, ADC_GAIN_2X);
		} else {
		adc_configure_gain(&adc_instance, ADC_GAIN_1X);
	}
}

void disable_adc(void)
{
	adc_disable_interrupt(&adc_instance, ADC_SEQ_SEOC);
}

void enable_adc(void)
{
	adc_enable_interrupt(&adc_instance, ADC_SEQ_SEOC);
}

void adcife_read_conv_result(void)
{
	ioport_set_pin_level(PIN_PA16, true); // for debugging
	
	if ((adc_get_status(&adc_instance) & ADCIFE_SR_SEOC) == ADCIFE_SR_SEOC) {
		adc_sample_data[adc_sample_index++] = adc_get_last_conv_value(&adc_instance);
		conversion_done_flag = 1;
		adc_clear_status(&adc_instance, ADCIFE_SCR_SEOC);
	}
	
	if(adc_sample_index >= SAMPLE_BUFFER_SIZE)
	{
		ioport_set_pin_level(PIN_PA15, true);
		
		adc_sample_index = 0;
		
		for(int a = 0; a < (SAMPLE_BUFFER_SIZE); a++)
		{
			mean_sample += adc_sample_data[a];
		}
		
		mean_sample /= SAMPLE_BUFFER_SIZE;
		
		current[CURRENT_BUFFER_SIZE - 1] = (int)( mean_sample/16 - 127 );
		
		ioport_set_pin_level(PIN_PA15, false);
	}
	
	ioport_set_pin_level(PIN_PA16, false);
}
