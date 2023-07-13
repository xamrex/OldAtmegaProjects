#include <avr/io.h>
#include <avr/pgmspace.h>
#include "nokia/lcd.h"
#include "LCD/lcd44780.h"
#include "odczytgodzin.h"
#include <avr/interrupt.h>
void int_init(void);
volatile uint8_t tapeta=1,wyczyszczone=0;
/* main -- Program starts here */
int main(void)
{

	// Setup NOKIA LCD
	int_init();
	sei();
	lcd_ninit();
	lcd_contrast(90);
	lcd_init();
	lcd_str_P(PSTR("Mamy teraz:"));
	//	obrazek1();

	while(1){
		aktualizacja(); 	//aktualizuje
		odczytgodzinyLCD(); // wywsietla na LCD
		if (tapeta){
			if (wyczyszczone==0){lcd_clear(); oproznijbufor(); wyczyszczone=1;}
		   	odczytgodzinyNokia(); }
			else odczytgodzinyNokiaanalog();
		}

}
void int_init(void){
DDRB &= ~(1<<PB3); //int2 jako wejscie
PORTB |= (1<<PB3); //ustawienie stanu wysokiego
GICR |= (1<<INT2); //uaktywnienie przerwania numer 2
MCUCSR &=~(1<<ISC2); //przerwanie aktywne na zbocze opadaj¹ce
}
ISR(INT2_vect){
tapeta=!tapeta;
wyczyszczone=0;
}
