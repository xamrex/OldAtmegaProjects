// -----------------------------------------
// ----------------------------------------

#include <avr/io.h>
#include <inttypes.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <avr/interrupt.h>
//#include <avr/signal.h>
#include "usart.h"
/* 
-----------------------------------
// UBRR = ( F_CPU / (16 * BAUD) ) - 1
#define USART_UBRR 25 // UBRR=25 => FCPU=4MHz & BAUD=9600
*/

uint8_t usart_CharTab[16] PROGMEM = { 
											0x30, 0x31, 0x32, 0x33, 
											0x34, 0x35, 0x36, 0x37, 
											0x38, 0x39, 0x41, 0x42, 
											0x43, 0x44, 0x45, 0x46 };


void usart_Init(void) {
	UDR = 0x00; // reset data in udr
	UBRRH = 0;
	UBRRL = USART_UBRR; // Assign UBRRL
	UCSRB = (1 << RXEN) | (1 << TXEN); // Enable Tx & Rx
	UCSRC = (1<<URSEL) | (1 << UCSZ0) | (1 << UCSZ1); // Frame 8bit
}

void usart_SendChar(uint8_t c) {
	// wait until UDR will be empty
	while ( ! (UCSRA & (1 << UDRE) ) ) { }
	// insert char into UDR (this oper. sends char)
	UDR = c;
}

void usart_SendStr(char* str) {
	char znak;
	while(0 != (znak = *(str++))) usart_SendChar(znak);
}

void usart_SendStrP(prog_char* str) {
	char znak;
	while (0 != (znak = pgm_read_byte(str++))) usart_SendChar(znak);
}

void usart_SendDec(int val) {
	char bufor[7];
	usart_SendStr(itoa(val, bufor, 10));
}

uint8_t usart_DecToHex(uint8_t z) {
	z &= 0x0f;
	return pgm_read_byte(&usart_CharTab[z]);
}

void usart_SendHexByte(uint8_t data) {
	usart_SendChar(usart_DecToHex((data >> 4) & 0x0f));
	usart_SendChar(usart_DecToHex(data & 0x0f));
}

void usart_SendBinByte(uint8_t bin) {
	uint8_t i;
	for (i=0;i<8;i++) {
		if (bin & 0x80) usart_SendChar('1'); else usart_SendChar('0');
		bin <<= 1;
	}
}
