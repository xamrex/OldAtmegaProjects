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

#include "LCD/lcd44780.h"
#include "I2C_SOFT/i2c_soft.h"

#define PCF8583_ADDR 0xA2

volatile char nazwadnia;
volatile uint8_t int0_flag=1;	// flaga zmieniana w przerwaniu i sprawdzana w pêtli g³ównej
uint8_t dec2bcd(uint8_t dec);
uint8_t bcd2dec(uint8_t bcd);
void zapis(void);
void rtc_odczyt (void);

uint8_t bufor[9];		// rezerwacja bufora 4 bajty
uint8_t sekundy, minuty, godziny,dztyg,dnimca,rokmod,rok,nrmca,rokfree;


int main(void) {

	// Przerwanie INT0
	MCUCR |= (1<<ISC01);	// wyzwalanie zboczem opadaj¹cym
	GICR |= (1<<INT0);		// odblokowanie przerwania
	PORTD |= (1<<PD2);		// podci¹gniêcie pinu INT0 do VCC

	i2c_init();
	lcd_init();

	sei();

	lcd_str("start...");
	zapis();
	int0_flag=0;
	while(1) {

		if ( int0_flag ) {
			//odczyt 4 bajtów do bufora od adresu 0x01 z pamiêci RAM naszego RTC
			I2C_read_buf( PCF8583_ADDR, 0x01, 8, bufor );
			/* ODCZYTYWANIE SEK,MIN,GODZ */
			sekundy = bcd2dec( bufor[1] );
			minuty = bcd2dec( bufor[2] );
			godziny = bcd2dec( bufor[3] );
			/* ODCZYTYWANIE ROKU */
			rokfree = (bufor[7]*4);
			rokfree = (rokfree +(bufor[4]>>6));
			/* ODCZYTYWANIE DNI MCA(1-31) */
			dnimca = (0b00111111 & bufor[4]);
			dnimca = bcd2dec(dnimca);
			/* ODCZYTYWANIE NR MCA(1-12) */
			nrmca = (0b00011111 & bufor[5]);
			nrmca = bcd2dec(nrmca);
			/* ODCZYTYWANIE NAZWY DNIA(Pn-Nd) */
			dztyg = (bufor[5]>>5);	//dien tygodnia (0-PN 6-ND)
			if (dztyg==0) nazwadnia="Pn";
			if (dztyg==1) nazwadnia="Pt";
			if (dztyg==2) nazwadnia="Sr";
			if (dztyg==3) nazwadnia="Cz";
			if (dztyg==4) nazwadnia="Pt";
			if (dztyg==5) nazwadnia="Sob";
			if (dztyg==6) nazwadnia="Nd";

			// wyœwietlenie czasu na LCD
			lcd_locate(0,0);
			if( godziny < 10 ) lcd_str("0");
			lcd_int(godziny);
			lcd_str(":");
			if( minuty < 10 ) lcd_str("0");
			lcd_int(minuty);
			lcd_str(":");
			if( sekundy < 10 ) lcd_str("0");
			lcd_int(sekundy);


			lcd_locate(1,0);
			lcd_str(nazwadnia); //wusw nazwednia (pn-nd)
			lcd_str(":");
			if (dnimca<10) lcd_str("0");
			lcd_int(dnimca); //wysw. dzien miesiaca (1-31)
			lcd_str("-");
			if (nrmca<10) lcd_str("0");									//COS SIE NIE WYSYLA ??
			lcd_int(nrmca); //wysw. dzien miesiaca (1-12)
			lcd_str("-");
			lcd_str("20");
			if (rokfree<10) lcd_str("0");
			lcd_int(rokfree); //wysw. rok

			// POPRAWIENIE WYSW. ROKU //

				if (nrmca==12 && dnimca==31 && godziny==23 && minuty==59 && sekundy==59 &&((bufor[4]>>6) ==3)){
					I2C_read_buf( PCF8583_ADDR, 0x08, 1, bufor );
					rokfree = bufor[7];
					bufor[0]=rokfree+1;
					I2C_write_buf( PCF8583_ADDR, 0x08, 1, bufor );
			}
			int0_flag=0;
		}

	}
}
/************************* ZAPIS DATY *****************************/
void zapis (void) {
/******************  USTAWIENIE CZASU START ************************/

	bufor[1] = dec2bcd(50);	// sekundy
	bufor[2] = dec2bcd(59);	// minuty
	bufor[3] = dec2bcd(23);	// godziny
	dztyg = 3;				// Numer dnia tygodnia (0-PN 6-Nd)
	dnimca = dec2bcd(21);	//dni miesiaca (1-31)
	nrmca = dec2bcd(12);		//Numer miesiaca (1-12)
	rok = 12;			//rok -podjemy tylko najmlodze 2 cyfry roku
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


// procedura obs³ugi przerwania INT 0
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

