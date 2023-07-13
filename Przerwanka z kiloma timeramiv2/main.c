#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include "LCD/lcd44780.h"

#define LED1_PIN (1<<PB0)
#define LED1_ON PORTB&=~LED1_PIN
#define LED1_OFF PORTB|=LED1_PIN
#define LED1_TOGG PORTB ^=LED1_PIN

#define LED2_PIN (1<<PB1)
#define LED2_TOGG PORTB ^=LED2_PIN

volatile uint8_t flaga,Timer1, Timer2, Timer3, Timer4;
int main(void){
sei();// zezwolenie na przerwania
DDRB |=LED1_PIN|LED2_PIN; // ustawienie diody jako wyjscie
PORTB=0xff;
/********** KONFIGURACJA TIMERA2 do wygenerowania przerwania ****************/
TCCR2 |=(1<<WGM21); //TRYB CTC
TCCR2 |=(1<<CS22)|(1<<CS21)|(1<<CS20); //Presclaer = 1024
OCR2 = 108; //Przerwanie porownaiania co 10ms (100hz)
TIMSK = (1<<OCIE2); //Odblokowanie przerwania COmpare Match
/*********************** INICJALIZACJA WYSW I RC5 ***************************/
lcd_init();
Timer1=0;
Timer2=10;

while (1){
if(!Timer1){
	Timer1=10;
	LED1_TOGG;

}

if(!Timer2){
	Timer2=10;
	LED2_TOGG;
}

	}


}


ISR(TIMER2_COMP_vect){

uint16_t x;
x = Timer1;				// 100Hz Timer1
if (x) Timer1 = --x;
x = Timer2;				// 100Hz Timer2
if (x) Timer2 = --x;
x = Timer3;				// 100Hz Timer3
if (x) Timer3 = --x;
x = Timer4;				// 100Hz Timer4
if (x) Timer4 = --x;
}
