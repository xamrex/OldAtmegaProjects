/*
 * main.c
 *
 *  Created on: 2010-09-03
 *       Autor: Miros³aw Kardaœ
 *
 *       ATmega32 - taktowana wewn. oscylatorem 8MHz
 *
 *		 Kalibracja OSCCAL
 *
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>



#include "MKUART/mkuart.h"

char wynik;

int main(void) {
DDRC|=(1<<PC0); //PC0 jako wyjscie
PORTC |=(1<<PC0); //dioda wylaczona
	USART_Init( __UBRR );			// inicjalizacja UART

	sei();							// globalne odblokowanie przerwañ



	// pêtla nieskoñczona
	while(1) {


		uart_putc('a');
_delay_us(500);
	}

}









