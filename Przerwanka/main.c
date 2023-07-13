#include <avr/io.h>
#include <avr/interrupt.h>

#define LED1_PIN (1<<PA7)
#define LED1_ON PORTA&=~LED1_PIN
#define LED1_OFF PORTA|=LED1_PIN
volatile uint8_t flaga;
int main(void){
sei();// zezwolenie na przerwania
DDRA |=LED1_PIN; // ustawienie diody jako wyjscie
flaga=1; //usatwienie 1 aby dioda nie swiecil
/********** KONFIGURACJA TIMERA0 do wygenerowania przerwania ****************/
TCCR0 |=(1<<CS00)|(1<<CS02); // wlacamy tim 0 i prescalujemy przez 1024
TIMSK |=(1<<TOIE0);	// Zezwolenia na przerwania
/*********************** INICJALIZACJA WYSW I RC5 ***************************/

while (1){
if (flaga==1) LED1_OFF;
if (flaga==0) LED1_ON;
	}
}


ISR(TIMER0_OVF_vect){
static uint8_t liczbat;
   liczbat = liczbat+1;
      if (liczbat==60){
    	  flaga=0;
         liczbat=0;
      }
}
