/*
 * main.c
 *
 *  Created on: 2010-03-30
 *       Autor: Miros³aw Kardaœ
 */
#include <avr/io.h>		// do³¹czenie g³ównego systemowego  pliku nag³ówkowego
#include <avr/interrupt.h>
#include <util/delay.h>



#include "d_led.h"		// do³¹czenie naszego pliku nag³ówkowego (obs³uga LED)

int main(void)
{
	// ****** inicjalizacja *********
	d_led_init();   		// inicjalizacja wyœwietlacza multipleksowanego

	// testowa inicjalizacja zmiennych oraz liczb maj¹cych siê wyœwietlaæ na wyrw. LED
	cy1=NIC;
	cy2=4;
	cy3=1;
	cy4=NIC;

	sei();			// w³¹czenie globalnego zezwolenia na przerwania


		while(1)
		;
}

