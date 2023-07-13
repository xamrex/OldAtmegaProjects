#include <avr/io.h>
#include <avr/eeprom.h>
#include "LCD/lcd44780.h"

// poni¿sz¹ linijkê czasami trzeba wpisaæ w eclipse przed definicjami
// zmiennych w pamiêci EEPROM, ¿eby nie podkreœla³ sk³adni jako b³êdnej
#define EEMEM __attribute__((section(".eeprom")))
char EEMEM tab2[8];
char txt[20];


int main(void)
{
	eeprom_write_block("qqqq",tab2,4);
	eeprom_read_block(txt,tab2,6);

	lcd_init();

	lcd_str_E(tab2);			// napis z pamiêci EEPROM
	lcd_locate(1,0);
	lcd_str(txt);				// napis z pamiêci RAM

	while(1);
}

