/*
 * libADC.h
 *
 * Created: 2017-04-23 20:32:55
 *  Author: Sebastian
 */ 
#ifndef _LIBADC_H_
#define _LIBADC_H_

#define CHANNELS  2

void initADC(void);
uint16_t getAdcValue(void);
void switchToAdc(uint8_t adc);
#endif