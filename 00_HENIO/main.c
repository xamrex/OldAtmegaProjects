/*
 * main.c
 *
 *  Created on: 2010-03-30
 *       Autor: Miros³aw Kardaœ
 */
#include <avr/io.h>		// do³¹czenie g³ównego systemowego  pliku nag³ówkowego
#include <avr/interrupt.h>
#include <util/delay.h>
#include "I2C_SOFT/i2c_soft.h"
#include "d_led.h"		// do³¹czenie naszego pliku nag³ówkowego (obs³uga LED)
#include "1Wire/ds18x20.h"

#define key1 (1<<PC0)
#define key1_down !(PINC & key1)

#define key2 (1<<PC1)
#define key2_down !(PINC & key2)

#define dioda (1<<PC2)
void display_temp(uint8_t x);
uint8_t czujniki_cnt;		/* iloœæ czujników na magistrali */
uint8_t subzero, cel, cel_fract_bits;
/*******************************************************************************/
volatile uint8_t czasTimera;
volatile uint8_t flaga=1;
/*******************************************************************************/

int main(void)
{

	DDRC=0xff;  //to pokazuje przerwanie

	d_led_init();
	i2c_init();
	czujniki_cnt = search_sensors(); //wysylamy rozkaz wyszukania wszystkich czujnikow
 /************************* USTAWIENIE 2 przyciskow i 1 diody *************************/
	DDRC &=~key1; //key1 jako wejscie
	DDRC &=~key2; //key2 jako wejscie

	PORTC |=key1; //podciagnienicie do VCC
	PORTC |=key2; //podciagnienicie do VCC

	DDRC |= dioda; //dioda jako wyjscie
	PORTC &=~dioda; // ustawienie stanu niskiego na niej
 /************************* USTAWIENIE 2 przyciskow i 1 diody *************************/

	//zapisgodziny(19);
	//zapisminuty(5);
	//zapissekundy(10);

/*********** PRZERWANIE INT2 *************************************************************************/
	MCUCR |= (1<<ISC01);	// wyzwalanie zboczem opadaj¹cym
	GICR |= (1<<INT0);		// odblokowanie przerwania
	DDRD &=~(1<<PD2);		 //INT0 jako wejscie
	PORTD |= (1<<PD2);		// podci¹gniêcie pinu INT0 do VCC
/******************** OBLUSGA TEMPERATURY ****************************/
sei();

		while(1)
		{
				if (flaga==1){
					cli();
					odczytczasu();     //odczytuj czas z RTC
					flaga=0;
					sei();
				}

				if (key1_down){
					if (key1_down){
					zapisminuty1();
				}
				_delay_ms(20);
			}

				if (key2_down){
					if (key2_down){
					zapisgodziny1();
				}
				_delay_ms(20);
			}



		if(((czasTimera>=0 && czasTimera <40) || (czasTimera>50 ))){
			PORTC &=~dioda; //zgas diode przecinkowa


				cy1=(bcd2dec(bufor[3])/10); //jedn godzin
				cy2=(bcd2dec(bufor[3])%10); // dz godzin
				cy3=(bcd2dec(bufor[2])/10);	//jedn min
				cy4=(bcd2dec(bufor[2])%10);	//dz min
				cy5=(bcd2dec(bufor[1])/10);	//dz sek
				cy6=(bcd2dec(bufor[1])%10); //jedn sek


			}

			if(czasTimera==40) DS18X20_start_meas( DS18X20_POWER_EXTERN, NULL ); //jesli czas zero to wysylaj rozkaz pomiaru
			if(czasTimera>40 && czasTimera<=50){
			if( DS18X20_OK == DS18X20_read_meas(gSensorIDs[0], &subzero, &cel, &cel_fract_bits) ) display_temp(0); //po 1 sek wyswietlaj ten pomiar
			}

		}
}

void display_temp(uint8_t x) {
PORTC |=dioda; //zalacz diode przecinkowa
cy1=10; //wygras pierwsza cyfre
if(subzero) cy1=11;   //jesli minus to na pierwszym miejscu daj minusa
cy2=cel/10;
cy3=cel%10;
cy4=cel_fract_bits; //wyswiet temp. po przecinku
cy5=12; //stopien
cy6=13; //litera C
}

// procedura obs³ugi przerwania INT 0

ISR( INT0_vect ) {
	flaga=1;

czasTimera++;		//zwiekszaj czas timera aby wysiwetlac przez 10 sek temperature
if (czasTimera==60) czasTimera=0;
}

