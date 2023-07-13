#include <avr/io.h>
#include <stdint.h>
#include <inttypes.h>
#include <avr/pgmspace.h>

#define LCD_PORT PORTC
#define LCD_DDR  DDRC
#define LCD_CLK (1<<PC0)
#define LCD_SIO (1<<PC1)
#define LCD_CS  (1<<PC2)
#define LCD_RST (1<<PC3)

#define nop() asm volatile("nop")
#define false 0
#define true 1

uint8_t Ver;
void readdata(void);
void LCD_Out(uint8_t Data, uint8_t isCmd);
void LCD_SetColor(uint8_t red, uint8_t green, uint8_t blue);
void LCD_Pixel(uint8_t xPos, uint8_t yPos);
void LCD_Clear(void);
void LCD_HLine(uint8_t xPos, uint8_t yPos, uint8_t width);
uint8_t getNibble(uint8_t nibble);
uint8_t LCD_Char(uint8_t mychr, uint8_t xPos, uint8_t yPos);
uint8_t LCD_CharWidth(uint8_t mychr);
uint8_t LCD_StringWidth(uint8_t *s);
uint8_t LCD_String(uint8_t *s, uint8_t xPos, uint8_t yPos);
void LCD_Rect(uint8_t xPos, uint8_t yPos, uint8_t width, uint8_t height, uint8_t filled);
void LCD_Circle(int xPos, int yPos, int radius, uint8_t filled);
void LCD_Line(int x0, int y0, int x1, int y1);
void LCD_RoundBox(uint8_t xPos, uint8_t yPos, uint8_t width, uint8_t height, uint8_t radius);
void LcdInit(void);
void LCD_ColorSet(uint8_t color);

