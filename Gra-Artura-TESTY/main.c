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
void info(void); // deklaracja funkcji info
uint8_t wylosowana,liczba,flaga,flaga2,powtorzenie,pozycja;

int main(void)
{
_delay_ms(100);
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
//ekran1();
}

void menu(){

lcd_locate(0,0);
lcd_str("  MENU GLOWNE  ");


	while (1){

/********************** NACISNIECKIE KLAWISZA UP or DOWN ******************/
		if(Ir_key_press_flag) {								  //Odbieranie kodu z pilota
			if (command==cursor_down && pozycja<2){				//Dodanie tego warunku, aby pozycja by³a max 2
				pozycja++;
				_delay_ms(200);
			}
			if (command==cursor_up && pozycja>0){				//Dodanie tego warunku, aby pozycja by³a min 0
				pozycja--;

			}

		}
		Ir_key_press_flag=0;
/********************** NACISNIECKIE KLAWISZA UP or DOWN ******************/
switch(pozycja){
	case 0: // Start GRY
	lcd_locate(1,0);
	lcd_str("1)Start Gry     ");
	if(Ir_key_press_flag) {
		if(command==OK || command==cursor_right){
			ekran1();
		}
	}
	break;

	case 1: //INFO
	lcd_locate(1,0);
	lcd_str("2)Info          ");
	if(Ir_key_press_flag) {
		if(command==OK || command==cursor_right){
			info();
		}
	}
	break;

	case 2: //Ustawienia
	lcd_locate(1,0);
	lcd_str("3)Ustawienia    ");
	break;
}

	}
}


void ekran1(){
/********************* EKRAN POWITALNY ***************************************/
lcd_cls();
if (command !=23){ //dodanie tego warunku, aby po wygranej grze nie otrzymywac tego komuniaktu
lcd_locate(0,0);
lcd_str("Wcisnij OK");
lcd_locate(1,0);
lcd_str("Aby wystartowac!");
}
	while(1){


		if(Ir_key_press_flag) {
			LED1_ON;_delay_ms(200);LED1_OFF;
				if(command==23){		// Jeœli naciœniemy OK
					wylosowana=TCNT1%99+1;	// Wylosuj zmienna (max 99)
					//wylosowana=22;
					powtorzenie=0; 	// Wykasowanie powtorzenia na 0
					liczba=0;		//wykasowanie liczby
					flaga2=0; 		// wykasowanie flagi 2
					ekran2();
				}

		}
	Ir_key_press_flag=0;// musi byc na koncu while'a
	}
}
/****************** EKRAN POWITALNY  KONIEC*************************************/
/****************** G£ÓWNA FUNKCJA START ***************************************/
void ekran2(){
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

					while(1){
						if(Ir_key_press_flag) {
						if(command==23){
							ekran1();
						}
						Ir_key_press_flag=0;
						}

					}

			}

			}

	}
}
/************************ DODATKOWE EKRANY ********************/
void info(){
lcd_cls();
while(1){
	if(Ir_key_press_flag) {								  //Odbieranie kodu z pilota
				if (command==cursor_left){
					pozycja=1;
					menu();

				}
	}
Ir_key_press_flag=0;
lcd_locate(0,0);
lcd_str("Program napisal");
lcd_locate(1,0);
lcd_str("   **XamreX**   ");
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



