#include <avr/io.h>
#include <avr/interrupt.h>


int main(void){
DDRB |= (1<<PB0); // PD7 jako wyjscie tu jest dolaczona dioda
PORTB |= (1<<PB0); //PD7 stan wysoki (dioda nie swieci)

/* konfuguracja przerwania */
DDRD &= ~(1<<PD2); //int2 jako wejscie
PORTD &= ~(1<<PD2); //ustawienie stanu niskiego

GICR |= (1<<INT0); //uaktywnienie przerwania numer 2
MCUCSR |=(1<<ISC2); //przerwanie aktywne na zbocze narastajace
sei();

	while(1)
	{

	}
}


ISR(INT0_vect){
PORTB ^= (1<<PB0);
}
