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
	

#define P1_PWM_PIN             AVR32_PWM_0_0_PIN
#define P1_PWM_FUNCTION        AVR32_PWM_0_0_FUNCTION
#define P1_PWM_CHANNEL_ID      0
#define P2_PWM_PIN             AVR32_PWM_1_0_PIN
#define P2_PWM_FUNCTION        AVR32_PWM_1_0_FUNCTION
#define P2_PWM_CHANNEL_ID      1
#define P3_PWM_PIN             AVR32_PWM_2_0_PIN
#define P3_PWM_FUNCTION        AVR32_PWM_2_0_FUNCTION
#define P3_PWM_CHANNEL_ID      2
#define P4_PWM_PIN             AVR32_PWM_3_0_PIN
#define P4_PWM_FUNCTION        AVR32_PWM_3_0_FUNCTION
#define P4_PWM_CHANNEL_ID      3
#define P5_PWM_PIN             AVR32_PWM_4_0_PIN
#define P5_PWM_FUNCTION        AVR32_PWM_4_0_FUNCTION
#define P5_PWM_CHANNEL_ID      4

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

	sysclk_init();

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
	avr32_pwm_channel_t pc1 = { .ccnt = 0 };
	avr32_pwm_channel_t pc2 = { .ccnt = 0 };
	avr32_pwm_channel_t pc3 = { .ccnt = 0 };
	avr32_pwm_channel_t pc4 = { .ccnt = 0 };
	avr32_pwm_channel_t pc5 = { .ccnt = 0 };
	
	pc1.cdty = 5; /* Channel duty cycle, should be < CPRD. */
	pc1.cprd = 40; /* Channel period. */
	pc1.cupd = 0; /* Channel update is not used here. */
	pc1.CMR.calg = PWM_MODE_LEFT_ALIGNED; /* Channel mode. */
	pc1.CMR.cpol = PWM_POLARITY_LOW;      /* Channel polarity. */
	pc1.CMR.cpd = PWM_UPDATE_DUTY;        /* Not used the first time. */
	pc1.CMR.cpre = AVR32_PWM_CPRE_MCK_DIV_128; /* Channel prescaler. */	

	pc2.cdty = 5; /* Channel duty cycle, should be < CPRD. */
	pc2.cprd = 40; /* Channel period. */
	pc2.cupd = 0; /* Channel update is not used here. */
	pc2.CMR.calg = PWM_MODE_LEFT_ALIGNED; /* Channel mode. */
	pc2.CMR.cpol = PWM_POLARITY_LOW;      /* Channel polarity. */
	pc2.CMR.cpd = PWM_UPDATE_DUTY;        /* Not used the first time. */
	pc2.CMR.cpre = AVR32_PWM_CPRE_MCK_DIV_128; /* Channel prescaler. */

	pc3.cdty = 5; /* Channel duty cycle, should be < CPRD. */
	pc3.cprd = 40; /* Channel period. */
	pc3.cupd = 0; /* Channel update is not used here. */
	pc3.CMR.calg = PWM_MODE_LEFT_ALIGNED; /* Channel mode. */
	pc3.CMR.cpol = PWM_POLARITY_LOW;      /* Channel polarity. */
	pc3.CMR.cpd = PWM_UPDATE_DUTY;        /* Not used the first time. */
	pc3.CMR.cpre = AVR32_PWM_CPRE_MCK_DIV_128; /* Channel prescaler. */

	pc4.cdty = 5; /* Channel duty cycle, should be < CPRD. */
	pc4.cprd = 40; /* Channel period. */
	pc4.cupd = 0; /* Channel update is not used here. */
	pc4.CMR.calg = PWM_MODE_LEFT_ALIGNED; /* Channel mode. */
	pc4.CMR.cpol = PWM_POLARITY_LOW;      /* Channel polarity. */
	pc4.CMR.cpd = PWM_UPDATE_DUTY;        /* Not used the first time. */
	pc4.CMR.cpre = AVR32_PWM_CPRE_MCK_DIV_128; /* Channel prescaler. */

	pc5.cdty = 5; /* Channel duty cycle, should be < CPRD. */
	pc5.cprd = 40; /* Channel period. */
	pc5.cupd = 0; /* Channel update is not used here. */
	pc5.CMR.calg = PWM_MODE_LEFT_ALIGNED; /* Channel mode. */
	pc5.CMR.cpol = PWM_POLARITY_LOW;      /* Channel polarity. */
	pc5.CMR.cpd = PWM_UPDATE_DUTY;        /* Not used the first time. */
	pc5.CMR.cpre = AVR32_PWM_CPRE_MCK_DIV_128; /* Channel prescaler. */



	
	/* Enable the alternative mode of the output pin to connect it to the PWM
	 * module within the device. */
	gpio_enable_module_pin(P1_PWM_PIN, P1_PWM_FUNCTION);
	gpio_enable_module_pin(P2_PWM_PIN, P2_PWM_FUNCTION);
	gpio_enable_module_pin(P3_PWM_PIN, P3_PWM_FUNCTION);
	gpio_enable_module_pin(P4_PWM_PIN, P4_PWM_FUNCTION);
	gpio_enable_module_pin(P5_PWM_PIN, P5_PWM_FUNCTION);

	/* Initialize the PWM module. */
	pwm_init(&pwm_opt);

	/* Set channel configuration to channel 0. */
	pwm_channel_init(P1_PWM_CHANNEL_ID, &pc1);
	pwm_channel_init(P2_PWM_CHANNEL_ID, &pc2);
	pwm_channel_init(P3_PWM_CHANNEL_ID, &pc3);
	pwm_channel_init(P4_PWM_CHANNEL_ID, &pc4);
	pwm_channel_init(P5_PWM_CHANNEL_ID, &pc5);

	/* Start channel 0. */
	pwm_start_channels(1 << P1_PWM_CHANNEL_ID);
	pwm_start_channels(1 << P2_PWM_CHANNEL_ID);
	pwm_start_channels(1 << P3_PWM_CHANNEL_ID);
	pwm_start_channels(1 << P4_PWM_CHANNEL_ID);
	pwm_start_channels(1 << P5_PWM_CHANNEL_ID);




	while (true) {
		delay_ms(1);
		/* Start conversions on all enabled channels */
		adc_start(&AVR32_ADC);

		/* Get value for the temperature adc channel */
		adc_value_temp = adc_get_value(&AVR32_ADC,
				EXAMPLE_ADC_TEMPERATURE_CHANNEL);

		adc_value_temp = (adc_value_temp >> 2) + 4;

		/* Get value for the light adc channel */
		adc_value_light = adc_get_value(&AVR32_ADC,
				EXAMPLE_ADC_LIGHT_CHANNEL);

		adc_value_temp = (adc_value_temp >> 2) + 4;


		pc1.CMR.cpd = PWM_UPDATE_PERIOD;
		pc1.cupd = (adc_value_light);
 		pwm_async_update_channel(P1_PWM_CHANNEL_ID, &pc1);
 		pc2.CMR.cpd = PWM_UPDATE_PERIOD;
		pc2.cupd = (adc_value_temp);
 		pwm_async_update_channel(P2_PWM_CHANNEL_ID, &pc2);
 		pc3.CMR.cpd = PWM_UPDATE_PERIOD;
		pc3.cupd = (adc_value_temp);
 		pwm_async_update_channel(P3_PWM_CHANNEL_ID, &pc3);
 		pc4.CMR.cpd = PWM_UPDATE_PERIOD;
		pc4.cupd = (adc_value_temp);
 		pwm_async_update_channel(P4_PWM_CHANNEL_ID, &pc4);
 		pc5.CMR.cpd = PWM_UPDATE_PERIOD;
		pc5.cupd = (adc_value_temp);
 		pwm_async_update_channel(P5_PWM_CHANNEL_ID, &pc5);

 		delay_ms(2);

 		pc1.CMR.cpd = PWM_UPDATE_DUTY;
		pc1.cupd = (adc_value_light) >> 1;
 		pwm_async_update_channel(P1_PWM_CHANNEL_ID, &pc1);
 		pc2.CMR.cpd = PWM_UPDATE_DUTY;
		pc2.cupd = (adc_value_temp) >> 1;
 		pwm_async_update_channel(P2_PWM_CHANNEL_ID, &pc2);
 		pc3.CMR.cpd = PWM_UPDATE_DUTY;
		pc3.cupd = (adc_value_temp) >> 1;
 		pwm_async_update_channel(P3_PWM_CHANNEL_ID, &pc3);
  		pc4.CMR.cpd = PWM_UPDATE_DUTY;
		pc4.cupd = (adc_value_temp) >> 1;
 		pwm_async_update_channel(P4_PWM_CHANNEL_ID, &pc4);
  		pc5.CMR.cpd = PWM_UPDATE_DUTY;
		pc5.cupd = (adc_value_temp) >> 1;
 		pwm_async_update_channel(P5_PWM_CHANNEL_ID, &pc5);


	}
}
