#include <avr/io.h>
#include <avr/eeprom.h>
#include "LCD/lcd44780.h"

// poni�sz� linijk� czasami trzeba wpisa� w eclipse przed definicjami
// zmiennych w pami�ci EEPROM, �eby nie podkre�la� sk�adni jako b��dnej
#define EEMEM __attribute__((section(".eeprom")))
char EEMEM tab2[8];
char txt[20];


int main(void)
{
	eeprom_write_block("qqqq",tab2,4);
	eeprom_read_block(txt,tab2,6);

	lcd_init();

	lcd_str_E(tab2);			// napis z pami�ci EEPROM
	lcd_locate(1,0);
	lcd_str(txt);				// napis z pami�ci RAM

	while(1);
}

