/*
 * main.c
 *
 *  Created on: 2010-03-30
 *       Autor: Miros�aw Karda�
 */
#include <avr/io.h>		// do��czenie g��wnego systemowego  pliku nag��wkowego
#include <avr/interrupt.h>
#include <util/delay.h>
#include "d_led.h"

int main(void)
{
	// ****** inicjalizacja *********
	d_led_init();   		// inicjalizacja wy�wietlacza multipleksowanego

	cy1=2;
	cy2=3;
	cy3=4;
	cy4=5;


	sei();			// w��czenie globalnego zezwolenia na przerwania



		//**********************  p�tla g��wna
		while(1)
		{


		}
}



