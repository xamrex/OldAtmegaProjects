/* ***********************************************************************
**
**  Copyright (C) 2005  Christian Kranz
**
**  Siemens S65 Display Control for L2F50 Display with Epson Controller
**
*********************************************************************** */
/*********************************************
* Chip type           : ATMEGA32
* Clock frequency     : clock 16 MHz
*********************************************/
#include <avr/io.h>
#include <math.h>
#include "RS/rs232.h"
#include "disp.h"
#include "colors_rgb565.h"
#include "stdlib.h"

#define SQRT3 1.7320508
#define SR 65

uint8_t nr[8]; //tablica przechowujaca znaki do stringa
char tablica[3]; //4 bity w 16kowym zapisie
uint16_t liczba;
uint16_t licznik;
char *endptr; //wskaxnik do funkcji strotl
uint8_t flaga,bak;

int x=66,y=88,dlugosc=1;
int kierunek=1;
int *pkierunek=&kierunek;
int *pdlugosc = &dlugosc;
int *px=&x;
int *py=&y;


void uart_puts(char *s)		// wysy³a ³añcuch z pamiêci RAM na UART
{
  register char c;
  while ((c = *s++)) USART_Transmit(c);			// dopóki nie napotkasz 0 wysy³aj znak
}

void uart_putint(int value, int radix)	// wysy³a na port szeregowy tekst
{
	char string[17];			// bufor na wynik funkcji itoa
	itoa(value, string, radix);		// konwersja value na ASCII
	uart_puts(string);			// wyœlij string na port szeregowy
}


void line2(int *kierunek, int *pdlugosc,int *x, int *y){



	if(*pdlugosc<120){

		if(*kierunek==60){
				LcdLine(*x,*x+0.5*(*pdlugosc),*y,*y+0.5*dlugosc*SQRT3,COLOR_BLACK);
				*x=*x+0.5*(*pdlugosc);
			}

	//////////////////////////////////
		if(*kierunek==1){
			LcdLine(*x,*x+*pdlugosc,*y,*y,COLOR_BLACK);
			*x=*x+*pdlugosc;
		}
		if(*kierunek==2){
			LcdLine(*x,*x,*y,*y+*pdlugosc,COLOR_BLACK);
			*y=*y+*pdlugosc;
		}
		if(*kierunek==3){
			LcdLine(*x,*x-*pdlugosc,*y,*y,COLOR_BLACK);
			*x=*x-*pdlugosc;
		}
		if(*kierunek==4){
			LcdLine(*x,*x,*y,*y-*pdlugosc,COLOR_BLACK);
			*y=*y-*pdlugosc;
		}
		*pdlugosc=*pdlugosc+2;
		*kierunek=*kierunek+1;
		if(*kierunek>4)	*kierunek=1;
		kolo(*x,*y,2,COLOR_RED);
		line2(pkierunek,pdlugosc,px,py);


	}
}




int main(void)
{
/************************************TIMER*************************************/



/************************************TIMER*************************************/
  port_init();
  lcd_init_c();
  fill_screen(COLOR_WHITE);
//  USART_init(1250000);

 // kolof(44,44,33,COLOR_BLUE);
  //LcdLine(0,90,5,120,COLOR_BLACK);
//LcdRect(0,70,0,80,COLOR_RED);
//kolofc(65,87,64);
 // print_str(0,0,"Forum.atnel.pl",COLOR_ALICE_BLUE,COLOR_RED);
  //ubi2dec(666,nr,0);
 //print_str(0,0,nr,COLOR_AQUA,COLOR_RED);
 // obrazek();



//line2(pkierunek,pdlugosc,px,py);
LcdLine(0,6*SQRT3,1,1,COLOR_BLACK);



//LcdLine(0,30,0,0,COLOR_BLACK);
//LcdLine(30,30,0,30,COLOR_BLACK);
//LcdLine(30,5,30,30,COLOR_BLACK);



//
/**************** wysiwqetlanie**************************************/
 /*
while(1){

PORTB &= ~_BV(LCD_CS);  // select display

 lcd_cmd(SD_CSET);
 lcd_dat0(0x08);  // start is 8, not 0
 lcd_dat0(0x01);
 lcd_dat0(0x8B);  // is 00x8B = 0x83+8
 lcd_dat0(0x01);

 lcd_cmd(SD_PSET);
 lcd_dat0(0x00);
 lcd_dat0(0xAF);

 lcd_cmd(RAMWR);
 PORTB |= _BV(LCD_CS);  // deselect display



		for(uint16_t i=0;i<(DISP_W*DISP_H);i++){

			tablica[0]=USART_Recive();
			tablica[1]=USART_Recive();
			liczba=(tablica[1]*256+tablica[0]);

				PORTB &= ~_BV(LCD_CS);  // select display
				lcd_dat16(liczba);
			PORTB |= _BV(LCD_CS);  // deselect display
		}
	PORTB |= _BV(LCD_CS);  // deselect display
}
*/


}


