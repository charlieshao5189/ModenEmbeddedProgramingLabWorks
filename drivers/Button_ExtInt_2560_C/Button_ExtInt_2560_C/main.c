/*
 * Button_ExtInt_2560_C.c
 *
 * Created: 11/4/2016 8:42:33 AM
 * Author : charlie
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile unsigned char button_flag;//gloabl variable must be volatile，it may be changed anytime at subroutine

int main(void)
{
	DDRK |= (1<<DDK7);//set Portk pin7 to digital output
	PORTK &= ~(1<<PK7);//turn off led
	//PORTK |= (1<<PK7);//turn on led
	
	DDRD &= ~(1<<DDD2);//set PORTD pin2 to digital input
	PORTD |= (1<<PD2);//pull up resister enable
	EICRA |= (1<<ISC20);//Any logical change on INTn generates an interrupt request
	EIMSK |= (1<<INT2);//enable external interrupt 2
	EIFR |= (1<<INTF2);//clear external interrupt 2 flag
    sei();
	
    /* Replace with your application code */
    while(1) 
    {
		if(!button_flag){
		//button_flag=0;
		PORTK |= (1<<PK7);//turn on led
 		_delay_ms(500);
 		PORTK &= ~(1<<PK7);//turn off led
 		_delay_ms(599);
		}
    }
}

ISR(INT2_vect)
{
	button_flag = ~button_flag;
}

