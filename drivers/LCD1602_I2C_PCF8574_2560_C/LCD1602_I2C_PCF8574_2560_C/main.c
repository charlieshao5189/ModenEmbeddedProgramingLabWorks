//*****************************************
// Program		LCD1602_I2C_PCF8574_2560_C.c
// Author		SHAO
// Date			19th October 2016
// Function		LCD1602(HD44780U+PCF8574T) Display
//	Input / Output
//					PD0-SCL,PD1-SDA
//  Fuse: CKDIV8:unchosen, SUT_CKSEL:Ext.OSC, 65ms.
//*****************************************


#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "driver/lcdpcf8574.h"
#include "driver/uart.h"
#include "driver/pcf8574.h"

int main(void)
{
	USART0_SETUP_9600_BAUD_ASSUME_1MHz_CLOCK();
	USART0_TX_String("program start!!!\n");

	//init lcd
	lcd_init(LCD_DISP_ON);
    USART0_TX_String("LCD_DISP_ON_BLINK!!!\n");
	//lcd go home
	lcd_home();

	uint8_t led = 0;
	while(1) {
		lcd_led(led); //set led
		led = !led; //invert led for next loop
		//test loop
		int i = 0;
		int line = 0;
		lcd_gotoxy(0, 0);
		lcd_puts("love you");
		lcd_gotoxy(0,1);
		lcd_puts("Ingvil! ");
		while(1);
		for(i=0; i<10; i++) {
			char buf[10];
			itoa(i, buf, 10);
			lcd_gotoxy(0, line);
			lcd_puts("i= ");
			lcd_gotoxy(3, line);
			lcd_puts(buf);
			line++;
			line %= 2;
			_delay_ms(500);
		}
	}
}


