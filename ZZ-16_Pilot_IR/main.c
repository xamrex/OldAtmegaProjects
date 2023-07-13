/*
 * main.c
 *
 *  Created on: 2010-09-24
 *       Autor: Miros³aw Kardaœ
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
// Tutaj definiujemy jaki adres bêdzie emitowa³ pilot IR
#define _ADDRESS_RC5 0		// TV = 0


// definicje dla obs³ugi diody LED
#define LED_PIN_NR 2
#define LED (1<<LED_PIN_NR)


uint8_t address;
uint8_t command;

void czekaj_us(uint16_t usekundy);
void send_rc5_one();
void send_rc5_zero();
void send_rc5(uint8_t adr, uint8_t cmd, uint8_t tog);
/**************** MAIN **********************/
int main(void) {


	DDRB 	= 	0xff;
	PORTB	|=	(LED);


	// **************************************************************************************
	// TIMER0
	// - u¿ywany do generowania fali noœnej dla IR
	// za³adowanie OCR0A wartoœci¹ do generowania noœnej ok 36kHz = 110
	OCR0A =  110;
	// ustawienie Timer0 w tryb = 2 - CTC - CompareA
	TCCR0A |= (1<<WGM01);	// tryb CTC
	TCCR0B |= (1<<CS00);	// preskaler = 1
	//****************************************************************************************

	// Timer1 - s³u¿y do odmierzania opóŸnieñ (z dok³adnoœci¹ wielokrotnoœci 1us)
	// jego preskaler ustawiany jest na 8 podczas za³¹czania
	// u¿ywana jest wtedy jeo flaga OCF0A
	// nie korzystamy z przerwañ
	// ustawiamy Timer1 w tryb = 4 - CTC
	TCCR1B |= (1<<WGM12);


	// globalne zezwolenie na przerwania
	sei();

	while (1) {

static uint8_t toggle_bit;



//command=9;
//address = _ADDRESS_RC5;
//send_rc5(address, command, toggle_bit);

send_rc5_one();
send_rc5_one();
send_rc5_one();

send_rc5_zero();
send_rc5_one();
send_rc5_zero();
send_rc5_one();
send_rc5_zero();
send_rc5_one();

send_rc5_one();
send_rc5_zero();
send_rc5_one();
send_rc5_one();

send_rc5_zero();
send_rc5_one();
send_rc5_one();

send_rc5_zero();
send_rc5_one();
send_rc5_one();

_delay_ms(1615);	// gap


	}
}

//------------------------------------------------- RC5 - START
// dok³adna pêtla opóŸniaj¹ca = wielokrotnoœci 1us
// w oparciu o Timer1, taktowanie 8MHz, preskaler = 8
void czekaj_us(uint16_t usekundy) {
	OCR1A = usekundy;
	TIFR |= (1<<OCF1A);
	TCCR1B |= (1<<CS11);
	while ( !( TIFR & (1<<OCF1A) ) ) {};
	TCCR1B &= ~(1<<CS11);
}
// przes³anie bitu o wartoœci 1
void send_rc5_one() {
	czekaj_us(2100);
	TCCR0A |= (1<<COM0A0);
	czekaj_us(500);
	TCCR0A &= ~(1<<COM0A0);
}
// przes³anie bitu o wartoœci 0
void send_rc5_zero() {
//	TCCR0A |= (1<<COM0A0);
//	czekaj_us(0);
	TCCR0A &= ~(1<<COM0A0);
	czekaj_us(2600);
}



//// przes³anie kompletnej ramki RC5
//void send_rc5(uint8_t adr, uint8_t cmd, uint8_t tog) {
//	uint16_t data = 0;
//	uint8_t i=15;
//
//	// Musimy uformowaæ ramkê RC5
//	// w tym celu przesuwamy bity do wys³ania w lew¹ stronê
//	data |= ( (1<<15)|(1<<14)|(tog<<13)|(adr<<8)|(cmd<<2) );
//	/* data = 0bssTaaaaaccccccxx
//	 *   	  	||||adr||cmd |
//	 *   		|||
//	 *   		||+-- bit TOGGLE
//	 *			||
//	 *			++--- 2 bity Startu
//	*/
//
//	// wysy³amy kolejno 14 bitów
//	// 2-bity startu, 1-bit Toggle, 5-bitów adresu, 6-bitów komendy
//	do {
//		if ( !(data & ( 1 << i )) ) send_rc5_zero();
//		else send_rc5_one();
//	} while(--i>1);
//}
//

