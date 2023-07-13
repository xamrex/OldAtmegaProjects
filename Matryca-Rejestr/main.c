#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#define MOSI PB5
#define SCK PB7
#define CS PB4

#define r1 PD0
#define r2 PD1
#define r3 PD2
#define r4 PD3
#define r5 PD4
#define r6 PD5
#define r7 PD6
#define r8 PD7

// cyfry 0-3
uint8_t tabela1[] PROGMEM ={12,  8, 12, 12};
uint8_t tabela2[] PROGMEM ={18, 12, 18, 18};
uint8_t tabela3[] PROGMEM ={18, 10,  8, 16};
uint8_t tabela4[] PROGMEM ={18,  8,  4,  8};
uint8_t tabela5[] PROGMEM ={18,  8,  2, 16};
uint8_t tabela6[] PROGMEM ={18,  8,  2, 18};
uint8_t tabela7[] PROGMEM ={12, 62, 30, 12};
uint8_t tabela8[] PROGMEM ={ 0,  0,  0,  0};

void SendSpi(uint32_t bajt);
void InitSpi(void);
void swiatelko(void);
void InitTimer(void);
volatile uint8_t znak;

int main(void){

	InitSpi();
	InitTimer();
	SendSpi(0);
	sei();


while(1)
{
	for(uint8_t i=0;i<4;i++){
		znak=i;
	_delay_ms(1000);
	}
}

}



/************ INNE FUNKCJE ************/
void SendSpi(uint32_t bajt){
	SPDR=bajt>>24;
	while( !(SPSR & (1<<SPIF)) );
	SPDR=bajt>>16;
	while( !(SPSR & (1<<SPIF)) );
	SPDR=bajt>>8;
	while( !(SPSR & (1<<SPIF)) );
	SPDR=bajt;
	while( !(SPSR & (1<<SPIF)) );
	PORTB |= (1<<CS);
	_delay_us(1);
	PORTB &= ~(1<<CS);

	}

void InitSpi(void){
	DDRB |=(1<<MOSI)|(1<<SCK)|(1<<CS);
	SPCR |=(1<<SPE)|(1<<MSTR);
	SPSR |=(1<<SPI2X);
/** KONFIGURACJA PORTOW DO RZEDOW **/
	DDRD = (1<<r1)|(1<<r2)|(1<<r3)|(1<<r4)|(1<<r5)|(1<<r6)|(1<<r7)|(1<<r8);
	PORTD = (1<<r1)|(1<<r2)|(1<<r3)|(1<<r4)|(1<<r5)|(1<<r6)|(1<<r7)|(1<<r8);

}
void InitTimer(void){
/********** KONFIGURACJA TIMERA2 do wygenerowania przerwania ****************/
TCCR2 |=(1<<WGM21); //TRYB CTC
TCCR2 |=(1<<CS22)|(1<<CS21); //Presclaer = 256
OCR2 = 33; //Przerwanie porownaiania co 1ms (1000hz)
TIMSK = (1<<OCIE2); //Odblokowanie przerwania COmpare Match
/********** KONFIGURACJA TIMERA2 do wygenerowania przerwania ****************/
}

ISR(TIMER2_COMP_vect){
static uint8_t nrwysw=0; //zmienna do wlaczania wybranej kolumny wyswietlacza
switch(nrwysw){
	case 0:
	PORTD = 0xff;
	SendSpi(pgm_read_byte(&tabela1[znak]));
	PORTD = ~(1<<r1);
	break;

	case 1:
	PORTD = 0xff;
	SendSpi(pgm_read_byte(&tabela2[znak]));
	PORTD = ~(1<<r2);
	break;

	case 2:
	PORTD = 0xff;
	SendSpi(pgm_read_byte(&tabela3[znak]));
	PORTD = ~(1<<r3);
	break;

	case 3:
		PORTD = 0xff;
	SendSpi(pgm_read_byte(&tabela4[znak]));
	PORTD = ~(1<<r4);
	break;

	case 4:
	PORTD = 0xff;
	SendSpi(pgm_read_byte(&tabela5[znak]));
	PORTD = ~(1<<r5);
	break;

	case 5:
	PORTD = 0xff;
	SendSpi(pgm_read_byte(&tabela6[znak]));
	PORTD = ~(1<<r6);
	break;

	case 6:
	PORTD = 0xff;
	SendSpi(pgm_read_byte(&tabela7[znak]));
	PORTD = ~(1<<r7);
	break;

	case 7:
	PORTD = 0xff;
	SendSpi(pgm_read_byte(&tabela8[znak]));
	PORTD = (unsigned char)~(1<<r8);
	break;
}
nrwysw++;
if(nrwysw>7)nrwysw=0;
}

