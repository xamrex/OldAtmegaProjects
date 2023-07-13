/*
 * lcd3310.c
 *
 *  Created on: 2011-05-20
 *       Autor: XamreX
 */

#include <avr/io.h>
#include <inttypes.h>

#include "makra.h"
#include "harddef.h"
#include "lcd3310.h"

// Wys³anie danych do LCD. Ustawieniem portów przejmujê siê przed wywo³aniem procedury. Tutaj programowa implementacja SPI.
void lcd_Send(uint8_t data)
{
#if defined LCD_HARDWARE_SPI
	PORT(LCD_PORT) &= ~(1<<LCD_SCE);
	SPDR = data;
	while(!(SPSR & 1<<SPIF)) {}
	PORT(LCD_PORT) |= 1<<LCD_SCE;
#else
	uint8_t n;
	// Upewnienie siê, ¿e SCK jest w stanie niskim
	PORT(LCD_PORT) &= ~(1<<LCD_SCK);
	// Aktywacja interfejsu
	PORT(LCD_PORT) &= ~(1<<LCD_SCE);
	// Do optymalizacji (assembler)
	for(n=8; n!=0; --n)
	{
		// Wyprowadzenie bitu
		if(data >= 128)
			PORT(LCD_PORT) |= 1<<LCD_SD;
		else
			PORT(LCD_PORT) &= ~(1<<LCD_SD);
		// Sygna³ zegarowy i przesuniêcie danej
		PORT(LCD_PORT) |= 1<<LCD_SCK;
		data <<= 1;
		PORT(LCD_PORT) &= ~(1<<LCD_SCK);
	}
	// Wy³¹czenie interfejsu
	PORT(LCD_PORT) |= 1<<LCD_SCE;
#endif
}

// Wys³anie komendy do wyœwietlacza
void lcd_Command(uint8_t command)
{
	PORT(LCD_PORT) &= ~(1<<LCD_DC);
	lcd_Send(command);
	// Po zakoñczeniu komendy wracam natychmiast do trybu danych
	PORT(LCD_PORT) |= 1<<LCD_DC;
}

// Inicjacja LCD
void lcd_Init(void)
{
	// Zerowanie. Wed³ug dokumentacji wystarczy 100ns. Na wszelki wypadek wprowadzam niewielkie opóŸnienie.
	PORT(LCD_PORT) &= ~(1<<LCD_RES);
	asm volatile("nop\n\t nop\n\t"::);
	PORT(LCD_PORT) |= 1<<LCD_RES;
	// Wysy³anie sekwencji startowej
	lcd_Command(LCDC_FS|LCDC_FS_EXTENDED); // Komendy rozszerzone
	lcd_Command(LCDC_VOP|72); // Kontrast
	lcd_Command(LCDC_TEMP|2); // Korekcja temperaturowa
	lcd_Command(LCDC_BIAS|3); // Tryb multipleksowania
	lcd_Command(LCDC_FS|LCDC_FS_BASIC|LCDC_FS_HORADDR); // Komendy standardowe, tryb adresowania
	lcd_Command(LCDC_DC|LCDC_DC_NORMAL); // W³¹czenie trybu normalnego
	lcd_GoTo(0,0);
}
