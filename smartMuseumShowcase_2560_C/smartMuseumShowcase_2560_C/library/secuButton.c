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
#include "secuButton.h"
#include "tricolorled.h"

void alarm_ON(){
	PORTA|= (1<<DDA1);//active alarm
	alarmFlag=1;
	tricolorled_onoff(LED_GREEN,LED_OFF);
	lcd_gotoxy(13,1);
	lcd_puts("ON");
}

void alarm_OFF(){
	PORTA&= ~(1<<DDA1);//inactive alarm
	alarmFlag=0;
	tricolorled_onoff(LED_RED,LED_OFF);
}

void securityInit(){
	DDRA |= (1<<DDA1);//set portA pin0 to digital output, used as alarm control signal, set 1 will active alarm, set 0 inactive alarm
	PORTA&= ~(1<<DDA1);//turn off buzzer
	
	DDRD &= ~(1<<DDD2);//set PORTD pin2 to digital input, as security button signal input
	PORTD |= (1<<PD2);//pull up resister enable
	EICRA |= (1<<ISC21);//falling edge on INTn generates an interrupt request
	EIMSK |= (1<<INT2);//enable external interrupt 2
	EIFR |= (1<<INTF2);//clear external interrupt 2 flag
	sei();
    securityEnableFlag=1; //enable security alarm (buzzer and red state led)
	lcd_gotoxy(13,1);
	lcd_puts("EN ");
}


ISR(INT2_vect)
{
	if(1 == securityEnableFlag){
		alarm_ON();
		//fprintf(USART,"Allarm On!");
	}
	else
	{
		alarm_OFF();
		//fprintf(USART,"Allarm OFF!");
	}
}

