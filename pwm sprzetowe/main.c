/*
 * main.c
 *
 *  Created on: 2011-03-07
 *       Autor: Miros³aw Kardaœ
 */
#include <avr/io.h>
#include <avr/interrupt.h>
volatile uint16_t pwm1;
int main(void)
{
sei();
DDRC = 0xff;



/// TRYB NORMAL
TCCR2 |=(1<<CS20); //presc. 1
TIMSK |=(1<<OCIE2);
OCR2=200;
pwm1=21;
while(1);
}

ISR(TIMER2_COMP_vect){
static uint8_t cnt;
if (cnt>=pwm1) PORTC |=(1<<PC0); else PORTC &= ~(1<<PC0);

cnt++;

		}
