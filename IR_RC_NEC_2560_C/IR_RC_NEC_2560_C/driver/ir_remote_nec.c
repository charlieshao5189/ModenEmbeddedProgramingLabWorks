#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include "ir_remote_nec.h"
#include <util/delay.h>

#include "uart.h"

#define LEDPIN PINB7
#define LED_ON()   PORTB |= (1<<LEDPIN) 
#define LED_OFF()  PORTB &=~(1<<LEDPIN)

void ir_init() {
// 	//unsigned char temp;
// 	TCCR2A |= (1 << WGM21);
// 	TCCR2B |= (1 << CS20); 		/* prescaler = 1 */
// 	//temp=((F_CPU/ 2000000) * SAMPLE_RATIO)  - 1;
// 	OCR2B = ((F_CPU/ 2000000) * SAMPLE_RATIO)  - 1;
// 	//fprintf(USART,"temp:%d",temp);
//   	TIMSK2 |= (1 << OCIE2B); //compare match B interrupt enable
//      fprintf(USART,"timer initial finish!\n");
    PORTD = 0xFF;
    DDRD &= ~(1 << IR_PIN);      //set IR_PIN to input
	EIMSK |= (1 << INT3); 		/* External INT3 enable */
	EICRA |= (1 << ISC31); 		/* Falling edge detection */

		TCCR1A = 0b00000010;	// Normal port operation (OC1A, OC1B, OC1C), Clear Timer on 'Compare Match' (CTC) waveform mode)
		TCCR1B = 0b00001001;	// CTC waveform mode, use prescaler 8
		TCCR1C = 0b00000000;
		
		// For F_CPU Mhz cup clock to achieve a 10 millisecond(100MHz) interval:
		// Need to count F_CPU/100 clock cycles (but already divided by 8)
		// So actually need to count to (F_CPU/100 / 8-1) = 16000000/100/8 -1 = 19999 decimal, = 4E1F Hex
		OCR1AH = 0x00; // Output Compare Registers (16 bit) OCR1BH and OCR1BL
		OCR1AL = ((F_CPU/ 2000000) * SAMPLE_RATIO)  - 1;

		TCNT1H = 0b00000000;	// Timer/Counter count/value registers (16 bit) TCNT1H and TCNT1L
		TCNT1L = 0b00000000;
		TIMSK1 = 0b00000010;	// bit 1 OCIE1A		Use 'Output Compare A Match' Interrupt, i.e. generate an interrupt

    DDRB |= 1 << DDB7;//set pin to output
    LED_OFF();
	
	sei();					// Enable interrupts at global level, set Global Interrupt Enable (I) bit

	ir_reset();
	total_cycles_counter = 0;
}

void ir_reset() {
	state = STATE_WAIT;
	repeat = 0;
	cycles_counter = 0;
}

uint8_t ir_check_command(uint32_t command) {
	uint8_t address_high = (command >> 24);
	uint8_t address_low = (command >> 16);
	return (address_high & address_low) == 0;
}

void ir_check_timeout() {
	if (total_cycles_counter < TIMEOUT)
		return;

	ir_reset();
	total_cycles_counter = 0;
	check_falling_edge();
}
ISR(TIMER1_COMPA_vect) {
//ISR(TIMER2_COMPB_vect) {
	//TIMSK2 &= ~(1 << OCIE2B);
	cycles_counter++;
	total_cycles_counter++;	
// 	if(cycles_counter==32580){
//        LED_OFF();
// 	}else if (cycles_counter==65535)
// 	{
// 	   LED_ON();
// 	}
	
	
	//TIMSK2 |= (1 << OCIE2B);
}

ISR(INT3_vect) {
	ir_check_timeout();
	EIMSK &= (~(1 << INT3));	/* External INT3 disable */
	//LED_ON();
	switch (state) {
		case STATE_WAIT:
			if (edge == EDGE_FALLING)
				ir_handle_state_wait_edge_falling();
			else
				ir_handle_state_wait_edge_rising();
			break;

		case STATE_START:
			if (edge == EDGE_RISING)
				ir_handle_state_start_edge_rising();
			else if (edge == EDGE_FALLING)
				ir_handle_state_start_edge_falling();
			break;

		case STATE_BITS:
			if (edge == EDGE_RISING)
				ir_handle_state_bits_edge_rising();
			else if (edge == EDGE_FALLING)
				ir_handle_state_bits_edge_falling();
			break;

		case STATE_STOP:
			ir_handle_state_stop();
			break;

		default:
			ir_reset();
			break;
	}

	toggle_edge();
    //LED_OFF();
	EIMSK |= (1 << INT3); 		/* External INT3 enable */
}

void ir_handle_state_wait_edge_falling() {
	cycles_counter = 0;
	state = STATE_START;
}

void ir_handle_state_wait_edge_rising() {
	ir_reset();
}

void ir_handle_state_start_edge_falling() {
	  fprintf(USART,"counter: %d", cycles_counter);
	if(cycles_counter >= TIME_START_HALFBIT_HIGH_COMMAND_MIN && cycles_counter <= TIME_START_HALFBIT_HIGH_COMMAND_MAX) {
		state = STATE_BITS;
		command_bits_counter = 0;
		command = 0;
		return;
	}

	if (cycles_counter >= TIME_START_HALFBIT_HIGH_REPEAT_MIN && cycles_counter <= TIME_START_HALFBIT_HIGH_REPEAT_MAX) {
		state = STATE_STOP;
		repeat = 1;
		command = current_command;
		return;
	}

	ir_reset();
}

void ir_handle_state_start_edge_rising() {
	cycles_counter = 0;
	state = STATE_START;
}

void ir_handle_state_bits_edge_falling() {
	if (cycles_counter >= TIME_COMMAND_HALFBIT_HIGH_ZERO_MIN && cycles_counter <= TIME_COMMAND_HALFBIT_HIGH_ZERO_MAX) {
		command = (command << 1);
		command_bits_counter++;

		if (command_bits_counter < 32) {
			state = STATE_BITS;
		} else if (!ir_check_command(command)) {
			ir_reset();
		} else {
			state = STATE_STOP;
			current_command = command;
		}

		return;
	}

	if (cycles_counter >= TIME_COMMAND_HALFBIT_HIGH_ONE_MIN && cycles_counter <= TIME_COMMAND_HALFBIT_HIGH_ONE_MAX) {
		command = ((command << 1) | 1);
		command_bits_counter++;

		if (command_bits_counter < 32) {
			state = STATE_BITS;
		} else if (!ir_check_command(command)) {
			ir_reset();
		} else {
			state = STATE_STOP;
			current_command = command;
		}

		return;
	}


	command_bits_counter = 0;
	command = 0;
	ir_reset();
}

void ir_handle_state_bits_edge_rising() {
	cycles_counter = 0;
	state = STATE_BITS;
}

void ir_handle_state_stop() {
	cycles_counter = 0;
	state = STATE_WAIT;
	current_command = command;
	total_cycles_counter = 0;
}
