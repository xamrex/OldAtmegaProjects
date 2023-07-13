#include <avr/io.h>
#include "LCD/lcd44780.h"
#include <avr/interrupt.h>

volatile uint8_t godz,min,sek;
volatile uint16_t total=1; // musimy zaczac od 1 gdzy nie zlicza liczby 2
volatile uint16_t tczas; // calkowity czas w sekundach


int main(void){

uint32_t zmienna=0;
uint32_t i=0;


DDRB=0xff;
lcd_init();
sei();

/***************** konf timera *****************************/
TCCR1B|=(1<<WGM12); //tryb CTC (4)
TCCR1B|=(1<<CS12); //prescaler 256
//OCR1A=43200; //rejstr porownujacy 11,8mhz
OCR1A=62500; // 20mhz
TIMSK|=(1<<OCIE1A); // zezwolenie na comp. match od A


for (zmienna=2;;zmienna++){
		while((zmienna%i) !=0 ){
			i++;
		}

		if (zmienna==i) {
				lcd_locate(0,0);
				lcd_ulint(i);

				total++;
				lcd_locate(1,0);
				lcd_ulint(total);
		}
		i=2;// zacznij od nowa


	}

}
ISR(TIMER1_COMPA_vect){
sek++;
PORTB ^=(1<<PB0);
tczas++; //zwieksz ilosc calkowitego czasu wykonywania skryptu
if(sek==60){sek=0; min++;}
if(min==60){min=0; godz++;}

lcd_locate(1,8);
if(godz<10) lcd_int(0);
lcd_int(godz);
lcd_locate(1,10);
lcd_str(":");
lcd_locate(1,11);
if(min<10) lcd_int(0);
lcd_int(min);
lcd_locate(1,13);
lcd_str(":");
lcd_locate(1,14);
if(sek<10) lcd_int(0);
lcd_int(sek);

//lcd_locate(0,10);



}
