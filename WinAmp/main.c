#include <avr/io.h>
#include <util/delay.h>
#include "RS/rs232.h"
#include "LCD/lcd44780.h"
#include <avr/interrupt.h>

#define K1_pin (1<<PD7)
#define K2_pin (1<<PD6)
#define K3_pin (1<<PD5)
#define K4_pin (1<<PD4)
#define K5_pin (1<<PD3)

#define K1_down !(PIND & K1_pin)
#define K2_down !(PIND & K2_pin)
#define K3_down !(PIND & K3_pin)
#define K4_down !(PIND & K4_pin)
#define K5_down !(PIND & K5_pin)

void spr_klawisze(void);

int main(void) {
char zmienna;
	_delay_ms(10);
	/********** KONFIGURACJA TIMERA2 do wygenerowania przerwania ****************/
	TCCR0 |=(1<<WGM01); //TRYB CTC
	TCCR0 |=(1<<CS02)|(1<<CS00); //Presclaer = 1024
	OCR0 = 255; //Przerwanie porownaiania co ~23ms
	TIMSK = (1<<OCIE0); //Odblokowanie przerwania COmpare Match

	PORTD=K1_pin|K2_pin|K3_pin|K4_pin|K5_pin;
	USART_init(9600);			// inicjalizacja UART
	lcd_init();
	lcd_str("Czekam...");
	sei();

	// pêtla nieskoñczona
	while(1) {

zmienna=USART_Recive();


/***************** Wysw Tekstu **************************/

if (zmienna !='\r' && zmienna!='\n')		lcd_char(zmienna);

		if (zmienna=='\n') {
			lcd_str("              ");
			lcd_locate(0,0);
		}

		if (zmienna=='\r') {
			lcd_str("              ");
			lcd_locate(1,0);
		}

	}

}

void spr_klawisze(void){
/******************** Obsluga klawiszy ******************/
			/**** KLAWISZ NUMER 1 ****/
					if (K1_down){
						_delay_ms(20);
						if (K1_down){
							USART_Transmit('1');
							_delay_ms(40);
						}
					}

			/**** KLAWISZ NUMER 2 ****/
					if (K2_down){
						_delay_ms(20);
						if (K2_down){
							USART_Transmit('2');
							_delay_ms(40);
						}
					}
			/**** KLAWISZ NUMER 3 ****/
					if (K3_down){
						_delay_ms(20);
						if (K3_down){
							USART_Transmit('3');
							_delay_ms(180);
						}
					}
			/**** KLAWISZ NUMER 4 ****/
					if (K4_down){
						_delay_ms(20);
						if (K4_down){
							USART_Transmit('4');
							_delay_ms(180);
						}
					}
			/**** KLAWISZ NUMER 5 ****/
					if (K5_down){
						_delay_ms(20);
						if (K5_down){
							USART_Transmit('5');
							_delay_ms(180);
						}
					}
/****************** KONIEC OBSLUGI KLAWISZY **************/
}
ISR(TIMER0_COMP_vect){
	spr_klawisze();
}
