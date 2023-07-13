/*KURS AVR-GCC cz.4
Program, z pomoc�  buzzera (przetwornika piezo),
odgrywa kr�tk� melodyjk�.

uk�ad atmega 1MHz
PB0 -> R(330Ohm) -> BUZZER -> PB1
*/

#include <avr/io.h>
#include <util/delay.h>
#include "RS/rs232.h"
/**** ZMIENNE GLOBALNE ****/
/*
Tablica dzwi�k�w:
cz�stotliw��(Hz), czas_trwania(ms), cz�stotliwo��, ...
*/
int koziolek[15][2]={ 523,125,
                    587,125,
                    659,250,
                    698,125,
                    659,125,
                    587,250,
                    523,250,
                    1047,250,
                    784,250,
                    523,250,
                    1047,250,
                    784,250,
                    523,250,
                    1047,250,
                    784,1000 };

/**** DEFINICJE W�ASNYCH FUNKCJI ****/

/* Konfiguruje porty we/wy uC */
void init(void)
{
  /* PB0,PB1 - wy�cia */
	USART_init(19200); //inicjajca uarta
  DDRB  = 0x03;
  PORTB = 0x00;
}

/*
Funkcja generuje sygna� prostok�tny na wyprowadzeniach PB0 i PB1,
gdzie przy��czony jest  buzzer. Funkcja przyjmuje argumenty:
cz�stotliwo��(Hz) sygna�u i du�go�� czasu trwania sygna�u (ms).
*/
void beep(unsigned int frequency, unsigned int duration)
{
  unsigned int i,t,n;
  t = 125000/frequency;
  n = (250UL*duration)/t;

  PORTB |= 0x01;
  PORTB &= ~0x02;
  for(i=0; i < n; i++)
  {
    PORTB ^= 0x01;
    PORTB ^= 0X02;
    _delay_loop_2(t);
  }
}
/*
Odgrywa melodyjk� dzwi�k po dzwi�ku. Jako argument�w funkcja
oczekuje tablicy dzwi�k�w oraz numer�w pierwszego i ostatniego
dzwi�ku. Elementami tablicy d�wi�k�w  s� tablice o dw�ch elementach
typu int (cz�stotliwo�� w Hz i d�ugo�� trwania dzwi�ku w ms).
*/
void play(int nots[][2], unsigned int start, unsigned int stop)
{
   int n;

   for(n=start; n <= stop; n++)
        beep(nots[n][0], nots[n][1]);
}

/**** POCZ�TEK PROGRAMU ****/

/* Definicja g��wnej funkcji */
int main(void)
{
char zmienna;
   /* Konfiguracja sprz�tu */
   init();

   /* Niesko�czona p�tla */
   while (1)
   {
	   zmienna=USART_Recive();

		if (zmienna=='1'){
		    play(koziolek,0,14);
			}




   }
   return 0;
}
