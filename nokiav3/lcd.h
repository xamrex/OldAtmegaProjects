/*
** lcd.h
**
** LCD 3310 driver
** Target: ATMEGA :: AVR-GCC
**
** Written by Tony Myatt - 2007
** Quantum Torque - www.quantumtorque.com
*/
#ifndef _NOKIALCD_H_
#define _NOKIALCD_H_

/* Lcd screen size */
#define LCD_X_RES 84
#define LCD_Y_RES 48
#define LCD_CACHE_SIZE ((LCD_X_RES * LCD_Y_RES) / 8)


/* LCD Port */
#define LCD_PORT                   PORTB
#define LCD_DDR                    DDRB

/* ATMega8 port pinout for LCD. */
/* 0.2.6 bug, fixed */
#define LCD_DC_PIN                 PB0  /* Pin 0 */
#define LCD_CE_PIN                 PB2  /* Pin 2 */
#define SPI_MOSI_PIN               PB3  /* Pin 3 */
#define LCD_RST_PIN                PB4  /* Pin 4 */
#define SPI_CLK_PIN                PB5  /* Pin 5 */

/* Special Chars */
#define ARROW_RIGHT	ICON(0)
#define ARROW_LEFT 	ICON(1)
#define ARROW_UP 	ICON(2)
#define ARROW_DOWN 	ICON(3)
#define STOP 		ICON(4)
#define PLAY	 	ICON(5)

/* Function for my special characters */
#define	ICON(x)		'z'+1+x

typedef enum
{
    PIXEL_OFF =  0,
    PIXEL_ON  =  1,
    PIXEL_XOR =  2

} LcdPixelMode;


void lcd_init(void);
void lcd_contrast(unsigned char contrast);
void lcd_clear(void);
void lcd_clear_area(unsigned char line, unsigned char startX, unsigned char endX);
void lcd_clear_line(unsigned char line);
void lcd_goto_xy(unsigned char x, unsigned char y);
void lcd_goto_xy_exact(unsigned char x, unsigned char y);
void lcd_chr(char chr);
void lcd_str(char* str);
void lcd_znak(void);
void obrazek1(void);
void LcdUpdate     ( void );
typedef enum { LCD_CMD  = 0, LCD_DATA = 1 } LcdCmdData;
void LcdSend ( unsigned char data, LcdCmdData cd );
unsigned char LcdPixel ( unsigned char x, unsigned char y, LcdPixelMode mode );
unsigned char LcdLine ( unsigned char x1, unsigned char x2, unsigned char y1, unsigned char y2, LcdPixelMode mode );
unsigned char LcdRect ( unsigned char x1, unsigned char x2, unsigned char y1, unsigned char y2, LcdPixelMode mode );


#endif



