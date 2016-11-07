//*****************************************
// Program		PWM_LED_Dimmer_2560_C.c
// Author		SHAO
// Date			12nd October 2016

// Function		Uses 16-bit Counter/Timer 5
//				This example uses the PWM output by using both interrupt handlers to signal the pulse changeover points
//				and thus DOES NOT REQUIRE A SIGNAL ON AN OUTPUT PIN - for this reason Timer 5 is used, which does not
//				have on OC pin on the 64-pin package of the ATmega1281.

//				The PWM signal is interpreted in software using two interrupt handlers:
//					ISR(TIMER5_OVF_vect), the TIMER5_Overflow_Handler (signals Start of new cycle), turns LEDS on
//					ISR(TIMER5_COMPA_vect) the TIMER5_CompareA_Handler (signals the cutoff for the high part of the cycle), turn LEDS off
//				The Pulse-Width-Modulated signal will not appear on any pin directly (Timer 5 does not have any OC pins)

//	Input / Output
//					Switched connected to PORT D to select PWM configuration, PD0 button:increase duty, PD1 button:decrease duty.
//					LEDs connected to Port B to show effect of PWM (changes LED brightness)
//  Fuse: CKDIV8:chosen, SUT_CKSEL:Int.OSC, 65ms. 
//*****************************************
#include <avr/io.h>
#include <avr/interrupt.h>
#include "drivers/button.h"
#include "drivers/uart.h"




void InitialiseGeneral();
void InitialiseTimer5_PWM_ChannelA();
void key_deal();
unsigned char SwitchesValue = 1;//count action times of switch

int main( void )
{

	InitialiseTimer5_PWM_ChannelA();
	InitialiseGeneral(); // Port B must be set for output AFTER the timer setup of OC0
	USART0_SETUP_9600_BAUD_ASSUME_1MHz_CLOCK();
	key_configure();
	USART0_TX_String("program start!!!\n");
	while(1)
	{
		key_deal();
	}
}

void InitialiseGeneral()
{
	DDRB = 0xFF;			// Set Port B as output (LEDs)
	PORTB = 0x00;			// LEDs off initially
}

void InitialiseTimer5_PWM_ChannelA()
{
	// TCCR5A ?Timer/Counter 5 Control Register A
	// Bit 7:6 ?COMnA1:0: Compare Output Mode for Channel A
	// Bit 5:4 ?COMnB1:0: Compare Output Mode for Channel B
	// Bit 3:2 ?COMnC1:0: Compare Output Mode for Channel C
	// Bit 1:0 ?WGMn1:0: Waveform Generation Mode (0101 Fast PWM, 8-bit)
	TCCR5A = 0b00000001;	// No output pins in use, set all to normal mode, waveform  = Fast PWM, 8-bit
	
	// TCCR5B ?Timer/Counter 5 Control Register B
	// Bit 7 ?ICNCn: Input Capture Noise Canceler
	// Bit 6 ?ICESn: Input Capture Edge Select
	// Bit 5 ?Reserved Bit
	// Bit 4:3 ?WGMn3:2: Waveform Generation Mode (0101 Fast PWM, 8-bit)
	// Bit 2:0 ?CSn2:0: Clock Select (010 = 8 prescaler)
	TCCR5B = 0b00001010; // waveform  = Fast PWM, 8-bit, 8 prescaler
	
	// TCCR5C ?Timer/Counter 5 Control Register C
	// Bit 7 ?FOCnA: Force Output Compare for Channel A
	// Bit 6 ?FOCnB: Force Output Compare for Channel B
	// Bit 5 ?FOCnC: Force Output Compare for Channel C
	TCCR5C = 0b00000000;
	
	// TCNT5H and TCNT5L –Timer/Counter 5
	TCNT5H = 0;
	TCNT5L = 0;
	
	// OCR5AH and OCR5AL ?Output Compare Register 5 A
	OCR5AH = 0x00;
	OCR5AL = 0x26;

	// TIMSK5 ?Timer/Counter 5 Interrupt Mask Register
	// Bit 5 ?ICIEn: Timer/Countern, Input Capture Interrupt Enable
	// Bit 3 ?OCIEnC: Timer/Countern, Output Compare C Match Interrupt Enable
	// Bit 2 ?OCIEnB: Timer/Countern, Output Compare B Match Interrupt Enable
	// Bit 1 ?OCIEnA: Timer/Countern, Output Compare A Match Interrupt Enable
	// Bit 0 ?TOIEn: Timer/Countern, Overflow Interrupt Enable
	TIMSK5 = 0b00000011; // Enable OCIE5A and TOIE5 interrupts

	// TIFR5 ?Timer/Counter5 Interrupt Flag Register
	TIFR5 = 0b00101111;		// Clear all flags
}

ISR(TIMER5_OVF_vect) // TIMER5_Overflow_Handler
{	// Here at Start of new cycle
	PORTB = 0XFF; //LEDS on
}

ISR(TIMER5_COMPA_vect) // TIMER5_CompareA_Handler
{	// Here when reach the cutoff for the high part of the cycle
	PORTB = 0x00; //LEDS off
}

void key_deal()
{

	
	//if((0 != Trg)||(0 !=Cont))
	if(0 != Trg)
	{
		fprintf(USART,"Trg: %x\n",Trg);
		switch(Trg)
		{
			case Switch_0_Pressed:
			if( SwitchesValue <= 1)
			{
				SwitchesValue = 11;
			}
			SwitchesValue--;
			fprintf(USART,"SwitchesValue-: %d\n",SwitchesValue);
			break;
			case Switch_1_Pressed:
			SwitchesValue++;
			if(SwitchesValue >= 11)
			{
				SwitchesValue = 1;
			};
			fprintf(USART,"SwitchesValue+: %d\n",SwitchesValue);
			break;
		}
		
		switch(SwitchesValue)
		{
			case 1:
				OCR5AL = 26;			// 10% duty cycle
				break;
			case 2:
				OCR5AL = 51;			// 20% duty cycle
				break;
			case 3:
				OCR5AL = 77;		// 30% duty cycle
				break;
			case 4:
				OCR5AL = 102;		// 40% duty cycle
				break;
			case 5:
				OCR5AL = 128;		// 50% duty cycle
				break;
			case 6:
				OCR5AL = 153;		// 60% duty cycle
				break;
			case 7:
				OCR5AL = 179;		// 70% duty cycle
				break;
			case 8:
				OCR5AL = 204;		// 80% duty cycle (LEDS appear near-full brightness)
				break;
			case 9:
			    OCR5AL = 230;		 // 90% duty cycle (LEDS appear near-full brightness)
			    break;
			case 10:
			    OCR5AL = 255;		 // 100% duty cycle (LEDS appear near-full brightness)
			    break;

		}
	}
}