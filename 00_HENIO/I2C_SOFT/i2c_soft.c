/*
 * i2c_twi.c
 *
 *  Created on: 2010-09-07
 *       Autor: Miros�aw Karda�
 */
#include <avr/io.h>
#include <util/delay.h>


#include "i2c_soft.h"


void I2C_START (void) {
		I2C_SDL_LO;
		QDEL;
		I2C_SCL_LO;
}

void I2C_REP_START (void) {
	HDEL;
	I2C_SCL_HI;
	I2C_SDL_LO;
	QDEL;
	I2C_SCL_LO;
}

void I2C_STOP(void) {
	I2C_SDL_LO;
	HDEL;
	I2C_SCL_HI;
	QDEL;
	I2C_SDL_HI;
	HDEL;
}

u08 i2cPutbyte(u08 b) {
	u08 i=0x80;

	do {
		if ( b & i ) I2C_SDL_HI;
		else I2C_SDL_LO;

		I2C_SCL_TOGGLE;
	} while ( i >>= 1 );		// wysy�anie bajtu

	I2C_SDL_HI;			// pozostawienie SDA w stanie wysokim

	DDR(SDAPORT) &= ~(1<<SDA);
	HDEL;
	I2C_SCL_HI;
	b = PIN(SDAPORT) & (1<<SDA);	// sprawdzenie bitu ACK

	HDEL;
	I2C_SCL_LO;
	DDR(SDAPORT) |= (1<<SDA);
	HDEL;
	return (b == 0);	//zwr�cenie ACK jako rezultatu funkcji
}


u08 i2cGetbyte(uint8_t ack) {
	u08 i;
	u08 c,b = 0;

	I2C_SDL_HI;
	DDR(SDAPORT) &= ~(1<<SDA);

	for(i=8;i>0;i--) {
		HDEL;
		I2C_SCL_HI;
		c = PIN(SDAPORT) & (1<<SDA);
		b <<= 1;
		if(c) b |= 1;
		HDEL;
		I2C_SCL_LO;
	}

	DDR(SDAPORT) |= (1<<SDA);

	if (ack) I2C_SDL_LO;		// wy�lij ACK
	else I2C_SDL_HI;		// wy�lij NAK

	I2C_SCL_TOGGLE;
	I2C_SDL_HI;
	return b;
}

// Inicjalizacja komunikacji I2C
void i2c_init(void) {
	DDR(SDAPORT) |= (1<<SDA);
	DDR(SCLPORT) |= (1<<SCL);
	I2C_SDL_HI;
	I2C_SCL_HI;
}

// Wys�anie sekwencji bajt�w z bufora [buf] do urz�dzenia [device] pod adres [adr]
void I2C_write_buf(u08 device, u08 adr, u08 len, u08 *buf) {

	I2C_START();      		// i2c START
	i2cPutbyte(device); 		// wys�anie adresu urz�dzenia
	i2cPutbyte(adr);		// wys�anie adresu pami�ci do zapisu

	// wys�anie danych
	while (len--)
		i2cPutbyte(*buf++);

	I2C_STOP();			// i2c STOP
}

void godzina(u08 buf) {

	I2C_START();      		// i2c START
	i2cPutbyte(0xA2); 		// wys�anie adresu urz�dzenia
	i2cPutbyte(0x04);		// wys�anie adresu pami�ci do zapisu

	// wys�anie danych

	i2cPutbyte(buf);

	I2C_STOP();			// i2c STOP
}

// Odebranie sekwencji bajt�w do bufora [buf] z urz�dzenia [deice] spod adresu [adr]
void I2C_read_buf(u08 device, u08 adr, u08 len, u08 *buf) {

	I2C_START();			// i2c START
	i2cPutbyte(device);		// wys�anie adresu urz�dzenia
	i2cPutbyte(adr);   		// wys�anie adresu pami�ci do odczytu

	I2C_REP_START();		// repeated START

	// wys�anie adresu urz�dzenia z ustawieniem bitu odczytu
	i2cPutbyte(device | 1);

	// odbieranie danych
	while (len--) *buf++ = i2cGetbyte(len ? 1 : 0);

	I2C_STOP();			// i2c STOP
}



// konwersja liczby dziesi�tnej na BCD
uint8_t dec2bcd(uint8_t dec) {
return ((dec / 10)<<4) | (dec % 10);
}

// konwersja liczby BCD na dziesi�tn�
uint8_t bcd2dec(uint8_t bcd) {
    return ((((bcd) >> 4) & 0x0F) * 10) + ((bcd) & 0x0F);
}

void odczytczasu(void){

	I2C_read_buf( 0xA2, 0x01, 4, bufor );


}
void zapisgodziny(uint8_t dana){
	I2C_START();      		// i2c START
		i2cPutbyte(0xA2); 		// wys�anie adresu urz�dzenia
		i2cPutbyte(0x04);		// wys�anie adresu pami�ci do zapisu (godzina)
		i2cPutbyte(dec2bcd(dana));
		I2C_STOP();			// i2c STOP
}



void zapisminuty(uint8_t dana){
	I2C_START();      		// i2c START
		i2cPutbyte(0xA2); 		// wys�anie adresu urz�dzenia
		i2cPutbyte(0x03);		// wys�anie adresu pami�ci do zapisu (minuta)
		i2cPutbyte(dec2bcd(dana));
		I2C_STOP();			// i2c STOP
}
void zapisminuty1(void){
uint8_t pomoc=bcd2dec(bufor[2]);
pomoc++;
if (pomoc==60) pomoc=0;
zapisminuty(pomoc);
zapissekundy(0); //zeruj sekundy
}
void zapisgodziny1(void){
uint8_t pomoc=bcd2dec(bufor[3]);
pomoc++;
if (pomoc==24) pomoc=0;
zapisgodziny(pomoc);

}

void zapissekundy(uint8_t dana){
	I2C_START();      		// i2c START
		i2cPutbyte(0xA2); 		// wys�anie adresu urz�dzenia
		i2cPutbyte(0x02);		// wys�anie adresu pami�ci do zapisu (sekunda)
		i2cPutbyte(dec2bcd(dana));
		I2C_STOP();			// i2c STOP
}
