/*
 * main.c
 * 
 * Copyright 2013 Shimon <shimon@monistit.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
#define  F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "driver/spi.h"
#include "driver/mfrc522.h"
#include "driver/uart.h"

/*self test*/
//uint8_t SelfTestBuffer[64];
// int main()
// {
// 	uint8_t byte;
// //	_delay_ms(50);
// // 	LCDInit(LS_BLINK);
// // 	LCDWriteStringXY(2,0,"RFID Reader");
// // 	LCDWriteStringXY(5,1,VERSION_STR);
// 	
// 	USART0_SETUP_9600_BAUD_ASSUME_1MHz_CLOCK();
// 	fprintf(USART,"mfrc522 program start!\n");
// 	spi_init();
// 	_delay_ms(1000);
// // 	LCDClear();
// // 	LCDWriteStringXY(0,0,"FIFO Data : 0x");
// 	
// 	mfrc522_reset();
// 	for(byte = 0 ; byte<25; byte ++)
// 		mfrc522_write(FIFODataReg,0x00);
// 	mfrc522_write(CommandReg,Mem_CMD);
// 	mfrc522_write(AutoTestReg,0x09);
// 	mfrc522_write(FIFODataReg,0x00);
// 	mfrc522_write(CommandReg,CalcCRC_CMD);
// 	fprintf(USART,"initial finish!\n");
// 	_delay_ms(500);
// 	for(byte = 0; byte<	64; byte++)
// 	{
// 		SelfTestBuffer[byte] = mfrc522_read(FIFODataReg);
// 		//LCDHexDumpXY(14,0,SelfTestBuffer[byte]);
// 		fprintf(USART,"SelfTestBuffer[%d]:%X\n",byte,SelfTestBuffer[byte]);
// 		_delay_ms(150);
// 	}	
// 	
// 	while(1){} 
// }
/*card detection*/
// int main()
// {
// 	uint8_t byte;
// 	uint8_t str[MAX_LEN];
// /*	_delay_ms(50);*/
// // 	LCDInit(LS_BLINK);
// // 	LCDWriteStringXY(2,0,"RFID Reader");
// // 	LCDWriteStringXY(5,1,VERSION_STR);
// 	
// 	USART0_SETUP_9600_BAUD_ASSUME_1MHz_CLOCK();
// 	spi_init();
// // 	_delay_ms(1000);
// // 	LCDClear();
// 	
// 	//init reader
// 	mfrc522_init();
// 	
// 	//check version of the reader
// 	byte = mfrc522_read(VersionReg);
// 	if(byte == 0x92)
// 	{
// 		fprintf(USART,"MIFARE RC522v2 Detected!\n");
// // 		LCDWriteStringXY(2,0,"MIFARE RC522v2");
// // 		LCDWriteStringXY(4,1,"Detected");
// 	}else if(byte == 0x91 || byte==0x90)
// 	{
// 		fprintf(USART,"MIFARE RC522v2 Detected!\n");
// // 		LCDWriteStringXY(2,0,"MIFARE RC522v1");
// // 		LCDWriteStringXY(4,1,"Detected");
// 	}else
// 	{
// 		fprintf(USART,"No reader found!\n");
// /*		LCDWriteStringXY(0,0,"No reader found");*/
// 	}
// 	
// 	byte = mfrc522_read(ComIEnReg);
// 	mfrc522_write(ComIEnReg,byte|0x20);
// 	byte = mfrc522_read(DivIEnReg);
// 	mfrc522_write(DivIEnReg,byte|0x80);
// 	
// 	_delay_ms(1500);
// /*	LCDClear();*/
// 	
// 	while(1){
// 		byte = mfrc522_request(PICC_REQALL,str);
// /*		LCDHexDumpXY(0,0,byte);*/
//         fprintf(USART,"byte:%x\n",byte);
// 		_delay_ms(1000);
// 	}
// }
/*card serial reading*/
uint8_t SelfTestBuffer[64];
int main()
{
	uint8_t byte;
	uint8_t str[MAX_LEN];
	
	USART0_SETUP_9600_BAUD_ASSUME_1MHz_CLOCK();
	spi_init();
	
	fprintf(USART,"program start!\n");
	//init reader
	mfrc522_init();
	
	//check version of the reader
	byte = mfrc522_read(VersionReg);
	if(byte == 0x92)
	{
		fprintf(USART,"MIFARE RC522v2 Detected!\n");
// 		LCDWriteStringXY(2,0,"MIFARE RC522v2");
// 		LCDWriteStringXY(4,1,"Detected");
	}else if(byte == 0x91 || byte==0x90)
	{
		fprintf(USART,"MIFARE RC522v2 Detected!\n");
// 		LCDWriteStringXY(2,0,"MIFARE RC522v1");
// 		LCDWriteStringXY(4,1,"Detected");
	}else
	{
		fprintf(USART,"No reader found!\n");
/*		LCDWriteStringXY(0,0,"No reader found");*/
	}

	byte = mfrc522_read(ComIEnReg);
	mfrc522_write(ComIEnReg,byte|0x20);
	byte = mfrc522_read(DivIEnReg);
	mfrc522_write(DivIEnReg,byte|0x80);
	
	while(1){
		byte = mfrc522_request(PICC_REQALL,str);
		
		if(byte == CARD_FOUND)
		{
			byte = mfrc522_get_card_serial(str);
			if(byte == CARD_FOUND)
			{
				for(byte=0;byte<8;byte++)
				fprintf(USART,"serial number[%d]: %x\n",byte,str[byte]);
				//LCDHexDumpXY(byte*2,0,str[byte]);
				
				_delay_ms(2500);
			}
			else
			{
				fprintf(USART,"error");
				//LCDWriteStringXY(0,1,"Error");
			}
		}
		
		_delay_ms(1000);
	}
}