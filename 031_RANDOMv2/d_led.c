/*
 * d_led.c
 *
 *  Created on: 2010-03-30
 *       Autor: Miros³aw Kardaœ
 */
#include <avr/io.h>			// do³¹czenie g³ównego systemowego  pliku nag³ówkowego
#include <avr/interrupt.h>	// do³¹czenie pl. nag³ówkowego potrzebnego do obs³. przerwañ
#include <avr/pgmspace.h>	// do³¹czenie pl. nag³ówkowego potrzebnego do odczytu
							// danych zawartych w pamiêci programu FLASH

#include "d_led.h"			// do³¹czenie naszego pliku nag³ówkowego
							// w nim znajduj¹ siê potrzebne tu m.in. definicje preprocesora

// definicje zmiennych globalnych przechowuj¹cych cyfry do wyœwietlania
// volatile – poniewa¿ bêd¹ wykorzystywane do odczytu i zapisu zarówno w przerwaniu i programie
// g³ównym. Trzeba wiêc wy³¹czyæ optymalizacjê dostêpu do nich. (zmienne ulotne)
volatile int16_t cy1;
volatile int16_t cy2;
volatile int16_t cy3;
volatile int16_t cy4;

// definicja tablicy zawieraj¹cej definicje bitowe cyfr LED
uint8_t cyfry[15] PROGMEM = {
		~(SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F),			// 0
		~(SEG_B|SEG_C),									// 1
		~(SEG_A|SEG_B|SEG_D|SEG_E|SEG_G),				// 2
		~(SEG_A|SEG_B|SEG_C|SEG_D|SEG_G),				// 3
		~(SEG_B|SEG_C|SEG_F|SEG_G),						// 4
		~(SEG_A|SEG_C|SEG_D|SEG_F|SEG_G),				// 5
		~(SEG_A|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G),			// 6
		~(SEG_A|SEG_B|SEG_C|SEG_F),						// 7
		~(SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G),	// 8
		~(SEG_A|SEG_B|SEG_C|SEG_D|SEG_F|SEG_G),			// 9
		0xFF,											// NIC (puste miejsce)
		~(SEG_A|SEG_D|SEG_E|SEG_F),						// 11 C
		~(SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F),			// 12 O
		~(SEG_D|SEG_E|SEG_F),							// 13 L
		~(SEG_A|SEG_G|SEG_D|SEG_F|SEG_E),				// 14 E
		~(SEG_E|SEG_G),									// 15 r
};



// ****** definicja funkcji inicjalizuj¹cej pracê z wyœwietlaczem multipleksowanym
void d_led_init(void)
{
	LED_DATA_DIR = 0xFF;   					// wszystkie piny portu C jako WYJŒCIA(katody)
	LED_DATA = 0xFF;						// wygaszenie wszystkich katod – stan wysoki
	ANODY_DIR |= CA1 | CA2 | CA3 | CA4;		// 4 piny portu A jako WYJŒCIA (anody wyœwietlaczy)
	LED_ANODY |= CA1 | CA2 | CA3  | CA4;	// wygaszenie wszystkich wyœwietlaczy - anody

	// ustawienie TIMER0
	TCCR0 |= (1<<WGM01);				// tryb CTC
	TCCR0 |= (1<<CS02)|(1<<CS00);		// preskaler = 1024
	OCR0 = 9;							// dodatkowy podzia³ przez 39 (rej. przepe³nienia)
	TIMSK |= (1<<OCIE0);				// zezwolenie na przerwanie CompareMatch
}


// ================= PROCEDURA OBS£UGI PRZERWANIA – COMPARE MATCH
ISR(TIMER0_COMP_vect)
{
	static uint8_t licznik=1;		// zmienna do prze³¹czania kolejno anod wyrwietlacza

	LED_ANODY = (LED_ANODY & 0xF0);


	if(licznik==1) 		LED_DATA = pgm_read_byte( &cyfry[cy1] );	// gdy zapalony wyœw.1 podaj stan zmiennej c1
	else if(licznik==2) LED_DATA = pgm_read_byte( &cyfry[cy2] );	// gdy zapalony wyœw.2 podaj stan zmiennej c2
	else if(licznik==4) LED_DATA = pgm_read_byte( &cyfry[cy3] );	// gdy zapalony wyœw.3 podaj stan zmiennej c3
	else if(licznik==8) LED_DATA = pgm_read_byte( &cyfry[cy4] );	// gdy zapalony wyœw.4 podaj stan zmiennej c4

	LED_ANODY = (LED_ANODY & 0xF0) | (~licznik & 0x0F);			// cykliczne prze³¹czanie kolejnej anody w ka¿dym przerwaniu

	// operacje cyklicznego przesuwania bitu zapalaj¹cego anody w zmiennej licznik
	licznik <<= 1;					// przesuniêcie zawartoœci bitów licznika o 1 w lewo
	if(licznik>8) licznik = 1;		// jeœli licznik wiêkszy ni¿ 8 to ustaw na 1
}

