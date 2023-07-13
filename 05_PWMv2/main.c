/*
 * main.c
 *
 *  Created on: 2011-03-05
 *       Autor: Miros³aw Kardaœ
 */
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

volatile uint8_t r;

int main(void){
DDRC=0xFF;


TCCR0 |=(1<<CS00); // wlacamy tim 0 i prescalujemy przez 1
TIMSK |=(1<<TOIE0);	// Zezwolenia na przerwania

sei();

while(1){
_delay_ms(6);
r++;
}
}

ISR(TIMER0_OVF_vect){
static uint8_t reference;
reference++;
if (reference < r) PORTC=0x00;
else PORTC=0xff;


}



