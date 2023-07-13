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
#include "IR_DECODE/ir_decode.h"


int main(void) {
uint8_t pozycja=0;
	DDRA |= (1<<PA7);
	PORTA |= (1<<PA7); /* pod�wietlenie wy�wietlacza LCD */

	lcd_init();	/* inicjalizacja LCD */
	ir_init();	/* inicjalizacja dekodowania IR */

	sei();	/* w��czamy globalne przerwania */

	lcd_locate(0,0);
	lcd_str_P(PSTR("Kody pilota RC5")); /* wy�wietl napis w zerowej linii LCD */

	/* p�tla niesko�czona */
	while(1) {

		if(Ir_key_press_flag) {	/* je�li odebrano prawid�owe kody z pilota */
			if (command==8 && pozycja<2) {
				pozycja++; //Dodanie tego warunku, aby pozycja by�a max 2
				_delay_ms(100);
			}
			if (command==2 && pozycja>0) pozycja--;   //Dodanie tego warunku, aby pozycja by�a min 0
			/* wyzerowanie flagi odbioru oraz warto�ci odebranych kod�w */
			Ir_key_press_flag=0;
			command=0xff;
			address=0xff;
		}

		switch(pozycja){
		case 0:
		lcd_locate(0,0);
		lcd_str("\x7e"); //wysw strzalki
		lcd_locate(0,1);
		lcd_str("Start Gry      ");
		lcd_locate(1,0);
		lcd_str(" Info           ");
		break;

		case 1:
		lcd_locate(0,0);
		lcd_str(" Start Gry      ");
		lcd_locate(1,0);
		lcd_str("\x7e"); //wysw strzalki
		lcd_locate(1,1);
		lcd_str("Info           ");
		break;

		case 2:
		lcd_locate(0,0);
		lcd_str(" Info           ");
		lcd_locate(1,0);
		lcd_str("\x7e"); //wysw strzalki
		lcd_locate(1,1);
		lcd_str("Ustawienia     ");
		break;
		}


	}
}
