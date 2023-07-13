/*KURS AVR-GCC cz.4
Program, z pomoc¹  buzzera (przetwornika piezo),
odgrywa krótk¹ melodyjkê.

uk³ad atmega 1MHz
PB0 -> R(330Ohm) -> BUZZER -> PB1
*/

#include <avr/io.h>
#include <util/delay.h>
#include "RS/rs232.h"
/**** ZMIENNE GLOBALNE ****/
/*
Tablica dzwiêków:
czêstotliwœæ(Hz), czas_trwania(ms), czêstotliwoœæ, ...
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

/**** DEFINICJE W£ASNYCH FUNKCJI ****/

/* Konfiguruje porty we/wy uC */
void init(void)
{
  /* PB0,PB1 - wyœcia */
	USART_init(19200); //inicjajca uarta
  DDRB  = 0x03;
  PORTB = 0x00;
}

/*
Funkcja generuje sygna³ prostok¹tny na wyprowadzeniach PB0 i PB1,
gdzie przy³¹czony jest  buzzer. Funkcja przyjmuje argumenty:
czêstotliwoœæ(Hz) sygna³u i du³goœæ czasu trwania sygna³u (ms).
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
Odgrywa melodyjkê dzwiêk po dzwiêku. Jako argumentów funkcja
oczekuje tablicy dzwiêków oraz numerów pierwszego i ostatniego
dzwiêku. Elementami tablicy dŸwiêków  s¹ tablice o dwóch elementach
typu int (czêstotliwoœæ w Hz i d³ugoœæ trwania dzwiêku w ms).
*/
void play(int nots[][2], unsigned int start, unsigned int stop)
{
   int n;

   for(n=start; n <= stop; n++)
        beep(nots[n][0], nots[n][1]);
}

/**** POCZ¥TEK PROGRAMU ****/

/* Definicja g³ównej funkcji */
int main(void)
{
char zmienna;
   /* Konfiguracja sprzêtu */
   init();

   /* Nieskoñczona pêtla */
   while (1)
   {
	   zmienna=USART_Recive();

		if (zmienna=='1'){
		    play(koziolek,0,14);
			}




   }
   return 0;
}
