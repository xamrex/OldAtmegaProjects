/*
 * main.c
 *
 *  Created on: 2011-03-10
 *       Autor: Miros�aw Karda�
 */
#include <avr/io.h>
volatile uint8_t wynik;

void funkcja(uint8_t port);
void funkcja2 (void);

int main(void){

	wynik=1;
	funkcja2();

}


void funkcja(uint8_t port){
	DDRB=0xff;
	PORTB = port;

}
