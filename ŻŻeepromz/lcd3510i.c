////////////////////////////////////////////////////////////////////////////////////////////
// lcd3510i.c - Biblioteka obs³ugi wyœwietlacza Nokia3510i
//
// Wersja dzia³aj¹ca z lcd na 12bpp, z buforem 8bpp i w³asn¹ palet¹.
//
// Autor: Rados³aw Koppel          Kompilator: WinAVR 20050214
////////////////////////////////////////////////////////////////////////////////////////////
#include <avr\io.h>
#include <inttypes.h>
#include <util\delay.h>
#include <string.h>
#include <avr\pgmspace.h>

#include "makra.h"
#include "harddef.h"
#include "lcd3510i.h"

lcd_pixel lcd_buffer[LCD_SX*LCD_SY] EXMEM;
uint16_t lcd_rgb[256]; // XXXXRRRRGGGGBBBB

//__________________________________________________________________________________________
// Funkcje niskiego poziomu
// Pomocnicza funkcja wewnêtrzna realizuj¹ca szybkie wyszeregowanie
static inline void lcd_serout(uint8_t data)
{
	// 7
	PORT(LCD_PORT) &= ~(1<<LCD_SCK);
	PORT(LCD_PORT) &= ~(1<<LCD_SIO);
	if(data & 1<<7)
		PORT(LCD_PORT) |= 1<<LCD_SIO;
	PORT(LCD_PORT) |= 1<<LCD_SCK;
	// 6
	PORT(LCD_PORT) &= ~(1<<LCD_SCK);
	PORT(LCD_PORT) &= ~(1<<LCD_SIO);
	if(data & 1<<6)
		PORT(LCD_PORT) |= 1<<LCD_SIO;
	PORT(LCD_PORT) |= 1<<LCD_SCK;
	// 5
	PORT(LCD_PORT) &= ~(1<<LCD_SCK);
	PORT(LCD_PORT) &= ~(1<<LCD_SIO);
	if(data & 1<<5)
		PORT(LCD_PORT) |= 1<<LCD_SIO;
	PORT(LCD_PORT) |= 1<<LCD_SCK;
	// 4
	PORT(LCD_PORT) &= ~(1<<LCD_SCK);
	PORT(LCD_PORT) &= ~(1<<LCD_SIO);
	if(data & 1<<4)
		PORT(LCD_PORT) |= 1<<LCD_SIO;
	PORT(LCD_PORT) |= 1<<LCD_SCK;
	// 3
	PORT(LCD_PORT) &= ~(1<<LCD_SCK);
	PORT(LCD_PORT) &= ~(1<<LCD_SIO);
	if(data & 1<<3)
		PORT(LCD_PORT) |= 1<<LCD_SIO;
	PORT(LCD_PORT) |= 1<<LCD_SCK;
	// 2
	PORT(LCD_PORT) &= ~(1<<LCD_SCK);
	PORT(LCD_PORT) &= ~(1<<LCD_SIO);
	if(data & 1<<2)
		PORT(LCD_PORT) |= 1<<LCD_SIO;
	PORT(LCD_PORT) |= 1<<LCD_SCK;
	// 1
	PORT(LCD_PORT) &= ~(1<<LCD_SCK);
	PORT(LCD_PORT) &= ~(1<<LCD_SIO);
	if(data & 1<<1)
		PORT(LCD_PORT) |= 1<<LCD_SIO;
	PORT(LCD_PORT) |= 1<<LCD_SCK;
	// 0
	PORT(LCD_PORT) &= ~(1<<LCD_SCK);
	PORT(LCD_PORT) &= ~(1<<LCD_SIO);
	if(data & 1<<0)
		PORT(LCD_PORT) |= 1<<LCD_SIO;
	PORT(LCD_PORT) |= 1<<LCD_SCK;
}

// Funkcja przesy³a instrukcje
void lcd_Command(uint8_t command)
{
	PORT(LCD_PORT) &= ~(1<<LCD_CS);
#if defined LCD_HARDWARE_SPI
	// Wy³¹czenie spi
	SPCR &= ~(1<<SPE);
	// Wys³anie 0 - komenda
	PORT(LCD_PORT) &= ~(1<<LCD_SCK);
	PORT(LCD_PORT) &= ~(1<<LCD_SIO);
	PORT(LCD_PORT) |= 1<<LCD_SCK;
	// W³¹czenie spi
	SPCR |= 1<<SPE;
	// Wys³anie danej
	SPDR = command;
	while(!(SPSR & 1<<SPIF)) {};
#else
	// 0 - dla komendy
	PORT(LCD_PORT) &= ~(1<<LCD_SCK);
	PORT(LCD_PORT) &= ~(1<<LCD_SIO);
	// Clk (wystarczy 30ns L i 30ns H)
	PORT(LCD_PORT) |= 1<<LCD_SCK;

	lcd_serout(command);
#endif
	PORT(LCD_PORT) |= 1<<LCD_CS;
}

// Funkcja przesy³a dan¹
void lcd_Data(uint8_t data)
{
	PORT(LCD_PORT) &= ~(1<<LCD_CS);
#if defined LCD_HARDWARE_SPI
	// Wy³¹czenie spi
	SPCR &= ~(1<<SPE);
	// Wys³anie 1 - dana
	PORT(LCD_PORT) &= ~(1<<LCD_SCK);
	PORT(LCD_PORT) |= 1<<LCD_SIO;
	PORT(LCD_PORT) |= 1<<LCD_SCK;
	// W³¹czenie spi
	SPCR |= 1<<SPE;
	// Wys³anie danej
	SPDR = data;
	while(!(SPSR & 1<<SPIF)) {};
#else
	// 1 - dla danej
	PORT(LCD_PORT) &= ~(1<<LCD_SCK);
	PORT(LCD_PORT) |= 1<<LCD_SIO;
	// Clk (wystarczy 30ns L i 30ns H)
	PORT(LCD_PORT) |= 1<<LCD_SCK;

	lcd_serout(data);
#endif
	PORT(LCD_PORT) |= 1<<LCD_CS;
}

//__________________________________________________________________________________________
// Obs³uga wyœwietlacza

// Inicjacja wyœwietlacza
void lcd_Init(void)
{
	uint8_t n; // licznik pêtli
	// Zerowanie sprzêtowe
	PORT(LCD_PORT) &= ~(1<<LCD_RES);
	_delay_ms(10.0);
	PORT(LCD_PORT) |= 1<<LCD_RES | 1<<LCD_SCK | 1<<LCD_SIO | 1<<LCD_CS;
	_delay_ms(5.0);

	// Instrukcja zerowania programowego
	lcd_Command(LCD_SWRESET);
	_delay_ms(5.0);

	// Inicjacja ustawieñ
	lcd_Command(LCD_INIESC);

	//____________________________________________________________________________
	// <Display setup 1>
	lcd_Command(LCD_REFSET); // Inicjacja stanów
	lcd_Data(0);

	lcd_Command(LCD_DISCTL); // Ustawienie parametrów sterowania wyœwietlaczem
	lcd_Data(128); // Iloœæ cykli dla sk³adowej, wartoœæ zalecana. Umo¿liwia fizycznie uzyskanie 128 odcieni.
	lcd_Data(0x80); // bit7 = 1 - nie wykonuj inwersji n-tej lini
	lcd_Data(0x81); // dziel czêstotliwoœæ przez 2 dla trybu IDLE, napiêcie matrycy 1/9, IC na górze modu³u, organizacja 98x67
	lcd_Data(84); // Dalej do koñca cykle pracy. Wed³ug zalecanych w dokumentacji, z dwoma cyklami przerwy
	lcd_Data(69);
	lcd_Data(82);
	lcd_Data(67);

	lcd_Command(LCD_GCPSET0); // Gray Scale Position (Rejestr GCPSET0) - ustawienie odcieni szaroœci dla poszczególnych wartoœci (0 ustawione zawsze na 0)
	lcd_Data(1);//1
	lcd_Data(2);//2
	lcd_Data(4);//3
	lcd_Data(8);//4
	lcd_Data(16);//5
	lcd_Data(30);//6
	lcd_Data(40);//7
	lcd_Data(50);//8
	lcd_Data(60);//9
	lcd_Data(70);//10
	lcd_Data(80);//11
	lcd_Data(90);//12
	lcd_Data(100);//13
	lcd_Data(110);//14
	lcd_Data(127);//15

	lcd_Command(LCD_GAMSET); // Wybranie krzywej gamma
	lcd_Data(LCD_GAM0);

	lcd_Command(LCD_COMOUT); // Sposób sterowania wyprowadzeniami linii wyœwitlacza
	lcd_Data(4); // Pierwsze linie 0-41 (konieczne dla 67 linii), z przeplotem

	//__________________________________________________________________
	// <Power supply setup>
	lcd_Command(LCD_PWRCTL);
	lcd_Data(4); // zegar ok. 26kHz, high power mode, wewnêtrzna rezystancja dla V1out

	lcd_Command(LCD_SLPOUT); // Statruje wewnêtrzny oscylator sterownika

	lcd_Command(LCD_VOLCTL); // Wartoœæ napiêcia na przetwornicy
	lcd_Data(127);
	lcd_Data(3);

		// Kontrast - pasuje nam wartoœæ domyœlna, wiêc nie trzeba jej zmieniaæ

	lcd_Command(LCD_TEMPGRD); // Korekcja temperaturowa
	for(n=0; n<14; n++)
		lcd_Data(0);

	lcd_Command(LCD_BSTON); // W³¹czenie przetwornicy
	_delay_ms(20.0); // 40ms konieczne dla stabilizacji napiêcia przetwornicy
	_delay_ms(20.0);

	//__________________________________________________________________
	// <Display setup2>

	lcd_Command(LCD_INVON); // W³¹czam inwersje - dla 0xffff bia³y dla 0 czarny

	// Tryb normalny - nie w³¹czamy trybu czêœciowego

	// Nie konfigurujemy obszarów poziomego przewijania

	//__________________________________________________________________
	// <Display setup3>

	// Tryb dostêpu do pamici 12 bit - nie zmieniamy domyœlnego

	// Nie ustawiamy palety dla 256 kolorów

	// Domyœlny sposób dostêpu do pamiêci obrazu nam pasuje

	// Okno zapisu zdefiniowane domyœlnie na ca³y obszar wyœwietlania

}

// W³¹czenie wyœwietlacza
void lcd_On(void)
{
	lcd_Command(LCD_DISON);
}

void lcd_Off(void)
{
	lcd_Command(LCD_DISOFF);
}

//__________________________________________________________________________________________
// Obs³uga palety
void lcd_loadRGB_P(const prog_uint16_t* pRGB, uint16_t size)
{
	memcpy_P(lcd_rgb, pRGB, size);
}

//__________________________________________________________________________________________
// Obs³uga bufora

// Czyszczenie bufora
void lcd_Cls(lcd_pixel pix)
{
	memset(lcd_buffer, pix, sizeof(lcd_buffer));
}

// Odœwie¿enie ekranu
void lcd_Update(void)
{
	uint16_t n;
	lcd_pixel* pbuf;
	// Wys³anie komendy zapisu do pamiêci wyœwietlacza
	lcd_Command(LCD_RAMWR);

	// Wys³anie danych
	pbuf = lcd_buffer;
	for(n=0; n<(ELEMS(lcd_buffer)/2); n++)
	{
		// W jednej pêtli przesy³am dwa piksele, poniewa¿ piksele parzyste i nieparzyste s¹ inaczej formatowan

		// Wczytujê wartoœæ koloru
		uint16_t pix1 = lcd_rgb[*pbuf++];
		uint16_t pix2 = lcd_rgb[*pbuf++];
		// Wysy³am dane
		pix1 <<= 4;
		lcd_Data((uint8_t)(pix1>>8));
		pix1 |= (pix2>>8) & 0x0F;
		lcd_Data((uint8_t)pix1);
		lcd_Data((uint8_t)pix2);
	}
	// Na koniec transmisji wysy³am komendê NOP
	// (powinna jej okresowe przes³anie zwiêksza stabilnoœæ systemu oraz przerywa dzia³anie komendy zapisu)
	lcd_Command(LCD_NOP);
}

//__________________________________________________________________________________________
// Funkcje rysuj¹ce
void lcd_Pixel(uint8_t x, uint8_t y, lcd_pixel color)
{
	lcd_buffer[x+y*LCD_SX] = color;
}
