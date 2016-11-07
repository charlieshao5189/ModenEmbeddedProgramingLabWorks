#include "button.h"
#include <avr/interrupt.h>
#include <avr/io.h>

#define  BTN_PORT_DDR DDRD
#define  BTN_PORT PIND
#define  BTN_PORT_MASK 0x03
#define  BTN_0    0x01
#define  BTN_1    0x02



unsigned char Trg; //button trigger sign
unsigned char Cont;//button continue sign

void tiemr1_10ms_debouce_configure()
{
		TCCR1A = 0b00000000;	// Normal port operation (OC1A, OC1B, OC1C), Clear Timer on 'Compare Match' (CTC) waveform mode)
		TCCR1B = 0b00001010;	// CTC waveform mode, use prescaler 8
		TCCR1C = 0b00000000;
		
		// For F_CPU Mhz cup clock to achieve a 10 millisecond(100MHz) interval:
		// Need to count F_CPU/100 clock cycles (but already divided by 8)
		// So actually need to count to (F_CPU/100 / 8-1) = 16000000/100/8 -1 = 19999 decimal, = 4E1F Hex
		OCR1AH = 0x4E; // Output Compare Registers (16 bit) OCR1BH and OCR1BL
		OCR1AL = 0x1F;

		TCNT1H = 0b00000000;	// Timer/Counter count/value registers (16 bit) TCNT1H and TCNT1L
		TCNT1L = 0b00000000;
		TIMSK1 = 0b00000010;	// bit 1 OCIE1A		Use 'Output Compare A Match' Interrupt, i.e. generate an interrupt
		// when the timer reaches the set value (in the OCR1A register)
}

void key_configure()
{
	BTN_PORT_DDR= 0x00;//set buttons connecting port direction INPUT(connect PIND0 with switch)
	sei();	   // Enable interrupts at global level, set Global Interrupt Enable (I) bit
	tiemr1_10ms_debouce_configure();
}

ISR(TIMER1_COMPA_vect) // TIMER1_CompareA_Handler (Interrupt Handler for Timer 1)
{	// Flip the value of the least significant bit of the 8-bit variable
    unsigned char ReadData;

    unsigned char Port_value_present;

    static unsigned char  Port_value_last=0;

    //关中断等//

    Port_value_present= BTN_PORT_MASK & BTN_PORT;                                 //端口当前值

    if(!(Port_value_present^Port_value_last))

    {

	    ReadData=Port_value_present^BTN_PORT_MASK;

	    Trg=ReadData&(ReadData^Cont);

	    Cont=ReadData;

    }

    Port_value_last= BTN_PORT_MASK & BTN_PORT;                                       //端口上一次的值

    //开中断等//
}