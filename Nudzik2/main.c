/*
 * main.c
 *
 *  Created on: 2012-08-01
 *       Autor: XamreX
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t sek=0;
volatile uint8_t min=0;

#define KEY1_PIN (1<<PD0)
#define KEY2_PIN (1<<PD1)
#define KEY3_PIN (1<<PD2)
#define KEY4_PIN (1<<PD3)
#define KEY5_PIN (1<<PD4)

#define KEY1_DOWN !(PIND & KEY1_PIN)
#define KEY2_DOWN !(PIND & KEY2_PIN)
#define KEY3_DOWN !(PIND & KEY3_PIN)
#define KEY4_DOWN !(PIND & KEY4_PIN)
#define KEY5_DOWN !(PIND & KEY5_PIN)

#define SEG_A (1<<0)
#define SEG_B (1<<1)
#define SEG_C (1<<2)
#define SEG_D (1<<3)
#define SEG_E (1<<4)
#define SEG_F (1<<5)
#define SEG_G (1<<6)


uint8_t cyfry[10]  = {
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

};


int main(void){

uint8_t klik=0;

	/**************** TIMER ************************/
	// ustawienie TIMER0
	TCCR0 |= (1<<WGM01);				// tryb CTC
		TCCR0 |= (1<<CS02)|(1<<CS00);		// preskaler = 1024
		OCR0 = 1;							// dodatkowy podzia³ przez 39 (rej. przepe³nienia)
		TIMSK |= (1<<OCIE0);				// zezwolenie na przerwanie CompareMatch





	uint8_t liczba1=1,liczba2=2,liczba3=3,liczba4=4;



 /// INICLAJIZALCJA
 DDRA=0xff; //kieruenk wyjscie
 DDRC=0xff; //kieruenk wyjscie

 PORTA=0xff; //STAN WYSOKI 5V
 PORTC=0x00; //STAN WYSOKI 5V

 DDRD=0x00; //keruenk wejscie
 PORTD=0xff; //ustaiwasz 5V




	while(1)
	{
		liczba1=min/10;
		liczba2=min%10;

		liczba3=sek/10;
		liczba4=sek%10;



			PORTC=~(1<<PC0); //zaplaasz 1 tranzystor
			PORTA=cyfry[liczba1];
			_delay_ms(2);

			PORTC=0xff ; //gasisz 1 tranzystor
			PORTC=~(1<<PC1); //zaplaasz 2 tranzystor
			PORTA=cyfry[liczba2];

			_delay_ms(2);
			PORTC=0xff ; //gasisz 2 tranzystor
			PORTC=~(1<<PC2); //zaplaasz 3 tranzystor
			PORTA=cyfry[liczba3];

			_delay_ms(2);
			PORTC=0xff ; //gasisz 2 tranzystor
			PORTC=~(1<<PC3); //zaplaasz 4 tranzystor
			PORTA=cyfry[liczba4];
			_delay_ms(2);



		if (KEY1_DOWN ){
			_delay_ms(50);
				if (KEY1_DOWN ){
					min+=10;
				}
			}

		if (KEY2_DOWN ){
			_delay_ms(50);
				if (KEY2_DOWN ){
					min++;
				}
		}


		if (KEY3_DOWN ){
			_delay_ms(50);
				if (KEY3_DOWN ){
					sek+=10;
				}
		}

		if (KEY4_DOWN ){
			_delay_ms(50);
				if (KEY4_DOWN ){
				sek++;
				}
		}
	///////////////////////////////////////
		if (KEY5_DOWN ){

															if(klik==1){
																cli();
																		if (KEY5_DOWN){
																			_delay_ms(50);
																								if (KEY5_DOWN){

																								}
																		}
															}





					_delay_ms(50);
						if (KEY5_DOWN && klik==0){
						sei();
						klik=1;
						_delay_ms(50);
						}
				}

	//////////////////////////

	}
}

ISR(TIMER0_COMP_vect)
{
	static uint8_t zmienna=0;
	zmienna ++;
	if (zmienna>=100) {
		zmienna=0;
	sek++;
			if (sek>59) {sek=0; min++;}
	}

 if(min>59) min=0;




}





