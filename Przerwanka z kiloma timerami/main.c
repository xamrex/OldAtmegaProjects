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

volatile uint8_t s1_flag,sekundy,cnt;
int main(void){
sei();// zezwolenie na przerwania
DDRB |=LED1_PIN|LED2_PIN; // ustawienie diody jako wyjscie
PORTB=0xff;
/********** KONFIGURACJA TIMERA2 do wygenerowania przerwania ****************/
TCCR2 |=(1<<WGM21); //TRYB CTC
TCCR2 |=(1<<CS22)|(1<<CS21)|(1<<CS20); //Presclaer = 1024
OCR2 = 78; //Przerwanie porownaiania co 10ms (100hz)
TIMSK = (1<<OCIE2); //Odblokowanie przerwania COmpare Match
/*********************** INICJALIZACJA WYSW I RC5 ***************************/
lcd_init();


while (1){
if(s1_flag){
	if(0==(sekundy%5)) {lcd_locate(0,0); lcd_str("TEKST1");};
	if(1==(sekundy%5)) {lcd_locate(0,0); lcd_str("TEKST2");};
	if(2==(sekundy%5)) {lcd_locate(0,0); lcd_str("TEKST3");};
	if(3==(sekundy%5)) {lcd_locate(0,0); lcd_str("TEKST4");};
	if(4==(sekundy%5)) {lcd_locate(0,0); lcd_str("TEKST5");};

	s1_flag=0;
}



}
}


ISR(TIMER2_COMP_vect){

static uint8_t cnt=0;
if(++cnt>99){
	s1_flag=1;
	sekundy++;
	if(sekundy>59) sekundy=0;
	cnt=0;
	}
}
