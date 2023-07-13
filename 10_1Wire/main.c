/*
 * main.c
 *
 *  Created on: 2010-09-13
 *       Autor: Miros³aw Kardaœ
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "LCD/lcd44780.h"

#include "1Wire/ds18x20.h"

void display_temp(uint8_t x);

uint8_t czujniki_cnt;		/* iloœæ czujników na magistrali */
volatile uint8_t s1_flag;	/* flaga tykniêcia timera co 1 sekundê */
volatile uint8_t sekundy;	/* licznik sekund 0-59 */

uint8_t subzero, cel, cel_fract_bits;

int main(void) {

	DDRA |= (1<<PA7);
	PORTA |= (1<<PA7); /* podœwietlenie wyœwietlacza LCD */

	/* ustawienie TIMER0 dla F_CPU=11,0592MHz */
	TCCR0 |= (1<<WGM01);				/* tryb CTC */
	TCCR0 |= (1<<CS02)|(1<<CS00);		/* preskaler = 1024 */
	OCR0 = 108;							/* dodatkowy podzia³ przez 108 (rej. przepe³nienia) */
	TIMSK |= (1<<OCIE0);				/* zezwolenie na przerwanie CompareMatch */
	/* przerwanie wykonywane z czêstotliwoœci¹ ok 10ms (100 razy na sekundê) */
	/* do naszych celów nie musi byæ to bardzo dok³adne 10ms */

	lcd_init();	/* inicjalizacja LCD */

	/* sprawdzamy ile czujników DS18xxx widocznych jest na magistrali */



	/* pêtla nieskoñczona */
	while(1) {
		czujniki_cnt = search_sensors();
lcd_locate(0,0);
		lcd_int(czujniki_cnt);
}
}
