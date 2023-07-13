/*
 * main.c
 *
 *  Created on: 2010-03-30
 *       Autor: Miros³aw Kardaœ
 */
#include <avr/io.h>		// do³¹czenie g³ównego systemowego  pliku nag³ówkowego
#include <avr/interrupt.h>
#include <util/delay.h>
#include "d_led.h"		// do³¹czenie naszego pliku nag³ówkowego (obs³uga LED)
#include "LCD/lcd44780.h"
#include <avr/pgmspace.h>

#define KEY1_PIN (1<<PB0)
#define KEY1_DOWN (!(PINB & KEY1_PIN))
//////////////////////////////////////
#define KEY2_PIN (1<<PB1)
#define KEY2_DOWN (!(PINB & KEY2_PIN))
//////////////////////////////////////
#define KEY3_PIN (1<<PB2)
#define KEY3_DOWN (!(PINB & KEY3_PIN))
//////////////////////////////////////
#define KEY4_PIN (1<<PB3)
#define KEY4_DOWN (!(PINB & KEY4_PIN))
//////////////////////////////////////
#define KEY5_PIN (1<<PB4)
#define KEY5_DOWN (!(PINB & KEY5_PIN))
////////////////////////////////////////

int main(void)
{

	d_led_init();
	sei();
// LCD
	lcd_init();
int ca=1;
	lcd_int(20/ca);
	_delay_ms(100);
	ca--;
	lcd_int(20/ca);
// LCD

	DDRB = 0x00;  //PORT B JAKO WEJSCIE
	PORTB |= (KEY1_PIN|KEY2_PIN|KEY3_PIN|KEY4_PIN|KEY5_PIN);

DDRA=0xFF;
PORTA=0xFF;

#define LED1_PIN (1<<PA0)
#define LED2_PIN (1<<PA1)
#define LED3_PIN (1<<PA2)
#define LED4_PIN (1<<PA3)

#define LED1_ON PORTA &=~LED1_PIN
#define LED2_ON PORTA &=~LED2_PIN
#define LED3_ON PORTA &=~LED3_PIN
#define LED4_ON PORTA &=~LED4_PIN

#define LED1_OFF PORTA |=LED1_PIN
#define LED2_OFF PORTA |=LED2_PIN
#define LED3_OFF PORTA |=LED3_PIN
#define LED4_OFF PORTA |=LED4_PIN

int losowa;
int blad=0;
	volatile int a,b,c,d,suma;


_delay_ms(100);


cy1=5;
cy2=5;
cy3=5;
cy4=5;



while(1){

	// PRZYCISK 1
		if (KEY1_DOWN)
		{
			_delay_ms(180);
			cy1=cy1+1;
			if (cy1>9)cy1=0;
		}

	// PRZYCISK 2
		if (KEY2_DOWN)
		{
			_delay_ms(180);
			cy2=cy2+1;
			if (cy2>9)cy2=0;
		}

	// PRZYCISK 3
		if (KEY3_DOWN)
		{
			_delay_ms(180);
			cy3=cy3+1;
			if (cy3>9)cy3=0;
		}
	// PRZYCISK 4
		if (KEY4_DOWN)
		{
			_delay_ms(180);
				cy4=cy4+1;
			if (cy4>9)cy4=0;
		}



		if (KEY5_DOWN){
			losowa=(cy1*1000 + cy2*100 + cy3*10 + cy4);
			break;
		}

}
void wypisz(int ile) {
int roznica;
roznica=ile-suma;
if (roznica<0)
	roznica=roznica*(-1);

	a = ile / 1000;
    b = ile % 1000 / 100;
    c = ile % 100 / 10;
    d = ile % 10;

    if (cy1 - a < -5 || (cy1 - a > 0 && cy1 - a < 5)) {
        while (cy1 != a) {
            cy1--;
            if (cy1 < 0) cy1 = 9;
            _delay_ms(160);
        }
    } else {
        while (cy1 != a) {
            cy1++;
            if (cy1 == 10) cy1 = 0;
            _delay_ms(160);
        }
    }

    if (ile+roznica<(a+1)*1000 && ile-roznica>a*1000){
    	LED1_ON;
    }
    _delay_ms(300);

    if (cy2 - b < -5 || (cy2 - b > 0 && cy2 - b < 5)) {
        while (cy2 != b) {
            cy2--;
            if (cy2 < 0) cy2 = 9;
            _delay_ms(160);
        }
    } else {
        while (cy2 != b) {
            cy2++;
            if (cy2 == 10) cy2 = 0;
            _delay_ms(160);
        }
    }
    if (ile-a*1000+roznica<(b+1)*100 && ile-a*1000-roznica>b*100 ){
       	LED2_ON;
    }
    _delay_ms(300);
    if (cy3 - c < -5 || (cy3 - c > 0 && cy3 - c < 5)) {
        while (cy3 != c) {
            cy3--;
            if (cy3 < 0) cy3 = 9;
            _delay_ms(160);
        }
    } else {
        while (cy3 != c) {
            cy3++;
            if (cy3 == 10) cy3 = 0;
            _delay_ms(160);
        }
    }
    if (ile-a*1000-b*100+roznica<(c+1)*10 && ile-a*1000-b*100-roznica>c*10){
           	LED3_ON;
    }
    _delay_ms(300);
    if (cy4 - d < -5 || (cy4 - d > 0 && cy4 - d < 5)) {
        while (cy4 != d) {
            cy4--;
            if (cy4 < 0) cy4 = 9;
            _delay_ms(160);
        }
    } else {
        while (cy4 != d) {
            cy4++;
            if (cy4 == 10) cy4 = 0;
            _delay_ms(160);
        }
    }
    suma=a*1000+b*100+c*10+d;
}



void szukaj() {
    int min = 0, max = 9999, typoje;

    typoje = (min + max) / 2;
    while (typoje != losowa) {
        wypisz(typoje);
        if (typoje < losowa) {
            min = typoje;
            typoje = (min + max) / 2;
        } else {
            max = typoje;
            typoje = (min + max) / 2;
        }
    }
    wypisz(typoje);
    LED4_ON;
    cy1 = C;
    cy2 = O;
    cy3 = 0;
    cy4 = L;
    while (1);
}



	// ****** inicjalizacja *********
	// inicjalizacja wyœwietlacza multipleksowanego
	while(1){
// LOSOWANIE

//wypisz(1290);
szukaj();

		// w³¹czenie globalnego zezwolenia na przerwania
}






		//**********************  pêtla g³ówna
		}

