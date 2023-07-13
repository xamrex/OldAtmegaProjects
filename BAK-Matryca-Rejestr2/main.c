#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
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

uint8_t  bufor[256];
////////////////////////////0    1   2   3   4   5   6   7   8   9   A   B   C   D   E   F   G   H    I  J   K   L   M   N   O	P	 Q   R	 S   T   U	 V   W	 X	 Y	 Z
uint8_t tabela1[] PROGMEM ={14,  2, 14, 31,  8, 31, 12, 31, 14, 14, 14, 15, 14,  7, 31, 31, 14, 17,  7, 28, 17,  1, 17, 17, 14, 15, 14, 15, 30, 31, 17, 17, 17, 17, 17, 31,};
uint8_t tabela2[] PROGMEM ={17,  3, 17,  8, 12,  1,  2, 17, 17, 17, 17, 17, 17,  9,  1,  1, 17, 17,  2,  8,  9,  1, 27, 17, 17, 17, 17, 17,  1,  4, 17, 17, 17, 17, 17, 16,};
uint8_t tabela3[] PROGMEM ={17,  2, 16,  4, 10, 15,  1, 16, 17, 17, 17, 17,  1, 17,  1,  1,  1, 17,  2,  8,  5,  1, 21, 19, 17, 17, 17, 17,  1,  4, 17, 17, 17, 10, 17,  8,};
uint8_t tabela4[] PROGMEM ={17,  2,  8,  8,  9, 16, 15,  8, 14, 30, 17, 15,  1, 17, 15, 15, 29, 31,  2,  8,  3,  1, 21, 21, 17, 15, 17, 15, 14,  4, 17, 17, 21,  4, 10,  4,};
uint8_t tabela5[] PROGMEM ={17,  2,  4, 16, 31, 16, 17,  4, 17, 16, 31, 17,  1, 17,  1,  1, 17, 17,  2,  8,  5,  1, 17, 25, 17,  1, 21,  5, 16,  4, 17, 17, 21, 10,  4,  2,};
uint8_t tabela6[] PROGMEM ={17,  2,  2, 17,  8, 17, 17,  4, 17,  8, 17, 17, 17,  9,  1,  1, 16, 17,  2,  9,  9,  1, 17, 17, 17,  1,  9,  9, 16,  4, 17, 10, 21, 17,  4,  1,};
uint8_t tabela7[] PROGMEM ={14,  7, 31, 14,  8, 14, 14,  4, 14,  6, 17, 15, 14,  7, 31,  1, 30, 17,  7,  6, 17, 31, 17, 17, 14,  1, 22, 17, 15,  4, 14,  4, 10, 17,  4, 31,};
uint8_t tabela8[] PROGMEM ={ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,};

volatile uint8_t nrwysw=0; //zmienna do wlaczania wybranej kolumny wyswietlacza
volatile uint8_t tryb;
volatile uint32_t znak,znak1,znak2,znak3,znak4,znak5,znak6,znak7,znak8,znak9;
volatile uint8_t przesow=0;
void SendSpi(uint32_t bajt);
void InitSpi(void);
void swiatelko(void);
void InitTimer(void);
void clear (void);
void pixel(uint8_t a, uint8_t b);
void update(void);
uint32_t przesuniecie(uint32_t liczba, int8_t ile);

int main(void){

	InitSpi();
	InitTimer();
	SendSpi(0);
	sei();
	tryb=0; //dla znakow
//	tryb=1; // dla bufora



znak1=10;
znak2=11;
znak3=12;
znak4=13;
znak5=14;
znak6=15;
while(1)
	{
	for(uint8_t i=0;i<30;i++){
		przesow=i;
		_delay_ms(500);
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
OCR2 = 100; //Przerwanie porownaiania co 1ms (1000hz)
TIMSK = (1<<OCIE2); //Odblokowanie przerwania COmpare Match
/********** KONFIGURACJA TIMERA2 do wygenerowania przerwania ****************/
}
void clear (void){
memset(bufor,0x00,256);
}


void pixel(uint8_t a, uint8_t b){
if ((a>8)|(b>32)|(a==0)|(b==0)) {a=1;b=1;} //zabezpieczenie
if (a==1){bufor[b-1]=1;}
if (a==2){bufor[32+(b-1)]=1;}
if (a==3){bufor[64+(b-1)]=1;}
if (a==4){bufor[96+(b-1)]=1;}
if (a==5){bufor[128+(b-1)]=1;}
if (a==6){bufor[160+(b-1)]=1;}
if (a==7){bufor[192+(b-1)]=1;}
if (a==8){bufor[224+(b-1)]=1;}
}

//////////////// FUNKCJA ///////////////////
uint32_t przesuniecie(uint32_t liczba, int8_t ile){

if (ile>=0) return liczba<<ile;
else return liczba>>-ile;
}


ISR(TIMER2_COMP_vect){
	uint32_t suma0,suma1,suma2,suma3,suma4,suma5,suma6,suma7;
	static uint8_t nrwysw=0; //zmienna do wlaczania wybranej kolumny wyswietlacza
	uint32_t pot=1;

	if (tryb==1){
	switch(nrwysw){

		case 0:

//		PORTD = 0xff; //wylacz wszystkie rzedy
		suma0=0;
		for(uint8_t i=0;i<32;i++){
		 suma0=suma0+bufor[i]*pot;
		 pot<<=1;
		}
		SendSpi(suma0);
		PORTD = ~(1<<r1);
		break;

		case 1:
//		PORTD = 0xff;
		suma1=0;
		for(uint8_t i=32;i<64;i++){
		 suma1=suma1+bufor[i]*pot;
		 pot<<=1;
		}
		SendSpi(suma1);
		PORTD = ~(1<<r2);
		break;

		case 2:
//		PORTD = 0xff;
		suma2=0;
		for(uint8_t i=64;i<96;i++){
		 suma2=suma2+bufor[i]*pot;
		 pot<<=1;
		}
		SendSpi(suma2);
		PORTD = ~(1<<r3);
		break;

		case 3:
//		PORTD = 0xff;
		suma3=0;
		for(uint8_t i=96;i<128;i++){
		 suma3=suma3+bufor[i]*pot;
		 pot<<=1;
		}
		SendSpi(suma3);
		PORTD = ~(1<<r4);
		break;

		case 4:
//		PORTD = 0xff;
		suma4=0;
		for(uint8_t i=128;i<160;i++){
		 suma4=suma4+bufor[i]*pot;
		 pot<<=1;
		}
		SendSpi(suma4);
		PORTD = ~(1<<r5);
		break;

		case 5:
//		PORTD = 0xff;
		suma5=0;
		for(uint8_t i=160;i<192;i++){
		 suma5=suma5+bufor[i]*pot;
		 pot<<=1;
		}
		SendSpi(suma5);
		PORTD = ~(1<<r6);
		break;

		case 6:
//		PORTD = 0xff;
		suma6=0;
		for(uint8_t i=192;i<224;i++){
		 suma6=suma6+bufor[i]*pot;
		 pot<<=1;
		}
		SendSpi(suma6);
		PORTD = ~(1<<r7);
		break;

		case 7:
//		PORTD = 0xff;
		suma7=0;
		for(uint16_t i=224;i<256;i++){
		 suma7=suma7+bufor[i]*pot;
		 pot<<=1;
		}
		SendSpi(suma7);
		PORTD = (unsigned char)~(1<<r8);
		break;
	}

	}

	else{
			switch(nrwysw){

				case 0:
				PORTD = ~(1<<r1);
				SendSpi(przesuniecie((uint32_t)pgm_read_byte(&tabela1[znak1]),0-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela1[znak2]),6-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela1[znak3]),12-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela1[znak4]),18-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela1[znak5]),24-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela1[znak6]),30-przesow));
				break;

				case 1:
				PORTD = ~(1<<r2);
				SendSpi(przesuniecie((uint32_t)pgm_read_byte(&tabela2[znak1]),0-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela2[znak2]),6-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela2[znak3]),12-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela2[znak4]),18-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela2[znak5]),24-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela2[znak6]),30-przesow));
				break;

				case 2:
				PORTD = ~(1<<r3);
				SendSpi(przesuniecie((uint32_t)pgm_read_byte(&tabela3[znak1]),0-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela3[znak2]),6-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela3[znak3]),12-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela3[znak4]),18-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela3[znak5]),24-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela3[znak6]),30-przesow));
				break;

				case 3:
				PORTD = ~(1<<r4);
				SendSpi(przesuniecie((uint32_t)pgm_read_byte(&tabela4[znak1]),0-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela4[znak2]),6-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela4[znak3]),12-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela4[znak4]),18-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela4[znak5]),24-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela4[znak6]),30-przesow));
				break;

				case 4:
				PORTD = ~(1<<r5);
				SendSpi(przesuniecie((uint32_t)pgm_read_byte(&tabela5[znak1]),0-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela5[znak2]),6-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela5[znak3]),12-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela5[znak4]),18-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela5[znak5]),24-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela5[znak6]),30-przesow));
				break;

				case 5:
				PORTD = ~(1<<r6);
				SendSpi(przesuniecie((uint32_t)pgm_read_byte(&tabela6[znak1]),0-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela6[znak2]),6-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela6[znak3]),12-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela6[znak4]),18-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela6[znak5]),24-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela6[znak6]),30-przesow));
				break;

				case 6:
				PORTD = ~(1<<r7);
				SendSpi(przesuniecie((uint32_t)pgm_read_byte(&tabela7[znak1]),0-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela7[znak2]),6-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela7[znak3]),12-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela7[znak4]),18-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela7[znak5]),24-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela7[znak6]),30-przesow));
				break;

				case 7:
				PORTD = (unsigned char)~(1<<r8);
				SendSpi(przesuniecie((uint32_t)pgm_read_byte(&tabela8[znak1]),0-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela8[znak2]),6-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela8[znak3]),12-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela8[znak4]),18-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela8[znak5]),24-przesow)+przesuniecie((uint32_t)pgm_read_byte(&tabela8[znak6]),30-przesow));
				break;
			}
		}
		nrwysw++;
		if(nrwysw>7)nrwysw=0;


	}


