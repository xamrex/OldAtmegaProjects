/*
 * main.c
 *
 *  Created on: 2010-09-13
 *       Autor: Miros�aw Karda�
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "LCD/lcd44780.h"

#include "1Wire/ds18x20.h"

void display_temp(uint8_t x);

uint8_t czujniki_cnt;		/* ilo�� czujnik�w na magistrali */
volatile uint8_t s1_flag;	/* flaga tykni�cia timera co 1 sekund� */
volatile uint8_t sekundy;	/* licznik sekund 0-59 */

uint8_t subzero, cel, cel_fract_bits;

int main(void) {

	DDRA |= (1<<PA7);
	PORTA |= (1<<PA7); /* pod�wietlenie wy�wietlacza LCD */

	/* ustawienie TIMER0 dla F_CPU=11,0592MHz */
	TCCR0 |= (1<<WGM01);				/* tryb CTC */
	TCCR0 |= (1<<CS02)|(1<<CS00);		/* preskaler = 1024 */
	OCR0 = 108;							/* dodatkowy podzia� przez 108 (rej. przepe�nienia) */
	TIMSK |= (1<<OCIE0);				/* zezwolenie na przerwanie CompareMatch */
	/* przerwanie wykonywane z cz�stotliwo�ci� ok 10ms (100 razy na sekund�) */
	/* do naszych cel�w nie musi by� to bardzo dok�adne 10ms */

	lcd_init();	/* inicjalizacja LCD */

	/* sprawdzamy ile czujnik�w DS18xxx widocznych jest na magistrali */



	/* p�tla niesko�czona */
	while(1) {
		czujniki_cnt = search_sensors();
lcd_locate(0,0);
		lcd_int(czujniki_cnt);
}
}
