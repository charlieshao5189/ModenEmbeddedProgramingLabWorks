/*
 * ADC_SingleChannel0_LDR_2560_C.c
 *
 * Created: 11/2/2016 1:25:13 PM
 * Author : charlie
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "driver/adc.h"
#include "driver/uart.h"

extern volatile unsigned int ADC0_Value; 

int main(void)
{
	uint16_t adcValue=0;
    USART0_SETUP_9600_BAUD_ASSUME_1MHz_CLOCK();
	Initialise_ADC();
	
	fprintf(USART,"Program initial finish!\n\t");
	
    while (1) 
    {
      adcValue=adc_read(0);
	  fprintf(USART,"Trg: %x\n",adcValue);
	  _delay_ms(1000);
    }
}

