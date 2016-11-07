/*
 * adc.c
 *
 * Created: 11/2/2016 1:26:25 PM
 *  Author: charlie
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"
#include "uart.h"
#define F_CPU 16000000UL
#include <util/delay.h>

volatile unsigned int ADC0_Value; 

void Initialise_ADC(){
// ADMUX ?ADC Multiplexer Selection Register
// bit7,6 Reference voltage selection (00 AREF,01 AVCC, 10 = Internal 1.1V, 11 = Internal 2.56V)
// bit 5 ADC Left adjust the 10-bit result
// 0 = ADCH (high) contains bit 1 = output bit 9, bit 0 = output bit 8. ADCL (low) contains output bits 7 through 0
// 1 = ADCH (high) contains bits 9 through 2. ADCL (low) contains bit 7 = output bit 1, bit 6 = output bit 0
// Bits 4:0 ?MUX4:0: Analog Channel and Gain Selection Bits (see 1281 manual p290)
// 00000 = ADC0 (ADC channel 0, single-ended input)
// 00010 = ADC2 (ADC channel 2, single-ended input)
ADMUX = (1<<REFS0);	// AVCC REF, Left-adjust output (Read most-significant 8 bits via ADCH), Convert channel 2

// ADCSRA ?ADC Control and Status Register A
// bit 7 ADEN (ADC ENable) = 1 (Enabled)
// bit 6 ADSC (ADC Start Conversion) = 0 (OFF initially)
// bit 5 ADATE (ADC Auto Trigger Enable) = 0 (OFF)
// bit 4 ADIF (ADC Interrupt Flag) = 0 (not cleared)
// bit 3 ADIE (ADC Interrupt Enable) = 0 (disable the ADC Conversion Complete Interrupt)
// bit 2,1,0 ADC clock prescaler
// 000 = division factor 2
// 001 = division factor 2
// 010 = division factor 4
// 011 = division factor 8
// 100 = division factor 16
// 101 = division factor 32
// 110 = division factor 64
// 111 = division factor 128
ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);	// ADC enabled, Interrupt enabled, Prescaler = 128

// ADCSRB ?ADC Control and Status Register B
// Bit 3 ?MUX5: Analog Channel and Gain Selection Bit (always 0 when using ADC0 - ADC7)
// Bit 2:0 ?ADTS2:0: ADC Auto Trigger Source (active when ADATE bit in ADCSRA is set)
// 0 0 0 Free Running mode
// 0 0 1 Analog Comparator
// 0 1 0 External Interrupt Request 0
// 0 1 1 Timer/Counter0 Compare Match A
// 1 0 0 Timer/Counter0 Overflow
// 1 0 1 Timer/Counter1 Compare Match B
// 1 1 0 Timer/Counter1 Overflow
// 1 1 1 Timer/Counter1 Capture Event
ADCSRB &= 0b11110000;	// clear bits 3,2,1,0 (Free running mode)

// DIDR0 ?Digital Input Disable Register 0
// Bit 7:0 ?ADC7D:ADC0D: ADC7:0 Digital Input Disable
// DIDR0 = 0b00000000;	// Disable digital input on bit 2
// 
// DIDR2 ?Digital Input Disable Register 2
// Bit 7:0 ?ADC15D:ADC8D: ADC15:8 Digital Input Disable
// DIDR2 = 0b11111111;	// Disable digital input on all bits (64-pin version of ATmega1281 does not even have these inputs)

// Start the ADC Conversion (start first sample, runs in 'free run' mode after)
// bit 6 ADCSRA (ADC Start Conversion) = 1 (START)
// Read ADSCSR and OR with this value to set the flag without changing others
// ADCSRA |= 0b01000000;	// start ADC conversion
// 
// sei();					// Enable interrupts at global level set Global Interrupt Enable (I) bit
	
}

// read adc value
uint16_t adc_read(uint8_t ch)
{
	// select the corresponding channel 0~7
	// ANDing with '7' will always keep the value
	// of 'ch' between 0 and 7
	ch &= 0b00000000;  // AND operation with 7
	ADMUX = (ADMUX & 0xF8)|ch;     // clears the bottom 3 bits before ORing

	// start single conversion
	// write '1' to ADSC
	ADCSRA |= (1<<ADSC);

	// wait for conversion to complete
	// ADSC becomes '0' again
	// till then, run loop continuously
	while(ADCSRA & (1<<ADSC));

	return (ADC);
}
// 
// ISR(ADC_vect)	// ADC Interrupt Handler
// {
// 	ADC0_Value &= 0;
// 	ADC0_Value |= ADCH;
// 	ADC0_Value <<= 8;
// 	ADC0_Value |= ADCL;
// 	fprintf(USART,"ADC0_Value: %X\n\t", ADC0_Value);
// 	_delay_ms(800);
// }
