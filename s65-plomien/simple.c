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
#include "disp.h"
#include "colors_rgb565.h"
#include "stdlib.h"


// Tworzenie koloru palety
// Iloœæ elementów tablicy

#define ELEMS(p) (sizeof(p)/sizeof(p[0]))
#define LCD_palRGB(r, g, b) ((uint16_t)((r&0x1F)<<11) | ((g&0x3F)<<5) | ((b&0x1F)<<0))
#define FIRE_MAX g_fireRGB[45]
#define LCD_SX 132
#define LCD_SY 176

uint16_t g_fireRGB[] =
{
LCD_palRGB(0, 0, 0),
LCD_palRGB(2, 0, 0),
LCD_palRGB(4, 0, 0),
LCD_palRGB(6, 0, 0),
LCD_palRGB(8, 0, 0),
LCD_palRGB(10, 0, 0),
LCD_palRGB(12, 0, 0),
LCD_palRGB(14, 0, 0),
LCD_palRGB(16, 0, 0),
LCD_palRGB(18, 0, 0),
LCD_palRGB(20, 0, 0),
LCD_palRGB(22, 0, 0),
LCD_palRGB(24, 0, 0),
LCD_palRGB(26, 0, 0),
LCD_palRGB(28, 0, 0),
LCD_palRGB(30, 0, 0),

LCD_palRGB(30, 2, 0),
LCD_palRGB(30, 4, 0),
LCD_palRGB(30, 8, 0),
LCD_palRGB(30, 12, 0),
LCD_palRGB(30, 16, 0),
LCD_palRGB(30, 20, 0),
LCD_palRGB(30, 24, 0),
LCD_palRGB(30, 28, 0),
LCD_palRGB(30, 32, 0),
LCD_palRGB(30, 36, 0),
LCD_palRGB(30, 40, 0),
LCD_palRGB(30, 44, 0),
LCD_palRGB(30, 48, 0),
LCD_palRGB(30, 52, 0),
LCD_palRGB(30, 60, 0),

LCD_palRGB(30, 60, 0),
LCD_palRGB(30, 60, 2),
LCD_palRGB(30, 60, 4),
LCD_palRGB(30, 60, 6),
LCD_palRGB(30, 60, 8),
LCD_palRGB(30, 60, 10),
LCD_palRGB(30, 60, 12),
LCD_palRGB(30, 60, 14),
LCD_palRGB(30, 60, 16),
LCD_palRGB(30, 60, 18),
LCD_palRGB(30, 60, 20),
LCD_palRGB(30, 60, 22),
LCD_palRGB(30, 60, 24),
LCD_palRGB(30, 60, 26),
LCD_palRGB(30, 60, 30),
};

uint16_t bufor[25*25];
void lcd_Pixel(uint16_t x, uint16_t y, uint16_t color)
{
	bufor[x+y*LCD_SX] = color;
}
void update (void){
	{
	  uint16_t i;

	  PORTB &= ~_BV(LCD_CS);  // select display

	  lcd_cmd(SD_CSET);
	  lcd_dat0(0x08);  // start is 8, not 0
	  lcd_dat0(0x01);
	  lcd_dat0(0x21);  // is 00x8B = 0x83+8
	  lcd_dat0(0x01);

	  lcd_cmd(SD_PSET);
	  lcd_dat0(0x00);
	  lcd_dat0(0xAF);

	  lcd_cmd(RAMWR);

	  // start data transmission
	  for (i=0; i<25*25; i++)
	    lcd_dat16(bufor[i]);
	  PORTB |= _BV(LCD_CS);  // deselect display
	}
}

int main(void)
{
  port_init();
  lcd_init_c();
  fill_screen(COLOR_WHITE);


  // Inicjacja generatora liczb pseudolosowych
  	// Jako dan¹ inicjuj¹c¹ przyjmujê sumê modulo dwa wszystkich komórek pamiêci
  	uint16_t *pmem;
  	uint16_t seed = 0; // Moglibyœmy nie inicjowaæ tej zmiennej, ale zg³osi to ostrze¿enie
  	uint16_t n;
  	pmem = (uint16_t*)0; // Uwaga - to nie jest zgodne z C "zagranie"
  	for(n=0; n<(RAMEND+1)/2; n++)
  	{
  		seed ^= *pmem++;
  	}
  	srand(seed);


/**************** wysiwqetlanie**************************************/
	while(1){
			update();
			// Podsycanie ognia
				uint8_t n;
				for(n=0; n<5; n++)
				{
					// Losowanie pozycji
					uint8_t x = rand() % (25-4) + 2;
					uint8_t y = 25 - rand() % 5 - 3;
					// Wypisanie piksela
					lcd_Pixel(x, y, FIRE_MAX);
				}
											uint16_t *ppix = bufor + 25 + 1;
													for(uint8_t y=1; y<25-1; y++)
													{
														for(uint8_t x=1; x<25-1; x++, ppix++)
														{
															// Uwaga odnoœnie optymalizacji
															// P³omieñ ma 46 kolorów. Dodajemy 4 piksele, maksymalna liczba 4*45=180
															// Umo¿liwia to wykonywanie ca³ych obliczeñ na 8 bitach
															uint16_t temp;
															temp = *ppix + *(ppix+25+1) + *(ppix+25-1) + *(ppix+25);
															if(temp != 0)
																temp -= 1;
															*ppix = temp / 4;
														}
														ppix += 2;
													}
												}


	}



