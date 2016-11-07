/*
 * DHT11_2560_C.c
 *
 * Created: 11/3/2016 8:18:34 PM
 * Author : charlie
 */ 

#include <avr/io.h>
#include "driver/uart.h"
#include "driver/dht.h"
#define F_CPU 16000000UL
#include <util/delay.h>

int main(void)
{
	int8_t humidity;
	int8_t temperature;
	
	USART0_SETUP_9600_BAUD_ASSUME_1MHz_CLOCK();
	fprintf(USART,"Reading temperature and humidity...\n");
    while (1) 
    {
	    dht_gettemperaturehumidity(&temperature, &humidity);
	    fprintf(USART,"temperature:%d,humidity:%d\n\t",temperature,humidity);
		_delay_ms(1000);
    }
}

