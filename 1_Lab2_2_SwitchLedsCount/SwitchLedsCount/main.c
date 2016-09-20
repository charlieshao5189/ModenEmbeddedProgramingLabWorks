/*
 * SwitchLedsCount.c
 *
 * Created: 9/16/2016 1:16:21 PM
 * Author : charlieshao5189
 * Function: count action times of switch, display count number with 4 leds
 * AVR Fuses Configuration:
 * 
 */ 

#include <avr/io.h>
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
		if(PIND != PortD_S) 
		{
			countNumber++; 	
			PORTB = countNumber;		
		}
    }
}

