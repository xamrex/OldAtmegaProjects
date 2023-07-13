/*
 * main.c
 *
 *  Created on: 2010-04-05
 *       Autor: Miros�aw Karda�
 */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// definicje zmiennych do sterowania 6 kana�ami programowych PWM
// zmienne typu uint8_t, rozdzielczo�� 8-bitowa
volatile uint8_t pwm1, pwm2, pwm3, pwm4, pwm5, pwm6;

// g��wna funkcja programu main()
int main(void)
{



	//***** PROGRAMOWY PWM - 6 KANA��W *******
	// ustawienie pin�w kana��w programowych PWM jako WYJ�CIA
	DDRC |= (1<<PC0)|(1<<PC1)|(1<<PC2);
	// wy��czenie diod LED pod��czonych katodami do wyj��


	// ustawienia TIMER2 w tryb CTC
	TCCR2 |= (1<<WGM21);	// tryb  CTC
	TCCR2 |= (1<<CS20);		// preskaler = 1
	OCR2 = 200;				// dodatkowy podzia� cz�sttotliwo�ci przez 200
	TIMSK |= (1<<OCIE2);	// zezwolenie na przerwanie CompareMatch

	sei();				// odblokowanie globalne przerwa�
	uint8_t i;			// definicja zmiennej i na potrzeby p�tli for()
	pwm1=0;pwm2=0;pwm3=0;

	while(1)
	{


		// p�tla rozjasniajaca  6 diod LED
				for(i=0;i<255;i++)
				{
					pwm1=i;
					_delay_ms(6);
				}
				for(i=0;i<255;i++)
				{
					pwm2=i;
					_delay_ms(6);
				}
				for(i=0;i<255;i++)
				{
					pwm3=i;
					_delay_ms(6);
				}

				_delay_ms(1000); //obie swieca prez 1 sek



		// p�tla stopniowo sciemnajaca 6 diod LED
		for(i=255;i;i--)
		{
			pwm1=i;
			_delay_ms(6);
		}

		for(i=200;i;i--)
		{
			pwm2=i;
			_delay_ms(6);
		}

		for(i=200;i;i--)
		{
			pwm3=i;
			_delay_ms(6);
		}



		_delay_ms(1999);
	}





}




// cia�o procedury obs�ugi przerwania Compare Match Timera2
ISR( TIMER2_COMP_vect )
{
	static uint8_t cnt; // definicja naszego licznika PWM

	// bezpo�rednie sterowanie wyj�ciami kana��w PWM
	if(cnt>=pwm1) PORTC &= ~(1<<PC0); else PORTC |= (1<<PC0);
	if(cnt>=pwm2) PORTC &= ~(1<<PC1); else PORTC |= (1<<PC1);
	if(cnt>=pwm3) PORTC &= ~(1<<PC2); else PORTC |= (1<<PC2);


	cnt++;	// zwi�kszanie licznika o 1
}


