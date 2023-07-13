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
volatile uint8_t cy1;
volatile uint8_t cy2;
volatile uint8_t cy3;
volatile uint8_t cy4;
volatile uint8_t cy5;
volatile uint8_t cy6;


// definicja tablicy zawieraj¹cej definicje bitowe cyfr LED
uint8_t cyfry[15] PROGMEM = {
		(SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F),			// 0
		(SEG_B|SEG_C),									// 1
		(SEG_A|SEG_B|SEG_D|SEG_E|SEG_G),				// 2
		(SEG_A|SEG_B|SEG_C|SEG_D|SEG_G),				// 3
		(SEG_B|SEG_C|SEG_F|SEG_G),						// 4
		(SEG_A|SEG_C|SEG_D|SEG_F|SEG_G),				// 5
		(SEG_A|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G),			// 6
		(SEG_A|SEG_B|SEG_C|SEG_F),						// 7
		(SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G),	// 8
		(SEG_A|SEG_B|SEG_C|SEG_D|SEG_F|SEG_G),			// 9
		0x00,											// NIC (puste miejsce)
		(SEG_G),										// - (znak MINUSA)
		(SEG_A|SEG_B|SEG_F|SEG_G),						// znak stopnia
		(SEG_A|SEG_E|SEG_F|SEG_D)						// Lietrka C

};



// ****** definicja funkcji inicjalizuj¹cej pracê z wyœwietlaczem multipleksowanym
void d_led_init(void)
{

	LED_DATA_DIR = (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G);   					// wszystkie piny portu C jako WYJŒCIA(katody)
	LED_DATA = 0xFF;						// wygaszenie wszystkich katod – stan wysoki
	ANODY_DIR |= (CA1 | CA2 | CA3 | CA4 | CA5 | CA6);		// 4 piny portu A jako WYJŒCIA (anody wyœwietlaczy)
	ANODY_PORT |= (CA1 | CA2 | CA3  | CA4 |CA5 | CA6);	// wygaszenie wszystkich wyœwietlaczy - anody


	// ustawienie TIMER0
	// ustawienie TIMER2
	TCCR2 |= (1<<WGM21);				// tryb CTC
	TCCR2 |= (1<<CS22)|(1<<CS21);		// preskaler = 256
	OCR2 = 55;							// dodatkowy podzia³ przez 129 co daje przerwanie co 3ms
	TIMSK |= (1<<OCIE2);				// zezwolenie na przerwanie CompareMatch

//ustawienie timer0
//	TCCR0|=(1<<CS02)|(1<<CS00); //precsaler 1024
//	TIMSK |=(1<<TOIE0);  //zezwolenie na compare macz

}


// ================= PROCEDURA OBS£UGI PRZERWANIA – COMPARE MATCH
ISR(TIMER2_COMP_vect)
{
	static uint8_t licznik=1;		// zmienna do prze³¹czania kolejno anod wyrwietlacza
	ANODY_PORT = (ANODY_PORT | (CA1 | CA2 | CA3  | CA4 |CA5 | CA6));	// wygaszenie wszystkich wyœwietlaczy


	if(licznik==1) 		LED_DATA =  (LED_DATA & 0b00000100) | (pgm_read_byte( &cyfry[cy1] ) & 0b11111011);	// gdy zapalony wyœw.1 podaj stan zmiennej c1
	else if(licznik==2) LED_DATA = (LED_DATA & 0b00000100) | (pgm_read_byte( &cyfry[cy2] ) & 0b11111011);	// gdy zapalony wyœw.2 podaj stan zmiennej c2
	else if(licznik==4) LED_DATA = (LED_DATA & 0b00000100) | (pgm_read_byte( &cyfry[cy3] ) & 0b11111011);	// gdy zapalony wyœw.3 podaj stan zmiennej c3
	else if(licznik==8) LED_DATA = (LED_DATA & 0b00000100) | (pgm_read_byte( &cyfry[cy4] ) & 0b11111011);	// gdy zapalony wyœw.4 podaj stan zmiennej c4
	else if(licznik==16) LED_DATA = (LED_DATA & 0b00000100) | (pgm_read_byte( &cyfry[cy5] ) & 0b11111011);// gdy zapalony wyœw.4 podaj stan zmiennej c4
	else if(licznik==32) LED_DATA = (LED_DATA & 0b00000100) | (pgm_read_byte( &cyfry[cy6] ) & 0b11111011);	// gdy zapalony wyœw.4 podaj stan zmiennej c4

	ANODY_PORT = (ANODY_PORT & ~licznik);			// cykliczne prze³¹czanie kolejnej anody w ka¿dym przerwaniu

//	 operacje cyklicznego przesuwania bitu zapalaj¹cego anody w zmiennej licznik
	licznik <<= 1;					// przesuniêcie zawartoœci bitów licznika o 1 w lewo
	if(licznik>32) licznik = 1;		// jeœli licznik wiêkszy ni¿ 8 to ustaw na 1

}

