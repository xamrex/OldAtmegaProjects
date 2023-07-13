/*
** main.c
**
** Nokia 3310 Example Program
** Target: ATMEGA :: AVR-GCC
**
** Written by Tony Myatt - 2007
** Quantum Torque - www.quantumtorque.com
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include "lcd.h"

/* main -- Program starts here */
int main(void)
{
	// Setup LCD
	lcd_init();
	lcd_contrast(90);
	lcd_goto_xy(1,1);

	char bufor[11];
	float val=-122.6;
	dtostrf(val,6,1,bufor);
	lcd_str(bufor);

	//LcdPixel(84,48,PIXEL_ON);


	//LcdSingleBar(30,30,15,10,PIXEL_XOR);
	//LcdRect(1,84,1,48,PIXEL_ON);
	//LcdLine(1,84,1,48,PIXEL_OFF);
	//LcdLine(1,84,48,1,PIXEL_OFF);
	//LcdUpdate();
	_delay_ms(10);


	//lcd_str("aa");
	//lcd_goto_xy_exact(1,1);
//	LcdSend(0xff, LCD_DATA);
	while(1){
	//	obrazek1();
	}
}
