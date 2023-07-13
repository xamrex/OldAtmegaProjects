/*
 * mian.c
 *
 *  Created on: 2011-02-26
 *       Autor: XamreX
 */
#include <avr/io.h>
#include <avr/interrupt.h>

#define LED1_PIN (1<<PA7)
#define LED1_ON PORTA&=~LED1_PIN
#define LED1_OFF PORTA|=LED1_PIN
uint8_t flaga;
int main(void){
DDRA |=LED1_PIN;
flaga=1;
while (1){
		if (flaga==0) LED1_ON;
		if (flaga==1) LED1_OFF;
	}

}
