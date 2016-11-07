//*****************************************
// Program		IR_RC_1838B_2560_C.c
// Author		SHAO
// Date			10/20/2016 4:50:25 PM
// Function		IR Remote control
//	Input / Output
//					PD0-SCL,PD1-SDA
//  Fuse: CKDIV8:unchosen, SUT_CKSEL:Ext.OSC, 65ms.
//*****************************************

#include <avr/io.h>
#define F_CPU 16000000UL
#include "driver/uart.h"

int main(void)
{
  USART0_SETUP_9600_BAUD_ASSUME_1MHz_CLOCK();
  fprintf(USART,"hello world!");
}