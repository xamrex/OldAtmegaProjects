/*
 * main.c
 *
 *  Created on: 2010-09-14
 *       Autor: Miros³aw Kardaœ
 */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
volatile uint16_t pwm1;

/* przydatne definicje pinów steruj¹cych */
#define WE_A PD1
#define WE_B PD0
#define KEY_PIN (1<<PC6)
#define KEY_DOWN !(PINC & KEY_PIN)

/* definicje poleceñ steruj¹cych prac¹ silnika */
#define DC_LEWO PORTD &= ~(1<<WE_A); PORTD |= (1<<WE_B)
#define DC_PRAWO PORTD |= (1<<WE_A); PORTD &= ~(1<<WE_B)
#define DC_STOP PORTD &= ~(1<<WE_A); PORTD &= ~(1<<WE_B)
#define DC_TOGP  PORTD ^= DC_PRAWO //stop i w prwao
#define DC_TOGL  PORTD ^= DC_LEWO //stop i w lewo

int main(void) {
	sei();
	DDRC=0xff;
	DDRC &= ~KEY_PIN;
	PORTC |=(KEY_PIN);
	TCCR2 |=(1<<CS20); //presc. 1
	TIMSK |=(1<<OCIE2);
	OCR2=200;
	/* ustawiamy piny steruj¹ce L293D jako wyjœcia */
	DDRD |= (1<<WE_A)|(1<<WE_B);

	pwm1=200;
	DC_PRAWO;

	while(1) {

		if (KEY_DOWN){
			_delay_ms(80);
			if(KEY_DOWN){
				DC_TOGP;
			}
		}

//		_delay_ms(1000);
//		DC_STOP;
//		_delay_ms(1000);
//		DC_LEWO;
//		_delay_ms(1000);
//		DC_STOP;
//		_delay_ms(1000);
	}
}
ISR(TIMER2_COMP_vect){
static uint8_t cnt;
if (cnt>=pwm1) PORTC &= ~(1<<PC0); else PORTC |=(1<<PC0);

cnt++;

		}
