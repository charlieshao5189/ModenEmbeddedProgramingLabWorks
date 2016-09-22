/*
 * SwitchLedsCount.c
 *
 * Created: 9/16/2016 1:16:21 PM
 * Author : shao
 * Function: count action times of switch, display count number with 4 leds
 */ 

#include <avr/io.h>

# define F_CPU 1000000UL
#include <util/delay.h>



void InitialiseGeneral();
unsigned char countNumber = 0;//count action times of switch

void InitialiseGeneral()
{
	//Congigure Ports
	DDRB = 0xFF;//Set port B direction to OUTPUT,connected to the leds
	PORTB = 0x00;//Set all leds initially off
	
	DDRD= 0x00;//set port D direction INPUT(connect PIND0 with switch)
}

int main(void)
{
	unsigned char PortD_S = 0x00; // store PORTD former status 
    InitialiseGeneral();
    while (1) 
    {
		PortD_S = PIND;    //read PIND and store its status to PortD_S
		_delay_ms(100);   //   1/10th second debouncer
		if(PIND != PortD_S){
			if(0 == PIND0){         // count increase button enbale
				countNumber++;
				if(10 == countNumber){
					countNumber = 0;
				}
				else if (5 == countNumber)
				{
					PORTB = 0xFF;
					_delay_ms(250);//Keep leds on for 1 second(250ms*4)
					_delay_ms(250);
					_delay_ms(250);
					_delay_ms(250);
					PORTB = 0x00;
				}
			}
			if(0 == PIND1){
				countNumber = 0x00;// reset switch enable
			}
			PORTB = countNumber;
		}
    }
}

