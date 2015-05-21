#include <stdio.h>

// asf
#include "delay.h"
#include "compiler.h"
#include "flashc.h"
#include "preprocessor.h"
#include "intc.h"
#include "pm.h"
#include "gpio.h"
#include "sysclk.h"
#include "pwm.h"
#include "adc.h"

// this
#include "conf_board.h"
	

#define EXAMPLE_PWM_PIN             AVR32_PWM_0_0_PIN
#define EXAMPLE_PWM_FUNCTION        AVR32_PWM_0_0_FUNCTION
#define EXAMPLE_PWM_CHANNEL_ID      0

#define EXAMPLE_ADC_TEMPERATURE_CHANNEL     0
#define EXAMPLE_ADC_TEMPERATURE_PIN         AVR32_ADC_AD_0_PIN
#define EXAMPLE_ADC_TEMPERATURE_FUNCTION    AVR32_ADC_AD_0_FUNCTION

#define EXAMPLE_ADC_LIGHT_CHANNEL           1
#define EXAMPLE_ADC_LIGHT_PIN               AVR32_ADC_AD_1_PIN
#define EXAMPLE_ADC_LIGHT_FUNCTION          AVR32_ADC_AD_1_FUNCTION


/** GPIO pin/adc-function map. */
const gpio_map_t ADC_GPIO_MAP = {
	{EXAMPLE_ADC_TEMPERATURE_PIN, EXAMPLE_ADC_TEMPERATURE_FUNCTION},
	{EXAMPLE_ADC_LIGHT_PIN, EXAMPLE_ADC_LIGHT_FUNCTION}
};



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// main

int main(void)
{
	signed short adc_value_temp  = -1;
	signed short adc_value_light = -1;

	// sysclk_init();

	gpio_enable_module(ADC_GPIO_MAP, sizeof(ADC_GPIO_MAP) /
			sizeof(ADC_GPIO_MAP[0]));

		AVR32_ADC.mr |= 0x1 << AVR32_ADC_MR_PRESCAL_OFFSET;
	adc_configure(&AVR32_ADC);

	/* Enable the ADC channels. */
	adc_enable(&AVR32_ADC, EXAMPLE_ADC_TEMPERATURE_CHANNEL);
	adc_enable(&AVR32_ADC, EXAMPLE_ADC_LIGHT_CHANNEL);

	irq_initialize_vectors();
	// register_interrupts();
	cpu_irq_enable();

	// timer_add(&clockTimer,120,&clockTimer_callback, NULL);

	pwm_opt_t pwm_opt =
		{
			.diva = AVR32_PWM_DIVA_CLK_OFF,
			.divb = AVR32_PWM_DIVB_CLK_OFF,
			.prea = AVR32_PWM_PREA_MCK,
			.preb = AVR32_PWM_PREB_MCK
		};

	/* PWM channel configuration structure. */
	avr32_pwm_channel_t pwm_channel = { .ccnt = 0 };
	
	/* With these settings, the output waveform period will be:
	 * (115200/256)/20 == 22.5Hz == (MCK/prescaler)/period, with
	 * MCK == 115200Hz, prescaler == 256, period == 20. */
	pwm_channel.cdty = 5; /* Channel duty cycle, should be < CPRD. */
	pwm_channel.cprd = 40; /* Channel period. */
	pwm_channel.cupd = 0; /* Channel update is not used here. */
	pwm_channel.CMR.calg = PWM_MODE_LEFT_ALIGNED; /* Channel mode. */
	pwm_channel.CMR.cpol = PWM_POLARITY_LOW;      /* Channel polarity. */
	pwm_channel.CMR.cpd = PWM_UPDATE_DUTY;        /* Not used the first time. */
	pwm_channel.CMR.cpre = AVR32_PWM_CPRE_MCK_DIV_128; /* Channel prescaler. */	
	
	/* Enable the alternative mode of the output pin to connect it to the PWM
	 * module within the device. */
	gpio_enable_module_pin(EXAMPLE_PWM_PIN, EXAMPLE_PWM_FUNCTION);

	/* Initialize the PWM module. */
	pwm_init(&pwm_opt);

	/* Set channel configuration to channel 0. */
	pwm_channel_init(EXAMPLE_PWM_CHANNEL_ID, &pwm_channel);

	/* Start channel 0. */
	pwm_start_channels(1 << EXAMPLE_PWM_CHANNEL_ID);




	while (true) {
		delay_ms(10);
		/* Start conversions on all enabled channels */
		adc_start(&AVR32_ADC);

		/* Get value for the temperature adc channel */
		adc_value_temp = adc_get_value(&AVR32_ADC,
				EXAMPLE_ADC_TEMPERATURE_CHANNEL);

		adc_value_temp = (adc_value_temp >> 4) + 2;


		pwm_channel.CMR.cpd = PWM_UPDATE_PERIOD;
		pwm_channel.cupd = (adc_value_temp & 0x3f);
 		pwm_async_update_channel(EXAMPLE_PWM_CHANNEL_ID, &pwm_channel);
 		delay_ms(2);
 		pwm_channel.CMR.cpd = PWM_UPDATE_DUTY;
		pwm_channel.cupd = (adc_value_temp & 0x3f) >> 1;
 		pwm_async_update_channel(EXAMPLE_PWM_CHANNEL_ID, &pwm_channel);

		/* Get value for the light adc channel */
		adc_value_light = adc_get_value(&AVR32_ADC,
				EXAMPLE_ADC_LIGHT_CHANNEL);


	}
}
