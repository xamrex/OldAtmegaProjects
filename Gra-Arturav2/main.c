/*
 * main.c
 *
 *  Created on: 2011-02-24
 *       Autor: XamreX
 *
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "LCD/lcd44780.h"
#include "IR_DECODE/ir_decode.h"
#include <avr/delay.h>
/***************** DEFINE **************************************/
#define LED1_PIN (1<<PA7)
#define LED1_ON PORTA&=~LED1_PIN
#define LED1_OFF PORTA|=LED1_PIN

#define LED2_PIN (1<<PC0)
#define LED2_ON PORTC&=~LED2_PIN
#define LED2_OFF PORTC|=LED2_PIN
/****************** DEKLARACJA FUNCKJI *************************/
void ekran1(void); // deklaracja funkcji ekran1
void ekran2(void); // deklaracja funkcji ekran2
void menu(void); // deklaracja funkcji menu

uint8_t wylosowana,liczba,flaga,flaga2,powtorzenie,i=0,b=0,wynik=0;

int main(void)
{
DDRA |=LED1_PIN;
DDRC |=LED2_PIN;
PORTC=0xff;
LED1_OFF;
/********** KONFIGURACJA TIMERA0 do wygenerowania przerwania ****************/
TCCR0 |=(1<<CS00)|(1<<CS02); // wlacamy tim 0 i prescalujemy przez 1024
TIMSK |=(1<<TOIE0);	// Zezwolenia na przerwania
TCCR0=0; // Stopujemy TIMER
/*********************** INICJALIZACJA WYSW I RC5 ***************************/
	lcd_init();	/* inicjalizacja LCD */
	ir_init();	/* inicjalizacja dekodowania IR */
	sei();	/* w³¹czamy globalne przerwania */

	uint8_t znak_strzalka_up[]={4,14,21,4,4,4,4,4};
	uint8_t znak_strzalka_down[]={4,4,4,4,4,21,14,4};


	lcd_defchar(0x80, znak_strzalka_up);
	lcd_defchar(0x81, znak_strzalka_down);


menu();

}

void menu(){
	while(1){

lcd_locate(0,12);
lcd_int(b);
lcd_int(command);
lcd_int(Ir_key_press_flag);
lcd_locate(1,15);
lcd_int(i);


/********************** NACISNIECKIE KLAWISZA UP or DOWN ******************/
		if(Ir_key_press_flag) {								  //Odbieranie kodu z pilota
			if (command==cursor_down){
				if(b==0) b=1;
				else i=1;
//				_delay_ms(200);
			}
			if (command==cursor_up){
				if(b==1) b=0;
				else i=0;
//				_delay_ms(200);
			}
		}
	Ir_key_press_flag=0;
wynik=i*10+b;
/********************** NACISNIECKIE KLAWISZA UP or DOWN ******************/

switch(wynik){
	case 0:
	lcd_locate(0,0);
	lcd_str("\x7e"); //wysw strzalki
	lcd_locate(0,1);
	lcd_str("Start Gry      ");
	lcd_locate(1,0);
	lcd_str(" Info           ");
if (command==OK){
	command=0;
	ekran1();
}

	break;

	case 1:
	lcd_locate(0,0);
	lcd_str(" Start Gry      ");
	lcd_locate(1,0);
	lcd_str("\x7e"); //wysw strzalki
	lcd_locate(1,1);
	lcd_str("Info           ");
	break;

	case 11:
	lcd_locate(0,0);
	lcd_str(" Info           ");
	lcd_locate(1,0);
	lcd_str("\x7e"); //wysw strzalki
	lcd_locate(1,1);
	lcd_str("Ustawienia     ");
	break;

	case 10:
	lcd_locate(0,0);
	lcd_str("\x7e"); //wysw strzalki
	lcd_locate(0,1);
	lcd_str("Info           ");
	lcd_locate(1,0);
	lcd_str(" Ustawienia     ");
	break;

	}
}
}

void ekran1(){
/********************* EKRAN POWITALNY ***************************************/
if (command !=OK){ //dodanie tego warunku, aby po wygranej grze nie otrzymywac tego komuniaktu
lcd_locate(0,0);
lcd_str("Wcisnij OK");
lcd_locate(1,0);
lcd_str("Aby wystartowac!");
}
	while(command !=OK){
	Ir_key_press_flag=0;
	}
//	wylosowana=TCNT1%99+1;	// Wylosuj zmienna (max 99)
	wylosowana=22;
	powtorzenie=0; 	// Wykasowanie powtorzenia na 0
	liczba=0;		//wykasowanie liczby
	flaga2=0; 		// wykasowanie flagi 2
	ekran2();
}
/****************** EKRAN POWITALNY  KONIEC*************************************/
/****************** G£ÓWNA FUNKCJA START ***************************************/
void ekran2(){
uint8_t tak=0;
lcd_cls();
lcd_locate(0,0);
lcd_str("Kliknij klawisz");
Ir_key_press_flag=0;

	while(1){
		if (flaga==0) LED2_ON;
		if (flaga==1) LED2_OFF;

		if(Ir_key_press_flag) {
			LED1_ON;_delay_ms(200);LED1_OFF; // po odebraniu kodu z pilota mrugnij dioda

			if (command >9) Ir_key_press_flag=0;
			if (command >= 0 && command <=9){   // Jesli wybierzemy cyfre

			Ir_key_press_flag=0;
/***************** Sprawdzanie liczb START ************************/
				if (flaga==1){ // Liczby 2 cyfrowe
					liczba = liczba * 10;
					liczba = liczba + command;
					flaga = 0;
				}
				else { // Liczba 1 cyfrowe
					TCCR0 |=(1<<CS00)|(1<<CS02); //wlacamy TIMER
					liczba = command;
					flaga = 1;
					flaga2=1; // Potrzebe tylko do startu aby nie wsyweitlac na poczatku 0
					powtorzenie++;

					lcd_locate(0,0);
					lcd_str("Wybrales:");
					lcd_int(command);
					lcd_str("_     ");

				}
			}
		}


/*************** Sprawdzanie liczb KONIEC ************************/
			if (flaga==0 && flaga2==1) {
			lcd_locate(0,0);
			lcd_str("Wybrales:");
			lcd_int(liczba);
			lcd_str("      ");
/***************** Jesli za duzo *******************************/
				if (liczba>wylosowana){
				lcd_locate(1,0);
				lcd_str("Za duzo");
			}
/***************** Jesli za malo *******************************/
			if (liczba<wylosowana){
				lcd_locate(1,0);
				lcd_str("Za malo");
			}
/***************** Jesli zgadles *******************************/
			if (liczba==wylosowana){
				lcd_locate(1,0);
				lcd_str("Zgadles za ");
				lcd_int(powtorzenie);
				lcd_str("x");

				_delay_ms(2000);
				lcd_cls();
				lcd_locate(0,0);
				lcd_str("Grac dalej?");

				lcd_locate(1,0);
				lcd_str("[TAK]");
				lcd_locate(1,11);
				lcd_str(" NIE");

				while(command!=OK){

				tak=1;
				Ir_key_press_flag=0;

				if (command==cursor_left) {
					lcd_locate(1,0);
					lcd_str("[TAK]");
					lcd_locate(1,11);
					lcd_str(" NIE  ");
					if(command==OK) tak=1;

				}
				if (command==cursor_right) {
					lcd_locate(1,0);
					lcd_str(" TAK ");
					lcd_locate(1,11);
					lcd_str("[NIE]");
					if(command==OK) tak=2;
				}

				}

				if (tak==1) {
					command=0;
					ekran1();
				}
				if (tak==2) {
					command=0;
					menu();
				}


					}

			}

			}


}

/************************ WYSTAPIENIE PRZXERWANIA *************/

ISR(TIMER0_OVF_vect){
static uint8_t liczbat;
   liczbat = liczbat+1;
      if (liczbat==60){	//odliczanie 2sek
    	 flaga=0;
         liczbat=0;
         TCCR0=0; // Kasujemy TIMER
         TCCR0 &=~((1<<CS00)|(1<<CS01)|(1<<CS02));// Stopujemy TIMER
      }
}





