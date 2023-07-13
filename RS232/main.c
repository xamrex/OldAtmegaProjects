/*
 * main.c
 *
 *  Created on: 2011-03-07
 *       Autor: Miros³aw Kardaœ
 */
#include <avr/io.h>
#include <avr/delay.h>

void USART_Init (uint32_t baud);
void USART_Transmit(unsigned char data);

int main(void){


USART_Init(9600);

while(1){
	USART_Transmit('A');
	USART_Transmit('R');
	USART_Transmit('T');
	USART_Transmit('U');
	USART_Transmit('R');
	USART_Transmit(0x0d);
	USART_Transmit(0x0a);


}
}

void USART_Init (uint32_t baud){
uint16_t _ubr = (F_CPU/16/baud-1);
UBRRH = (uint8_t)(_ubr>>8);
UBRRL = (uint8_t)_ubr;
UCSRB=(1<<RXEN)|(1<<TXEN);
UCSRC=(1<<URSEL)|(3<<UCSZ0);
}

void USART_Transmit(unsigned char data){
while(!(UCSRA & (1<<UDRE)));
UDR=data;
}
