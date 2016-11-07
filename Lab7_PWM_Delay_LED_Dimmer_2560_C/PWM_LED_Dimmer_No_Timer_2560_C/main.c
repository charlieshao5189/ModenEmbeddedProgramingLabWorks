/*
*
* Created: 9/16/2016 1:16:21 PM
* Author : SHAO
* Function: LED dimmer control, using delay implement PWM signal. PortB0-7 connectLED  PD0 increase LED light, PD1 decrease LED light, Light level:0-10
*/

#include <avr/io.h>
#include "drivers/button.h"
# define F_CPU 1000000UL
#include <util/delay.h>




void InitialiseGeneral();
void LED_Brightness_Duty(unsigned char duty);

void InitialiseGeneral()
{
	//Congigure Ports
	DDRB = 0xFF;//Set port B direction to OUTPUT,connected to the leds
	PORTB = 0x00;//Set all leds initially off
	

}
void delay_ms(uint16_t count) {
	while(count--) {
		_delay_ms(1);

	}
}
//period= 10ms
void LED_Brightness_Duty(unsigned char duty)//duty range:0-10
{
	PORTB = 0xFF; //LED ON
	delay_ms(duty);//
	PORTB = 0x81; //LED oFF
	delay_ms(10-duty);
}

extern unsigned char Trg; //button trigger sign
extern unsigned char Cont;//button continue sign
 void key_deal()
 {
    static unsigned char countNumber = 0;//count action times of switch
	if(0 != Trg)
	{
		//if((Cont&BTN_0)&(Trg^BTN_0))   //long-time push button
		if(Trg&BTN_0)      //first time push button
		{
			if(0 == countNumber)
			{
				countNumber = 11;
			}
			countNumber--;
		}
		else if(Trg&BTN_1)
		{
			countNumber++;
			if(10 <= countNumber)
			{
				countNumber = 0;
			}
		}
	}
	LED_Brightness_Duty(countNumber);
 }

int main(void)
{
	//unsigned char PortD_S = 0x03; // store PORTD former status
	InitialiseGeneral();
	key_configure();
	while (1)
	{
	key_deal();
	}

}

