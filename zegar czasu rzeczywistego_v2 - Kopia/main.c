/*
 * main.c
 *
 *  Created on: 2010-09-07
 *       Autor: Miros³aw Kardaœ
 *
 *       Program korzysta ze programowej obs³ugi I2C
 *       Umo¿liwia zapis danych do RTC oraz EEPROM a tak¿e
 *       oczyt z tych urz¹dzeñ
 *
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "d_led.h"		// do³¹czenie naszego pliku nag³ówkowego (obs³uga LED)
#include "LCD/lcd44780.h"
#include "I2C_SOFT/i2c_soft.h"

#include "headery.h"


volatile uint8_t int0_flag=1;	// flaga zmieniana w przerwaniu i sprawdzana w pêtli g³ównej


int main(void) {
	/********************** KONFIGURACJA DIOD ***********************************/
	DDRB=0xff;
	PORTB=0xff;
	DDRD|=(1<<PD3);


	/********** KONFIGURACJA TIMERA2 do wygenerowania przerwania ****************/
	TCCR2 |=(1<<WGM21); //TRYB CTC
	TCCR2 |=(1<<CS22)|(1<<CS21)|(1<<CS20); //Presclaer = 1024
	OCR2 = 108; //Przerwanie porownaiania co 10ms (100hz)
	TIMSK |= (1<<OCIE2); //Odblokowanie przerwania COmpare Match
	/********** KONFIGURACJA TIMERA2 do wygenerowania przerwania ****************/
	// Przerwanie INT0
	MCUCR |= (1<<ISC01);	// wyzwalanie zboczem opadaj¹cym
	GICR |= (1<<INT0);		// odblokowanie przerwania
	PORTD |= (1<<PD2);		// podci¹gniêcie pinu INT0 do VCC
	/********** KONFIGURACJA TIMERA2 do wygenerowania przerwania ****************/

	sei();
	i2c_init();
	lcd_init();
	d_led_init();   		// inicjalizacja wyœwietlacza multipleksowanego
/*** ZAPISANIE DATY ***/
//	zapis();
/*** ZAPISANIE DATY ***/


	while(1) {

		if ( int0_flag ) {
			rtc_odczyt();
			int0_flag=0;
		}

		if (Timer1==0){
			Timer1=30;	//0,3sek

			cy1=godziny/10;
			cy2=godziny%10;
			cy3=minuty/10;
			cy4=minuty%10;

		}


		}

	}

/************************* ZAPIS DATY *****************************/
void zapis (void) {
/******************  USTAWIENIE CZASU START ************************/

	bufor[1] = dec2bcd(20);	// sekundy
	bufor[2] = dec2bcd(39);	// minuty
	bufor[3] = dec2bcd(14);	// godziny
	dztyg = 4;				// Numer dnia tygodnia (0-PN 6-Nd)
	dnimca = dec2bcd(18);	//dni miesiaca (1-31)
	nrmca = dec2bcd(3);		//Numer miesiaca (1-12)
	rok = 11;			//rok -podjemy tylko najmlodze 2 cyfry roku
/******************  USTAWIENIE CZASU KONIEC ************************/
rokmod=rok%4;			//tu mamy wynik modulo, koty musimy zapisac do RAMU
rokfree=rok/4;				//tu mamy wynik, kty zapiszemy w wolnej pamieci
bufor[4] =( dnimca|(rokmod<<6) );	// 2bit-rok,2bit-dziesiatkidni,4bit-jednostki dni
bufor[5]=( nrmca|(dztyg<<5) );
// zapis 4 bajtów z bufora pod adres 0x01 w pamiêci RAM naszego RTC
I2C_write_buf( PCF8583_ADDR, 0x01, 7, bufor );
//Zapis roku do wolnej komorki RAM
bufor[0]=rokfree;
I2C_write_buf( PCF8583_ADDR, 0x08, 1, bufor );
}
/************************* ZAPIS DATY *****************************/

ISR( INT0_vect ) {
	int0_flag = 1;
}


// konwersja liczby dziesiêtnej na BCD
uint8_t dec2bcd(uint8_t dec) {
return ((dec / 10)<<4) | (dec % 10);
}

// konwersja liczby BCD na dziesiêtn¹
uint8_t bcd2dec(uint8_t bcd) {
    return ((((bcd) >> 4) & 0x0F) * 10) + ((bcd) & 0x0F);
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
