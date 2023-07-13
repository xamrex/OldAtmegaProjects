/*
 * rs232.c
 *
 *  Created on: 2011-08-17
 *       Autor: XamreX
 */

#include <avr/io.h>

void USART_Transmit (unsigned char data){
	while(!(UCSRA & (1<<UDRE)));
	UDR=data;
}
void USART_napis (char * str){
register char c;
while ((c = *str++)) USART_Transmit(c);

}

unsigned char USART_Recive(void){
	while(!(UCSRA & (1<<RXC)));
	return UDR;
}

void USART_init(uint32_t baud){
	uint16_t _ubr = (F_CPU/16/baud-1); //dlaU2X=0
	/* ustawianie predkosci */
	UBRRH=(uint8_t)(_ubr>>8);
	UBRRL=(uint8_t)(_ubr);
	/* za³aczenie nadajnika i odbiornika */
	UCSRB=(1<<RXEN)|(1<<TXEN);
	UCSRC=(1<<URSEL)|(3<<UCSZ0);
}
