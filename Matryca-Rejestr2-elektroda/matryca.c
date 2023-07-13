#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "matryca.h"


////////////////////////////0    1   2   3   4   5   6   7   8   9   :  ;   <   =   >   ?   @    A   B   C   D   E   F   G   H    I  J   K   L   M   N   O	P	 Q   R	 S   T   U	 V   W	 X	 Y	 Z   SB
uint8_t tabela[8][44] PROGMEM ={
							 { 6,  2, 14, 31,  8, 31, 12, 31, 14, 14,  0,  0,  8,  0,  1, 14, 14, 14, 15, 14,  7, 31, 31, 14, 17,  7, 28, 17,  1, 17, 17, 14, 15, 14, 15, 30, 31, 17, 17, 17, 17, 17, 31,  0},
							 { 9,  3, 17,  8, 12,  1,  2, 17, 17, 17,  6,  6,  4,  0,  2, 17, 17, 17, 17, 17,  9,  1,  1, 17, 17,  2,  8,  9,  1, 27, 17, 17, 17, 17, 17,  1,  4, 17, 17, 17, 17, 17, 16,  0},
							 { 9,  2, 16,  4, 10, 15,  1, 16, 17, 17,  6,  6,  2, 15,  4, 16, 16, 17, 17,  1, 17,  1,  1,  1, 17,  2,  8,  5,  1, 21, 19, 17, 17, 17, 17,  1,  4, 17, 17, 17, 10, 17,  8,  0},
							 { 9,  2,  8,  8,  9, 16, 15,  8, 14, 30,  0,  0,  1,  0,  8,  8, 22, 17, 15,  1, 17, 15, 15, 29, 31,  2,  8,  3,  1, 21, 21, 17, 15, 17, 15, 14,  4, 17, 17, 21,  4, 10,  4,  0},
							 { 9,  2,  4, 16, 31, 16, 17,  4, 17, 16,  0,  0,  2, 15,  4,  4, 21, 31, 17,  1, 17,  1,  1, 17, 17,  2,  8,  5,  1, 17, 25, 17,  1, 21,  5, 16,  4, 17, 17, 21, 10,  4,  2,  0},
							 { 9,  2,  2, 17,  8, 17, 17,  4, 17,  8,  6,  6,  4,  0,  2,  0, 21, 17, 17, 17,  9,  1,  1, 16, 17,  2,  9,  9,  1, 17, 17, 17,  1,  9,  9, 16,  4, 17, 10, 21, 17,  4,  1,  0},
							 { 6,  7, 31, 14,  8, 14, 14,  4, 14,  6,  6,  4,  8,  0,  1,  4, 14, 17, 15, 14,  7, 31,  1, 30, 17,  7,  6, 17, 31, 17, 17, 14,  1, 22, 17, 15,  4, 14,  4, 10, 17,  4, 31,  0},
							 { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}
							 };

/************************************* FUNKCJA SENDSPI ***************************************************/
inline void SendSpi(uint32_t bajt){
	SPDR=bajt>>24;
	while( !(SPSR & (1<<SPIF)) );
	SPDR=bajt>>16;
	while( !(SPSR & (1<<SPIF)) );
	SPDR=bajt>>8;
	while( !(SPSR & (1<<SPIF)) );
	SPDR=bajt;
	while( !(SPSR & (1<<SPIF)) );
	PORTB |= (1<<CS);
	_delay_us(1);
	PORTB &= ~(1<<CS);

	}
/************************************* INIT SENDSPI ***************************************************/
void InitSpi(void){
	DDRB |=(1<<MOSI)|(1<<SCK)|(1<<CS);
	SPCR |=(1<<SPE)|(1<<MSTR);
	SPSR |=(1<<SPI2X);
// KONFIGURACJA PORTOW DO RZEDOW //
	DDRD = 0xff; //caly portD
	PORTD = 0xff; //caly portD

}
/************************************* Inicjacja Timera2 ************************************************/
void InitTimer(void){
TCCR2 |=(1<<WGM21); //TRYB CTC
TCCR2 |=(1<<CS22)|(1<<CS21); //Presclaer = 256
OCR2 = 70; //Przerwanie
TIMSK = (1<<OCIE2); //Odblokowanie przerwania COmpare Match
sei(); //Odblokowanie przerwania
}


/********************************** FUNKCJA Przesuwajaca bity *********************************************/
uint32_t przesuniecie(uint32_t liczba, int8_t ile){

if (ile>=0) return liczba<<ile;
else return liczba>>-ile;
}
/******************************* FUNKCJA Kopiujaca znaki do tablicy ***************************************/
void napis (char * str){
register char znak;
static uint8_t numer=0; //aby poczatkowo numer zawsze wynosil 0
	while ( (znak=*(str++)) ){
		if (znak==32){txt[numer]=43;} // jesli znakiem jest spacja
		else{
			txt[numer]=znak-48;
		}
	numer++;
	}
	dlugosczdania=numer-1; // uaktualnienie / wpisanie do zmiennej z ilu znakow skalada sie znadnie
}
/*************************************** Przerwanie Timera2 *********************************************/
ISR(TIMER2_COMP_vect){
static uint8_t nrwysw=0; //zmienna do wlaczania wybranej kolumny wyswietlacza
uint32_t cowyslac=0; //zmienna ktora wysyla cala 32 bitowa liczbe do funkcji

				for (uint8_t i=0; i<=dlugosczdania;i++){
					cowyslac+=przesuniecie((uint32_t)pgm_read_byte(&tabela[nrwysw][txt[i]]),(i+6)*6-przesow);
				}

				SendSpi(cowyslac);
				PORTD = ~(1<<nrwysw);

		nrwysw++;
		if(nrwysw>7)nrwysw=0;
	}



