/*
 * main.c
 *
 *  Created on: 2010-03-30
 *       Autor: Miros³aw Kardaœ
 */
#include <avr/io.h>		// do³¹czenie g³ównego systemowego  pliku nag³ówkowego
#include <avr/interrupt.h>
#include <util/delay.h>
#include "d_led.h"

int main(void)
{
	// ****** inicjalizacja *********
	d_led_init();   		// inicjalizacja wyœwietlacza multipleksowanego

	cy1=2;
	cy2=3;
	cy3=4;
	cy4=5;


	sei();			// w³¹czenie globalnego zezwolenia na przerwania



		//**********************  pêtla g³ówna
		while(1)
		{


		}
}



