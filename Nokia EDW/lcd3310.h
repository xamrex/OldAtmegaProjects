/*
 * lcd3310.h
 *
 *  Created on: 2011-05-20
 *       Autor: XamreX
 */

#ifndef LCD3310_H_
#define LCD3310_H_
////////////////////////////////////////////////////////////////////////////////////////////
// Definicja komend LCD
// H = X
#define LCDC_FS 0x20
	#define LCDC_FS_BASIC 0
	#define LCDC_FS_EXTENDED 0x01
	#define LCDC_FS_HORADDR 0
	#define LCDC_FS_VERTADDR 0x02
	#define LCDC_FS_PD 0x04
// H = 0 (BASIC)
#define LCDC_DC 0x08
	#define LCDC_DC_BLANK 0
	#define LCDC_DC_NORMAL 0x04
	#define LCDC_DC_ALLON 0x01
	#define LCDC_DC_INVERT 0x05
#define LCDC_XADR 0x80
	// 0-83
#define LCDC_YADR 0x40
	// 0-5
// H = 1 (EXTENDED)
#define LCDC_TEMP 0x04
	// 0-3 - zwykle 2 jest ok
#define LCDC_BIAS 0x10
	// 0-7, dla naszego wyœwietlacza powinno to byæ 3 (n=4 dla mux rate=48)
#define LCDC_VOP 0x40
	// teoretycznie 0-127, nie przekraczaæ 32-88
// Koniec definicji komend
////////////////////////////////////////////////////////////////////////////////////////////////
static inline void lcd_GoTo(uint8_t x, uint8_t y)
{
	lcd_Command(LCDC_XADR|x);
	lcd_Command(LCDC_YADR|y);
}

#endif /* LCD3310_H_ */
