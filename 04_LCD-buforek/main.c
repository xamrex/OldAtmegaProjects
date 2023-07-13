/*
 * main.c
 *
 *  Created on: 2010-03-31
 *       Autor: Miros³aw Kardaœ
 */
#include <avr/io.h>
#include <avr/delay.h>
#include "LCD/lcd44780.h"
#include <avr/interrupt.h>

uint8_t flaga,Timer1, Timer2, Timer3, Timer4; // zmienne timerow

int main(void)
{
uint8_t i;
	DDRD |= (1<<PD0);
	PORTD |= (1<<PD0);
	lcd_init();

/********** KONFIGURACJA TIMERA2 do wygenerowania przerwania ****************/
		TCCR2 |=(1<<WGM21); //TRYB CTC
		TCCR2 |=(1<<CS22)|(1<<CS21)|(1<<CS20); //Presclaer = 1024
		OCR2 = 108; //Przerwanie porownaiania co 10ms (100hz)
		TIMSK |= (1<<OCIE2); //Odblokowanie przerwania COmpare Match
/********** KONFIGURACJA TIMERA2 do wygenerowania przerwania ****************/

while(1){
for (i=15;i>=0;i--){

lcd_locate(0,i);
lcd_str("Jakis txt       ");
_delay_ms(70);
}
_delay_ms(1000);
lcd_cls();
}
PORTD &= ~(1<<PD7);





}






ISR(TIMER2_COMP_vect){

uint16_t x;
x = Timer1;				// 100Hz Timer1
if (x) Timer1 = --x;
x = Timer2;				// 100Hz Timer2
if (x) Timer2 = --x;
x = Timer3;				// 100Hz Timer3
if (x) Timer3 = --x;
x = Timer4;				// 100Hz Timer4
if (x) Timer4 = --x;
}

