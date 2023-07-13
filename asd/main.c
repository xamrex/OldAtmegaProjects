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


	//clear();
	//napis("JAKIS BARDZO DLUGI NAPIS");
	// po wpisaniu napisu na sztywno musimy wywo³aæ funkcjê d³ugoœæ
	//dlugosc();


//clear();
//sprintf(txt,"DZISIAJ %i",zmienna);
//dlugosc();



		while(1)
		{
;
		}
}
