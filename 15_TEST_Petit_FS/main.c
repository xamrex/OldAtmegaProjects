#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

#include "PetitFS/diskio.h"
#include "PetitFS/pff.h"

#include "LCD/lcd44780.h"


char file_name[] = "plik.txt";
char bufor[16];
char bufor1[128];
char bufor2[128];
char bufor3[128];
/*-----------------------------------------------------------------------*/
/* Main                                                                  */
int main (void)
{
	DDRA |= (1<<PA7);
	PORTA |= (1<<PA7); /* podœwietlenie wyœwietlacza LCD */
	DIR Dir;   /* Directory object */
	BYTE res;
	WORD s1;
	FILINFO fno;

	FATFS fs;			/* File system object */
	//DIR dir;			/* Directory object */
	//FILINFO fno;		/* File information */

#define SCK PB7
#define MOSI PB5
#define CS PB4
	DDRB |= (1<<CS)|(1<<MOSI)|(1<<SCK)|(1<<CS);
	PORTB |= (1<<CS);
	SPCR |= (1<<SPE)|(1<<MSTR);

	lcd_init();

	lcd_str("odczyt: test.txt");
	lcd_locate(1,0);

	_delay_ms(100);

	res = disk_initialize();

	if( res == FR_OK ) {
		res = pf_mount(&fs);

		if( res == FR_OK ) {


		lcd_locate(1,0);
		pf_open("plik.txt");



		pf_read(bufor,16,&s1);
		lcd_locate(1,0);
		lcd_str(bufor);

		_delay_ms(2000);

		lcd_locate(1,0);
		pf_lseek(16);
		pf_read(bufor,16,&s1);
		lcd_locate(1,0);
		lcd_str(bufor);






		} else lcd_str("Read dir error");
	} else lcd_str("disk init error");

	while(1);

}
