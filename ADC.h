#pragma once
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Utils.h"

struct ADC_name {
	static constexpr auto val = 42;
};

struct ADC_1 : public ADC_name {
	static constexpr auto val = 1;
};

struct ADC_2 : public ADC_name {
	static constexpr auto val = 1;
};

struct ADC_3 : public ADC_name {
	static constexpr auto val = 1;
};

struct ADC_4 : public ADC_name {
	static constexpr auto val = 1;
};



template<typename first, typename... ADCs>
struct ADCList {
	using type = utils::list<first, ADCs...>;
	static constexpr auto adc_count = type::size;
};

template< template <typename ,typename...> typename ADCs>
struct ADC {
	static_assert(utils::isEqual<ADCList, ADCs>::value, "ungültiger template Parameter, ADC benötigt ADCList als Parameter");

	void initADC(void);
	uint16_t getAdcValue(void);
	void switchToAdc(uint8_t adc);


	 volatile uint16_t adc_result[ADCList::adc_count];

	uint8_t currentChannel = utils::getType<0,ADCs::type>::val;
	uint8_t channel_sel = ADC_1::val;

	void initADC(void) {
		ADMUX |= (1 << REFS0);					//REF voltage
		ADCSRA |= (1 << ADPS1) | (1 << ADPS0);	//ADC clockprescale /8
		ADCSRA |= (1 << ADEN) | (1 << ADIE);		//Enable + Interrupt
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

		if (channel_sel == 1) {
			ADMUX &= ~(1 << MUX0);
			channel_sel = 0;
		}
		else {
			ADMUX |= (1 << MUX0);
			channel_sel = 1;
		}

		// Start the next conversion.
		ADCSRA |= (1 << ADSC);
	}
};
