#include <avr/io.h>
#include <avr/delay.h>

#define MOSI PB5
#define SCK PB7
#define CS PB4

void SendSpi( uint8_t bajt );
void InitSpi(void);
uint8_t cnt;

int main(void){
	InitSpi();

	//while(1){

			SendSpi(4);

	//}

}


void SendSpi( uint8_t bajt) {
	SPDR=bajt;
	while( !(SPSR & (1<<SPIF)) );

	PORTB |= (1<<CS);
	_delay_us(1);
	PORTB &= ~(1<<CS);
}

void InitSpi(void){
	DDRB |= (1<<MOSI)|(1<<SCK)|(1<<CS);
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR1);
}
