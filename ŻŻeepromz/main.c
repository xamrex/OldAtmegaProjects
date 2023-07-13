////////////////////////////////////////////////////////////////////////////////////////////
// main.c - plik g³ówny programu generuj¹cego efekt ognia na wyœwietlaczu Nokia3510i
//
//     przeznaczenie: "Programowanie procesorów w jêzyku C (EdW)"
//
// Autor: Rados³aw Koppel          Kompilator: WinAVR 20050214
////////////////////////////////////////////////////////////////////////////////////////////
#include <avr\io.h>
#include <inttypes.h>
#include <avr\pgmspace.h>
#include <stdlib.h>

#include "makra.h"
#include "harddef.h"
#include "lcd3510i.h"

//___________________________________________________________________________________________
// Zmienne, tablice, sta³e

// Dostêp do bufora wyœwietlacza
extern lcd_pixel lcd_buffer[];

// Tablica opisuj¹ca paletê kolorów p³omienia
prog_uint16_t g_fireRGB[] =
{
	LCD_palRGB(0, 0, 0),
	LCD_palRGB(1, 0, 0),
	LCD_palRGB(2, 0, 0),
	LCD_palRGB(3, 0, 0),
	LCD_palRGB(4, 0, 0),
	LCD_palRGB(5, 0, 0),
	LCD_palRGB(6, 0, 0),
	LCD_palRGB(7, 0, 0),
	LCD_palRGB(8, 0, 0),
	LCD_palRGB(9, 0, 0),
	LCD_palRGB(10, 0, 0),
	LCD_palRGB(11, 0, 0),
	LCD_palRGB(12, 0, 0),
	LCD_palRGB(13, 0, 0),
	LCD_palRGB(14, 0, 0),
	LCD_palRGB(15, 0, 0),
	LCD_palRGB(15, 1, 0),
	LCD_palRGB(15, 2, 0),
	LCD_palRGB(15, 3, 0),
	LCD_palRGB(15, 4, 0),
	LCD_palRGB(15, 5, 0),
	LCD_palRGB(15, 6, 0),
	LCD_palRGB(15, 7, 0),
	LCD_palRGB(15, 8, 0),
	LCD_palRGB(15, 9, 0),
	LCD_palRGB(15, 10, 0),
	LCD_palRGB(15, 11, 0),
	LCD_palRGB(15, 12, 0),
	LCD_palRGB(15, 13, 0),
	LCD_palRGB(15, 14, 0),
	LCD_palRGB(15, 15, 0),
	LCD_palRGB(15, 15, 1),
	LCD_palRGB(15, 15, 2),
	LCD_palRGB(15, 15, 3),
	LCD_palRGB(15, 15, 4),
	LCD_palRGB(15, 15, 5),
	LCD_palRGB(15, 15, 6),
	LCD_palRGB(15, 15, 7),
	LCD_palRGB(15, 15, 8),
	LCD_palRGB(15, 15, 9),
	LCD_palRGB(15, 15, 10),
	LCD_palRGB(15, 15, 11),
	LCD_palRGB(15, 15, 12),
	LCD_palRGB(15, 15, 13),
	LCD_palRGB(15, 15, 14),
	LCD_palRGB(15, 15, 15),
};

// Maksymalna wartoœæ p³omienia
#define FIRE_MAX (ELEMS(g_fireRGB)-1)


//___________________________________________________________________________________________
// Funkcje

// Inicjacja pamiêci zewnêtrznej - robimy to jeszcze zanim program wystartuje
void before_main(void) __attribute__((naked)) __attribute__((section(".init3")));

// Funkcja umieszczona w sekcji .init3
// Stos jest ju¿ zainicjowany, rejestr zero_reg jest wyzerowany
// Zmienne jeszcze nie s¹ zainicjowane
void before_main(void)
{
	// W³¹czenie zewnêtrznego interfejsu pamiêci
	MCUCR = 1<<SRE;
	// Bity ustawiaj¹ce podzia³ na bloki oraz wprowadzane cykle opóŸnienia w EMCUCR.
	// Domyœlne zapewniaj¹ brak cykli oczekiwania przy dostêpie do zewnêtrznej pamiêci
	// oraz ca³y obszar jako jeden segment.

	// W³¹czenie podtrzymywania stanu na wyjœciach adresowych
	SFIOR = 1<<XMBK /*podtrzymywanie*/ | 1<<XMM0 /*maskowanie A15*/;
	// Ustawienie A15 na 0
	DDRC = 0x80;
	PORTC &= ~(1<<7);
}

// START programu
int main(void)
{
	// Inicjacja wyprowadzeñ
	DDR(LCD_PORT) = 1<<LCD_RES | 1<<LCD_SCK | 1<<LCD_SIO | 1<<LCD_CS;
	PORT(LCD_PORT) = 1<<LCD_SCK | 1<<LCD_SIO | 1<<LCD_CS;
	// Inicjacja portu SPI
#if defined LCD_HARDWARE_SPI
	SPCR = 1<<SPE | 1<<MSTR | 1<<CPOL | 1<<CPHA;
	SPSR = 1<<SPI2X;
#endif
	// Inicjacja wyœwietlacza
	lcd_Init();

	// W³¹czenie wyœwietlacza
	lcd_On();

	// Za³adowanie palety
	lcd_loadRGB_P(g_fireRGB, sizeof(g_fireRGB));

	// Wype³nienie wyœwietlacza samym bia³ym kolorem
	lcd_Cls(0);

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
	// Pêtla g³ówna
	for(;;)
	{
		// Odœwierzenie widoku
		lcd_Update();

		// Podsycanie ognia
		uint8_t n;
		for(n=0; n<80; n++)
		{
			// Losowanie pozycji
			uint8_t x = rand() % (LCD_SX-4) + 2;
			uint8_t y = LCD_SY - rand() % 5 - 3;
			// Wypisanie piksela
			lcd_Pixel(x, y, FIRE_MAX);
		}

		// Rozmazywanie obrazu
		// Zaczynamy od linii 1 (nie 0), i pierwszej kolumny
		lcd_pixel *ppix = lcd_buffer + LCD_SX + 1;
		for(uint8_t y=1; y<LCD_SY-1; y++)
		{
			for(uint8_t x=1; x<LCD_SX-1; x++, ppix++)
			{
				// Uwaga odnoœnie optymalizacji
				// P³omieñ ma 46 kolorów. Dodajemy 4 piksele, maksymalna liczba 4*45=180
				// Umo¿liwia to wykonywanie ca³ych obliczeñ na 8 bitach
				lcd_pixel temp;
				temp = *ppix + *(ppix+LCD_SX+1) + *(ppix+LCD_SX-1) + *(ppix+LCD_SX);
				if(temp != 0)
					temp -= 1;
				*ppix = temp / 4;
			}
			ppix += 2;
		}
	}

	return 0;
}
