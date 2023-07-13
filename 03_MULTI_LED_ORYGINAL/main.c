/*
 * main.c
 *
 *  Created on: 2010-03-30
 *       Autor: Miros�aw Karda�
 */
#include <avr/io.h>		// do��czenie g��wnego systemowego  pliku nag��wkowego
#include <avr/interrupt.h>
#include <util/delay.h>



#include "d_led.h"		// do��czenie naszego pliku nag��wkowego (obs�uga LED)

int main(void)
{
	// ****** inicjalizacja *********
	d_led_init();   		// inicjalizacja wy�wietlacza multipleksowanego

	// testowa inicjalizacja zmiennych oraz liczb maj�cych si� wy�wietla� na wyrw. LED
	cy1=NIC;
	cy2=4;
	cy3=1;
	cy4=NIC;

	sei();			// w��czenie globalnego zezwolenia na przerwania


		while(1)
		;
}

