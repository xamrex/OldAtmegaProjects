/*
 * main.c
 *
 *  Created on: 2011-02-24
 *       Autor: XamreX
 *
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "LCD/lcd44780.h"
#include "IR_DECODE/ir_decode.h"
#include <avr/delay.h>

void ekran2(void); // deklaracja funkcji ekran2
volatile uint8_t liczba,wylosowana;
int main(void)
{

	#define LED1_PIN (1<<PA7);
#define LED1_ON PORTA&=~LED1_PIN
#define LED1_OFF PORTA|=LED1_PIN
DDRA |=LED1_PIN;
LED1_OFF;
/********** KONFIGURACJA TIMERA0 do wygenerowania zmiennej losowej **********/
TCCR0|=(1<<CS00);
/*********************** KONFIGURACJA ZMIENNYCH ****************************/


	lcd_init();	/* inicjalizacja LCD */
	ir_init();	/* inicjalizacja dekodowania IR */
	sei();	/* w³¹czamy globalne przerwania */

/********************* EKRAN POWITALNY ***************************************/
lcd_locate(0,0);
lcd_str("Wcisnij OK");
lcd_locate(1,0);
lcd_str("Aby wystartowac!");
lcd_cls();
	while(1){
		//if(Ir_key_press_flag) {
			LED1_ON;_delay_ms(30);LED1_OFF;
lcd_locate(0,0);

liczba = liczba * 10;
liczba=command;
lcd_int(liczba);






		//}
	Ir_key_press_flag=0;// musi byc na koncu while'a
	}
}
/****************** EKRAN POWITALNY  KONIEC*************************************/
/****************** G£ÓWNA FUNKCJA START ***************************************/
void ekran2(void){
lcd_cls();
lcd_locate(0,0);
lcd_str("Kliknij klawisz");
Ir_key_press_flag=0;

	while(1){
		if(Ir_key_press_flag) {
			LED1_ON;_delay_ms(30);LED1_OFF;
			if (command >= 0 && command <=9){   // Jesli wybierzemy cyfre
			lcd_locate(0,0);
			lcd_str("Wybrales:");
			lcd_int(command);
			lcd_str("      ");

			if (command>wylosowana){
				lcd_locate(1,0);
				lcd_str("Za duzo");
			}

			if (command<wylosowana){
				lcd_locate(1,0);
				lcd_str("Za malo");
			}

			if (wylosowana==command){
				lcd_locate(1,0);
				lcd_str("Zgadles");
			}

			}

		}
		Ir_key_press_flag=0;// musi byc na koncu while'a
	}
}


