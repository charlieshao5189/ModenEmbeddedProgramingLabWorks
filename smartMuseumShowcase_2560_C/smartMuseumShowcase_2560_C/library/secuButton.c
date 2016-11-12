/*
* secuButton.c
*
* Created: 11/10/2016 1:50:09 PM
*  Author: charlie
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "lcd1602/lcdpcf8574.h"

#include "tricolorled.h"

void alarm_ON(){
	PORTA|= (1<<DDA1);//active alarm
	lcd_gotoxy(13,1);
	lcd_puts("ON ");
}

void alarm_OFF(){
	PORTA&= ~(1<<DDA1);//inactive alarm
		lcd_gotoxy(13,1);
		lcd_puts("OFF");
}

void secuButtonInit(){
	DDRA |= (1<<DDA1);//set portA pin0 to digital output, used as alarm control signal, set 1 will active alarm, set 0 inactive alarm
	alarm_OFF();//turn off alarm
	
	DDRD &= ~(1<<DDD2);//set PORTD pin2 to digital input, as security button signal input
	PORTD |= (1<<PD2);//pull up resister enable
	EICRA |= (1<<ISC21);//falling edge on INTn generates an interrupt request
	EIMSK |= (1<<INT2);//enable external interrupt 2
	EIFR |= (1<<INTF2);//clear external interrupt 2 flag
	sei();
}


ISR(INT2_vect)
{
	static unsigned char flag=0;
	if(0==flag){
		flag=1;
		tricolorled_onoff(LED_RED,LED_ON);
		alarm_ON();
		//fprintf(USART,"Allarm On!");
		
	}
	else
	{
		flag=0;
		tricolorled_onoff(LED_RED,LED_OFF);
		alarm_OFF();
		//fprintf(USART,"Allarm OFF!");
	}
}

