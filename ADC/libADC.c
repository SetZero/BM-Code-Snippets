/*
 * libADC.c
 *
 * Created: 2017-04-23 20:24:55
 *  Author: Sebastian
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "libADC.h"

volatile uint16_t adc_result[CHANNELS];

uint8_t currentChannel = 0;
uint8_t channel_sel = 0;

void initADC(void) {
	ADMUX |= (1 << REFS0);					//REF voltage
	ADCSRA |= (1 << ADPS1) | (1 << ADPS0);	//ADC clockprescale /8
	ADCSRA |= (1 << ADEN) | (1<<ADIE);		//Enable + Interrupt
	sei();									//be sure to enable interrupts
	ADCSRA |= (1 << ADSC);					//Start 1st conversion
}

uint16_t getAdcValue(void) {
	return adc_result[currentChannel];
}

void switchToAdc(uint8_t adc) {
	currentChannel = adc;
}

ISR(ADC_vect)
{
	// Save conversion result.
	adc_result[channel_sel] = ADC;
	
	if(channel_sel == 1) {
		ADMUX &= ~(1 << MUX0);
		channel_sel = 0;
	} else {
		ADMUX |= (1 << MUX0);
		channel_sel = 1;
	}

	// Start the next conversion.
	ADCSRA |= (1<<ADSC);
}
