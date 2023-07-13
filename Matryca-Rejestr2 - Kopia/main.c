#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "matryca.h"
#include "RS/rs232.h"


int main(void){

coileprzesuw=30;	// zmienna ktora przesuwa nam tekst co jakis czas

	InitSpi();
	InitTimer();
	USART_init(9600);
	porty_init();
	skopiuj(); //kopiujemy z eeprom na tablice ram

	sprawdzcfg(); //czy tryb seriwisowy?

	//napis("JAKIS BARDZO DLUGI NAPIS");
	//dlugosc();


//sprintf(txt,"DZISIAJ %i",zmienna);
//dlugosc();

int i =64;
			bufor[0+i]=1;
				bufor[2+i]=1;
				bufor[32+i]=1;
				bufor[33+i]=1;
				bufor[34+i]=1;
				bufor[35+i]=1;
				bufor[64+i]=1;
				bufor[66+i]=1;

		while(1)
		{


					//clear();


		}
}
