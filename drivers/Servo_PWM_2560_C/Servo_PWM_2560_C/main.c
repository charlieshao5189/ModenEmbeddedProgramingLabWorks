/*
 * Servo_PWM_2560_C.c
 *
 * Created: 11/4/2016 12:19:19 PM
 * Author : charlie
 */ 

#include <avr/io.h>
#include "driver/servo.h"
#define F_CPU 16000000UL
#include <util/delay.h>
int main(void)
{   
	InitialiseTimer3_FastPWM_Single();
    /* Replace with your application code */
    while (1) 
    {
		SetServoPosition(LockerOFF);
		_delay_ms(1000);
		SetServoPosition(LockerOn);
		_delay_ms(1000);
    }
}

