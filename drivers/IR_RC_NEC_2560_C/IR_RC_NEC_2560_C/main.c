#include <avr/io.h>
#include <stdlib.h>

#include "driver/ir_remote_nec.h"
#include "driver/ir_nec_commands.h"
#include "driver/uart.h"

#define LED_RED_PIN PD7
#define LED_RED_SET (PORTD |= (1 << LED_RED_PIN))
#define LED_RED_CLR (PORTD &= ~(1 << LED_RED_PIN))

#define LED_YELLOW_PIN PD6
#define LED_YELLOW_SET (PORTD |= (1 << LED_YELLOW_PIN))
#define LED_YELLOW_CLR (PORTD &= ~(1 << LED_YELLOW_PIN))

#define LED_BLUE_PIN PD5
#define LED_BLUE_SET (PORTD |= (1 << LED_BLUE_PIN))
#define LED_BLUE_CLR (PORTD &= ~(1 << LED_BLUE_PIN))

int main(void) {
	uint32_t current_command = 0;
	uint32_t last_command = 0;
    
	USART0_SETUP_9600_BAUD_ASSUME_1MHz_CLOCK();

// 	PORTD = 0xFF;
// 	DDRD |= (1 << IR_PIN) | (1 << LED_RED_PIN) | (1 << LED_YELLOW_PIN) | (1 << LED_BLUE_PIN);
	ir_init();
	
	fprintf(USART,"Program start!\n");
	while (1) {
		current_command = get_current_command();

		if (current_command == last_command)
		continue;

		switch (current_command) {
			case COMMAND_CH_MINUS:
			fprintf(USART,"COMMAND_CH_MINUS\n");
/*			LED_RED_CLR;*/
			break;

			case COMMAND_CH_PLUS:
			fprintf(USART,"COMMAND_CH_PLUS\n");
/*			LED_RED_SET;*/
			break;

			case COMMAND_PREV:
			fprintf(USART,"COMMAND_PREV\n");
/*			LED_YELLOW_CLR;*/
			break;

			case COMMAND_NEXT:
			fprintf(USART,"COMMAND_NEXT\n");
/*			LED_YELLOW_SET;*/
			break;

			case COMMAND_VOL_MINUS:
/*			LED_BLUE_CLR;*/
			break;

			case COMMAND_VOL_PLUS:
/*			LED_BLUE_SET;*/
			break;

			default:
			break;
		}

		last_command = current_command;
	}
}
