/*
 * main.c
 *
 *  Created on: 2011-03-18
 *       Autor: XamreX
 */

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

#define C1 ~(1<<PC4)
#define C2 ~(1<<PC3)
#define C3 ~(1<<PC2)
#define C4 ~(1<<PC1)
#define C5 ~(1<<PC0)
// cyfry 0-9 Litery KOLEJNO A,b,c
uint8_t tabela1[]={124,  0, 70, 68, 48,  0,124,134,108,242,255,255,15};
uint8_t tabela2[]={130,  0,138,130, 80,242,162,136,146,146,136,  9,9};
uint8_t tabela3[]={130, 66,146,130,158,146,162,144,146,146,136,  9,9};
uint8_t tabela4[]={130,254,162,146,  0,158,162,160,146,146,136,  9,9};
uint8_t tabela5[]={124,  2,66 ,108,  0,  0,156,192,108,254,255, 15,0};

volatile uint8_t znak;
uint8_t nrwysw;
int main(void){
	sei();
	/********** KONFIGURACJA TIMERA2 do wygenerowania przerwania ****************/
	TCCR2 |=(1<<WGM21); //TRYB CTC
	TCCR2 |=(1<<CS22)|(1<<CS21); //Presclaer = 256
	OCR2 = 31; //Przerwanie porownaiania co 1ms (1000hz)
	TIMSK = (1<<OCIE2); //Odblokowanie przerwania COmpare Match
	/********** KONFIGURACJA TIMERA2 do wygenerowania przerwania ****************/
	DDRC=0xff; //C jako wyjscie
	DDRD=0xff; // D jako wyscjie
	PORTD=0xff; // na C wysoki (zalaczenie) wierszy(1-8)
	PORTC=0xff; // wygaszenie kolumn (1-5)

	while(1){
for (int i=0;i<13;i++){
	znak=i;
	_delay_ms(800);
}

	}

}




ISR(TIMER2_COMP_vect){
switch(nrwysw){
	case 0:
	PORTC=C1;
	PORTD=tabela1[znak];
	break;

	case 1:
	PORTC=C2;
	PORTD=tabela2[znak];
	break;

	case 2:
	PORTC=C3;
	PORTD=tabela3[znak];
	break;

	case 3:
	PORTC=C4;
	PORTD=tabela4[znak];
	break;

	case 4:
	PORTC=C5;
	PORTD=tabela5[znak];
	break;
}



nrwysw++;
if(nrwysw>4)nrwysw=0;
}
