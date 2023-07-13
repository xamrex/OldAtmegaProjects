/*
 * main.c
 *
 *  Created on: 2010-04-05
 *       Autor: Miros³aw Kardaœ
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

volatile uint8_t pwm1, pwm2, pwm3, pwm4, pwm5, pwm6;
// g³ówna funkcja programu main()
int main(void)
{
	//***** SPRZÊTOWY PWM - 1 KANA£ OC0 (PB3) *******
	// ustawienie koñcówki OC0 (PB3) sprzêtowy PWM jako WYJŒCIE
	// ustawienia TIMER2 w Fast PWM
	TCCR0 |= (1<<WGM01)|(1<<WGM00);
	TCCR0 |= (1<<COM01);
	TCCR0 |= (1<<CS00);
OCR0=255;
while(1){

}
}


ISR(TIMER2_OVF_vect)
{
static uint16_t cnt; //definicja licznika PWM
TCNT2=56;
if(cnt>=pwm1) PORTC |=(1<<PC0); else PORTC &=~(1<<PC0);
if(cnt>=pwm2) PORTC |=(1<<PC1); else PORTC &=~(1<<PC1);
if(cnt>=pwm3) PORTC |=(1<<PC2); else PORTC &=~(1<<PC2);
cnt++;
if(cnt>511) cnt=0;
}





