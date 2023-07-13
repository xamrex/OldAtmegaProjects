#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "matryca.h"
#include "RS/rs232.h"




int main(void){

coileprzesuw=30;	// zmienna ktora przesuwa nam tekst co jakis czas

	InitSpi();
	InitTimer();
	USART_init(9600);
	porty_init();
	skopiuj(); //kopiujemy z eeprom na tablice ram

	sprawdzcfg(); //czy tryb seriwisowy?

	//napis("JAKIS BARDZO DLUGI NAPIS");
	//dlugosc();


//sprintf(txt,"DZISIAJ %i",zmienna);
//dlugosc();


//	int i =32;
//				bufor[0+i]=1;
//					bufor[2+i]=1;
//					bufor[32+i]=1;
//					bufor[33+i]=1;
//					bufor[34+i]=1;
//					bufor[35+i]=1;
//					bufor[64+i]=1;
//					bufor[66+i]=1;

int kierunek=0;
int x,y;
int over=0;
x=16,y=4;
lastx=x;
lasty=y;
int lastx=0;int lasty=0;
int ox=4;int oy=4;
pixel(ox,oy);
kierunek=3;
		while(!over)
		{
			if(kierunek==1){
				pixel(y,x);
				if(!(x==ox || y==oy)){
					pixelc(y,x-1);
					pixel(7,7);
				}
				++x;
				if(x==33){over=1;}

				_delay_ms(500);
			}else if (kierunek==3){
				pixel(y,x);
				if(!(x==ox && y==oy)){
					pixelc(y,x+1);

				}else{
					pixel(7,7);

				}

				--x;
				if(x==0){over=1;}
				_delay_ms(500);
			}
			else if (kierunek==2){
				pixel(y,x);
				pixelc(y+1,x);
				--y;
				if(y==0){over=1;}
				_delay_ms(500);
			}else if (kierunek==4){
				pixel(y,x);
				pixelc(y-1,x);
				++y;
				if(y==9){over=1;}
				_delay_ms(500);
			}

		}
		while(1){pixel(2,2);}
}
