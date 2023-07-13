#include <avr/io.h>
#include <util/delay.h>

#define MOSI PB5
#define SCK PB7
#define CS PB4

void SendSpi(uint16_t bajt);
void InitSpi(void);


int main(void){
	InitSpi();
	//SendSpi(0);
/**** PETLA WYSWIETLAJACA CYFRY *****/
	//while(1)
	//{
		//for (uint16_t i=0;i<=65535;i++){
			SendSpi(259);
			_delay_ms(1000);
			SendSpi(2);
		//}

	//}
}
/************ INNE FUNKCJE ************/
void SendSpi(uint16_t bajt){
	SPDR=bajt>>8;
	while( !(SPSR & (1<<SPIF)) );
	SPDR=bajt;
	while( !(SPSR & (1<<SPIF)) );
	PORTB |= (1<<CS);
	//_delay_us(1);
	PORTB &= ~(1<<CS);

	}

void InitSpi(void){
	DDRB |=(1<<MOSI)|(1<<SCK)|(1<<CS);
	SPCR |=(1<<SPE)|(1<<MSTR);
	SPSR |=(1<<SPI2X);
}

