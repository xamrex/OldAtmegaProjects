/*
** main.c
**
** Nokia 3310 Example Program
** Target: ATMEGA :: AVR-GCC
**
** Written by Tony Myatt - 2007
** Quantum Torque - www.quantumtorque.com
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include "lcd.h"

/* main -- Program starts here */
int main(void)
{
	// Setup LCD
	lcd_init();
	obrazek1();


	while(1){

	}
}
