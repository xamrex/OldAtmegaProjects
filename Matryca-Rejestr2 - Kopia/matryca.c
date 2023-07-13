#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>
#include "matryca.h"
#include <avr/eeprom.h>
#include "rs/rs232.h"

#define tryb 1  // 0 dla znakow 1 dla bufora  !!! (powtorzyc w matryca.h) !!!

uint8_t EEMEM txteeprom[max_dl_zdania];	// tabela przechowywujaca napis w eepromie

																											//  0   1   2   3   4   5   6   7   8   9   :  ;   <   =   >   ?   @    A   B   C   D   E   F   G   H    I  J   K   L   M   N   O   P   Q   R   S   T   U   V   W   X   Y   Z    ....................  a   b   c   d   e   f   g   h   i   j   k   l   m   n   o   p   q   r   s   t   u   v   w   x   y   z
uint8_t tabela[8][123] PROGMEM ={
                             {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 6,  2, 14, 31,  8, 31, 12, 31, 14, 14,  0,  0,  8,  0,  1, 14, 14, 14, 15, 14,  7, 31, 31, 14, 17,  7, 28, 17,  1, 17, 17, 14, 15, 14, 15, 30, 31, 17, 17, 17, 17, 17, 31,  0,  0,  0,  0,  0,  0,  0,  1,  0, 16,  0, 12,  0,  1,  2,  8,  1,  3,  0,  0,  0,  0,  0,  0,  0,  2,  0,  0,  0,  0,  0,  0},
                             {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 9,  3, 17,  8, 12,  1,  2, 17, 17, 17,  6,  6,  4,  0,  2, 17, 17, 17, 17, 17,  9,  1,  1, 17, 17,  2,  8,  9,  1, 27, 17, 17, 17, 17, 17,  1,  4, 17, 17, 17, 17, 17, 16,  0,  0,  0,  0,  0,  0,  0,  1,  0, 16,  0, 18, 30,  1,  0,  0,  1,  2,  0,  0,  0,  0,  0,  0,  0,  2,  0,  0,  0,  0,  0,  0},
                             {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 9,  2, 16,  4, 10, 15,  1, 16, 17, 17,  6,  6,  2, 15,  4, 16, 16, 17, 17,  1, 17,  1,  1,  1, 17,  2,  8,  5,  1, 21, 19, 17, 17, 17, 17,  1,  4, 17, 17, 17, 10, 17,  8,  0,  0,  0,  0,  0,  0, 14, 13, 14, 22, 14,  2, 17, 13,  3,  8,  9,  2, 27, 13, 14, 15, 22, 13, 14,  7, 17, 17, 21, 17, 17, 31},
                             {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 9,  2,  8,  8,  9, 16, 15,  8, 14, 30,  0,  0,  1,  0,  8,  8, 22, 17, 15,  1, 17, 15, 15, 29, 31,  2,  8,  3,  1, 21, 21, 17, 15, 17, 15, 14,  4, 17, 17, 21,  4, 10,  4,  0,  0,  0,  0,  0,  0, 16, 19,  1, 25, 17,  7, 17, 19,  2,  8,  5,  2, 21, 19, 17, 17, 25, 19,  1,  2, 17, 17, 21, 10, 17,  8},
                             {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 9,  2,  4, 16, 31, 16, 17,  4, 17, 16,  0,  0,  2, 15,  4,  4, 21, 31, 17,  1, 17,  1,  1, 17, 17,  2,  8,  5,  1, 17, 25, 17,  1, 21,  5, 16,  4, 17, 17, 21, 10,  4,  2,  0,  0,  0,  0,  0,  0, 30, 17,  1, 17, 31,  2, 30, 17,  2,  8,  3,  2, 21, 17, 17, 15, 30,  1, 14,  2, 17, 17, 21,  4, 30,  4},
                             {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 9,  2,  2, 17,  8, 17, 17,  4, 17,  8,  6,  6,  4,  0,  2,  0, 21, 17, 17, 17,  9,  1,  1, 16, 17,  2,  9,  9,  1, 17, 17, 17,  1,  9,  9, 16,  4, 17, 10, 21, 17,  4,  1,  0,  0,  0,  0,  0,  0, 17, 17, 17, 17,  1,  2, 16, 17,  2,  9,  5,  2, 17, 17, 17,  1, 16,  1, 16, 18, 25, 10, 21, 10, 16,  2},
                             {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 6,  7, 31, 14,  8, 14, 14,  4, 14,  6,  6,  4,  8,  0,  1,  4, 14, 17, 15, 14,  7, 31,  1, 30, 17,  7,  6, 17, 31, 17, 17, 14,  1, 22, 17, 15,  4, 14,  4, 10, 17,  4, 31,  0,  0,  0,  0,  0,  0, 30, 15, 14, 30, 14,  2, 14, 17,  7,  6,  9,  7, 17, 17, 14,  1, 16,  1, 15, 12, 22,  4, 10, 17, 14, 31},
                             {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}
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
}
/************************************* PORTY DO DIOD ***************************************************/
void porty_init(void){
	// KONFIGURACJA PORTOW DO RZEDOW //
	DDRA = (1<<r1)|(1<<r2)|(1<<r3)|(1<<r4)|(1<<r5)|(1<<r6)|(1<<r7)|(1<<r8);
	PORTA = (1<<r1)|(1<<r2)|(1<<r3)|(1<<r4)|(1<<r5)|(1<<r6)|(1<<r7)|(1<<r8);
	// KONFIGURACJA PRZYCISKU, KIEDY PO WCISNIECIU USTAWIAMY JAKIS TEXT DO EEPROM
	DDRB |= K1_pin;
	PORTB |= K1_pin;
}

/************************************* Inicjacja Timera2 ************************************************/
void InitTimer(void){
TCCR2 |=(1<<WGM21); //TRYB CTC
TCCR2 |=(1<<CS22)|(1<<CS21); //Presclaer = 256
OCR2 = 155; //Przerwanie porownaiania co 1ms (500hz)
TIMSK = (1<<OCIE2); //Odblokowanie przerwania COmpare Match
sei(); //Odblokowanie przerwania
}

#if tryb==1
/********************************* FUNKCJA Czyszczaca bufor **********************************************/
void clear (void){
memset(bufor,0x00,256);
}

/******************************** FUNKCJA Zapalajaca dany pixel ******************************************/
void pixel(uint8_t a, uint8_t b){
if ((a>8)|(b>32)|(a==0)|(b==0)) {a=1;b=1;} //zabezpieczenie
if (a==1){bufor[b-1]=1;}
if (a==2){bufor[32+(b-1)]=1;}
if (a==3){bufor[64+(b-1)]=1;}
if (a==4){bufor[96+(b-1)]=1;}
if (a==5){bufor[128+(b-1)]=1;}
if (a==6){bufor[160+(b-1)]=1;}
if (a==7){bufor[192+(b-1)]=1;}
if (a==8){bufor[224+(b-1)]=1;}
}
#endif
/***************************** FUNKCJA Kopiuj¹ca z EEPROM DO RAM ******************************************/
void skopiuj(void){
eeprom_read_block(txt,txteeprom,max_dl_zdania);
dlugosc();
}

/********************************** FUNKCJA Przesuwajaca bity *********************************************/
uint32_t przesuniecie(uint32_t liczba, int8_t ile){

if (ile>=0) return liczba<<ile;
else return liczba>>-ile;
}
/****************************** FUNKCJA Sprawdzajaca dlugosc tekstu ***************************************/
void dlugosc (void){
uint8_t dl=0;
dlugosczdania=0;

	while(txt[dl]){
	dlugosczdania++;
	dl++;
	}
}
/*********************** FUNKCJA Sprawdzajaca czy jestesmy w trybie konfiguracji ***************************/
void sprawdzcfg(void){
	if(K1_down){
		_delay_ms(50);
		if(K1_down){

			unsigned char znak;
			char tablicaeeprom[max_dl_zdania];
			char tablicaznowymtekstem[max_dl_zdania]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
			uint8_t odebrano;
			eeprom_read_block(tablicaeeprom,txteeprom,max_dl_zdania);

			napis("CONF");
			dlugosc();
			flagaprzesuwu=1;
			USART_napis("KONFIGURACJA WYSWIETLACZA\r\n\r\n");

			USART_napis("1.PODGLAD AGTUALNEGO TEKSTU\r\n");
			USART_napis("2.ZAPIS NOWEGO TEKSTU\r\n");

			while(1){
			znak=USART_Recive(); //obior znaku z rsa
			if (znak=='1') {USART_napis("Aktualnie wyswietlany napis to: "); USART_napis(tablicaeeprom); USART_napis(" \r\n");}
			if (znak=='2') {USART_napis("Wprowadz nowy napis i zatwierdz klawiszem ENTER\r\n"); odebrano=0;
								while(1){
								znak=USART_Recive(); //obior znaku z rsa
								if(znak)odebrano++;
								USART_Transmit(znak);
								tablicaznowymtekstem[odebrano-1]=znak;
								if (znak=='\r' || znak=='\n'){tablicaznowymtekstem[odebrano-1]=0; eeprom_write_block(tablicaznowymtekstem,txteeprom,max_dl_zdania); USART_napis("\r\nZapisano !\r\n"); eeprom_read_block(tablicaeeprom,txteeprom,max_dl_zdania); break;}

								}
							}

			}

		}
	}
}

/******************************* FUNKCJA Kopiujaca znaki do tablicy ***************************************/
void napis (char * str){
register char znak;
memset(txt,0x00,max_dl_zdania);
uint8_t numer=0; //aby poczatkowo numer zawsze wynosil 0
	while ( (znak=*(str++)) ){
		txt[numer]=znak;
		numer++;
	}
	dlugosczdania=numer; // uaktualnienie / wpisanie do zmiennej z ilu znakow skalada sie znadnie

}

/*************************************** Przerwanie Timera2 *********************************************/
ISR(TIMER2_COMP_vect){
static uint8_t nrwysw=0; //zmienna do wlaczania wybranej kolumny wyswietlacza

#if tryb==1
	uint32_t suma=0;
	uint32_t pot=1;
	switch(nrwysw){

		case 0:
		for(uint8_t i=0;i<32;i++){
			suma=suma+bufor[i]*pot;
			pot<<=1;
		}
		PORTA = ~(1<<r1);
		break;

		case 1:
		for(uint8_t i=32;i<64;i++){
			suma=suma+bufor[i]*pot;
			pot<<=1;
		}
		PORTA = ~(1<<r2);
		break;

		case 2:
		for(uint8_t i=64;i<96;i++){
			suma=suma+bufor[i]*pot;
			pot<<=1;
		}
		PORTA = ~(1<<r3);
		break;

		case 3:
		for(uint8_t i=96;i<128;i++){
			suma=suma+bufor[i]*pot;
			pot<<=1;
		}
		PORTA = ~(1<<r4);
		break;

		case 4:
		for(uint8_t i=128;i<160;i++){
			suma=suma+bufor[i]*pot;
			pot<<=1;
		}
		PORTA = ~(1<<r5);
		break;

		case 5:
		for(uint8_t i=160;i<192;i++){
			suma=suma+bufor[i]*pot;
			pot<<=1;
		}
		PORTA = ~(1<<r6);
		break;

		case 6:
		for(uint8_t i=192;i<224;i++){
			suma=suma+bufor[i]*pot;
			pot<<=1;
		}
		PORTA = ~(1<<r7);
		break;

		case 7:
		for(uint16_t i=224;i<256;i++){
			suma=suma+bufor[i]*pot;
			pot<<=1;
		}
		PORTA = (unsigned char)~(1<<r8);
		break;


	}
	SendSpi(suma);

#endif


#if tryb == 0
uint32_t cowyslac=0; //zmienna ktora wysyla cala 32 bitowa liczbe do funkcji

				for (uint8_t i=0; i<=dlugosczdania;i++){

					cowyslac+=przesuniecie((uint32_t)pgm_read_byte(&tabela[nrwysw][txt[i]]),(i+6)*6-przesow);
				}


				SendSpi(cowyslac);
				PORTA = ~(1<<nrwysw);

#endif
		nrwysw++;
		if(nrwysw>7)nrwysw=0;
/******************************* OBSLUGA PRZESUNIECIA TEKSTU **********************************/
	timer++;

	if(flagaprzesuwu==0){   //jesli zezowlenia na przesuwanie tekstu(domyslnie)
		if (timer==coileprzesuw){//jesli minie 30ms
			timer=0; //zerujemy timera
			przesow++; //przesuwamy text

		}
		if(przesow==dlugosczdania*6+35) przesow=0;
	}
	else przesow=36;	// w przeciwnym razie wyswietla napis od poczatku wysiwetlacza

}

