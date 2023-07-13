/*
 * main.c
 *
 *  Created on: 2011-03-05
 *       Autor: Miros³aw Kardaœ
 */
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>



int main(void){
sei();
DDRB |=(1<<PB3);


TCCR0 |=(1<<CS00);
TCCR0 |=(1<<WGM00);
TCCR0 |=(1<<COM01)|(1<<COM00);

OCR0=51;
while(1);

}
