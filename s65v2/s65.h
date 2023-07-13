#ifndef _S65_LCD_H_
#define _S65_LCD_H_

// Comment to disable DEBUG
//#define S65_DEBUG


#define S65_SPI_SOFTWARE 1 // set to 0 to disable SOFTWARE SPI
// Display type:
//#define S65_TYPE_LPH
//#define S65_TYPE_LS020
#define S65_TYPE_L2F50
#define S65_DATA_CMD_DELAY 0 // delay in [us] after deselecting display after data/command sequence


#if S65_SPI_SOFTWARE==1
	#define S65_PORT PORTA
	#define S65_DDR DDRA
	// LCD Pin 1 to 10:
	#define S65_RS 0  // Pin 1
	#define S65_RST 1 // Pin 2 
	#define S65_CS 2  // Pin 3
	#define S65_CLK 3 // Pin 4
	#define S65_DTA 4 // Pin 5
	// Pin  6: Vcc 2,9V
	// Pin  7: GND
	// Pin  8: Vcc 1,8V
	// Pin  9: LED+
	// Pin 10: LED-
	
	/*
	#define S65_RS 4  // Pin 1
	#define S65_RST 3 // Pin 2 
	#define S65_CS 2  // Pin 3
	#define S65_CLK 1 // Pin 4
	#define S65_DTA 0 // Pin 5
	*/
	
	#define S65_SPI_DELAY_US 1000
#else
	-#define S65_PORT PORTB
	-#define S65_DDR DDRB
	// LCD Pin 1 to 10:
	-#define S65_RS 4  // Pin 1
	-#define S65_RST 3 // Pin 2 
	-#define S65_CS 2  // Pin 3
	-#define S65_CLK 1 // Pin 4
	-#define S65_DTA 0 // Pin 5
	// Pin  6: Vcc 2,9V
	// Pin  7: GND
	// Pin  8: Vcc 1,8V
	// Pin  9: LED+
	// Pin 10: LED-
	//#define S65_SPI_DELAY_US 500

#endif

#if !defined(S65_TYPE_LPH) && !defined(S65_TYPE_LS020) && !defined(S65_TYPE_L2F50)
	#error No S65 display type defined (LPH or LS020 or L2F50)
#endif

#define s65_select S65_PORT &= ~(1<<S65_CS)
#define s65_deselect S65_PORT |= (1<<S65_CS)

#define s65_reset() {S65_PORT &= ~(1<<S65_RST);\
				    delay1ms(2); \
				    S65_PORT |= (1<<S65_RST); }

void s65_init(void);
void s65_fill(uint16_t color);

// functions enablers
#define S65_FUNC_DRAW 0
#define S65_FUNC_DRAW_PROG 0
#define S65_FUNC_PIXEL 1
	#define S65_ROTATE_90 0

/*
In LPH8836 S65 display without S65_ROTATE_90 display coords look like:

   (0,0) +---------------------------------+ (175,0)
  =====1=|                                 | +-+ 
  ==WIRE |                                 | |C| 
  ====10=|                                 | |T| 
	     |                                 | |R| 
	     |                                 | |L| 
	     |                                 | +-+ 
  (0,131)+---------------------------------+(131,175)

S65_ROTATE_90 == 1:


        +-------+                                                   
        |CTRL IC|                                                   
        +-------+                                                   
  0,0+---------------+0,131                                        
     |               |                                              
     |               |                                              
     |               |                                              
     |               |                                              
     |               |                                              
     |               |                                              
     |               |                                              
     |               |                                              
     |               |                                              
     |               |                                              
     |               |                                              
     |               |                                              
175,0+---------------+175,131                                       
      |WIRE|                                                        
    10||||||1          


In LS020 S65 display without S65_ROTATE_90 display coords look like:


      +-------+                                                   
      |CTRL IC|                                                   
      +-------+                                                   
  0,0+---------------+0,131                                        
     |               |                                              
     |  ---------->  |                                              
     |            /  |                                              
     |          /    |                                              
     |        /      |                                              
     |      /        |                                              
     |    /          |                                              
     |  /            |                                              
     | ----------->  |                                              
     |               |                                              
     |               |                                              
     |               |                                              
175,0+---------------+175,131                                       
      |WIRE|                                                        
    10||||||1          


S65_ROTATE == 1 - DO NOT WORKING

   (0,0) +---------------------------------+ (175,0)
  =====1=|     ------->                    | +-+ 
  ==WIRE |            /                    | |C| 
  ====10=|          /                      | |T| 
	     |        /                        | |R| 
	     |      /                          | |L| 
	     |    ------>                      | +-+ 
	     |                                 |    
  (0,131)+---------------------------------+(131,175)

                                             
*/

#if S65_FUNC_DRAW==1
void s65_draw(uint8_t x1, uint8_t y1,uint8_t x2, uint8_t y2, uint16_t *data);
#endif

#if S65_FUNC_DRAW_PROG==1
void s65_draw_P(uint8_t x1, uint8_t y1,uint8_t x2, uint8_t y2, prog_uint16_t *data);
#endif

#if S65_FUNC_PIXEL==1
void s65_frame(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void s65_pix(uint8_t x, uint8_t y, uint16_t color);
#endif

#if defined(S65_TYPE_LS020)
	#if S65_ROTATE_90 == 1
		#define S65_WIDTH 176
		#define S65_HEIGHT 132
	#else
		#define S65_WIDTH 132
		#define S65_HEIGHT 176
	#endif
#elif defined(S65_TYPE_LPH)
	#if S65_ROTATE_90 == 1
		#define S65_WIDTH 132
		#define S65_HEIGHT 176
	#else
		#define S65_WIDTH 176
		#define S65_HEIGHT 132
	#endif
#elif defined(S65_TYPE_L2F50)
	// in L2F50 rotate doesn't exists, and it's too much pain in ass to write soft-rotate (for now)
		#define S65_WIDTH 132
		#define S65_HEIGHT 176
#endif
// Function enablers (set 1 to include function, set 0 to exclude function):

#define S65_LINE_H 1
#define S65_LINE_V 1
#define S65_LINE 1
	
#define S65_RECT 1
#define S65_RECT_FILL 1

#define S65_CHAR 1

#define S65_STR 0
#define S65_STR_P 1
#define S65_DEC 0
#define S65_HEX_BYTE 0
#define S65_LOAD 1
#define S65_LOAD_COLOR 1

// function dependencies

#if ( (S65_LINE == 1) && ((S65_LINE_H == 0) || (S65_LINE_V == 0)) )
	#error "To use s65_line you have to enable functions: s65_lineV and s65_lineH!"
#endif

#if (S65_RECT == 1) && ((S65_LINE_V == 0) || (S65_LINE_H == 0))
	#error "To use s65_rect you have to enable s65_lineH and s65_lineV function too!"
#endif

#if ((S65_STR == 1) || (S65_STR_P == 1) || (S65_HEX_BYTE == 1)) && (S65_CHAR == 0)
	#error "To use: s65_str and/or s65_strP and/or s65_hexByte you have to enable function s65_char too!"
#endif

#if (S65_DEC == 1) && (S65_STR == 0) 
	#error "To use s65_dec you have to enable s65_str function too!"
#endif

// graphic functions
#if S65_LINE_H == 1
void s65_lineH(uint8_t x, uint8_t y, uint8_t w, uint16_t color);
#endif
#if S65_LINE_V == 1
void s65_lineV(uint8_t x, uint8_t y, uint8_t h, uint16_t color);
#endif
#if S65_LINE == 1
void s65_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color);
#endif
#if S65_RECT == 1
void s65_rect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color);
#endif
#if S65_RECT_FILL == 1
void s65_rectFill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color);
#endif
#if S65_CHAR == 1
uint8_t s65_char(uint8_t x, uint8_t y, char znak, uint16_t bg, uint16_t fg); // return character lenght
#endif
#if S65_STR == 1
uint8_t s65_str(uint8_t x, uint8_t y, char* str, uint16_t bg, uint16_t fg);
#endif
#if S65_STR_P == 1
uint8_t s65_strP(uint8_t x, uint8_t y, char* str, uint16_t bg, uint16_t fg);
#endif
#if S65_DEC == 1
uint8_t s65_dec(uint8_t x, uint8_t y, int val, uint16_t bg, uint16_t fg);
#endif
#if S65_HEX_BYTE == 1
uint8_t s65_hexByte(uint8_t x, uint8_t y, uint8_t val, uint16_t bg, uint16_t fg);
#endif

#if S65_LOAD == 1
void s65_load(prog_uint8_t* pData, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t bg, uint16_t fg);
#endif

#if S65_LOAD_COLOR == 1
void s65_load_color(prog_uint16_t* pData, uint8_t x, uint8_t y, uint8_t w, uint8_t h);
#endif




// --------------- COLOR DEFINITIONS -------------------------
/* 16 SAFE COLORS - HTML/CSS (24bit)
Color 	             Hexadecimal 	Color 	Hexadecimal 	Color 	Hexadecimal Color 	Hexadecimal
aqua / cyan           #00FFFF 	     gray 	#808080 	     navy 	#000080 	silver 	#C0C0C0
black 	              #000000 	     green 	#008000 	     olive 	#808000 	teal 	#008080
blue 	              #0000FF 	     lime 	#00FF00 	     purple	#800080 	white 	#FFFFFF
fuchsia / magenta 	  #FF00FF 	     maroon	#800000 	     red 	#FF0000 	yellow 	#FFFF00    
*/
#define S65_COLOR_AQUA		0x07ff
#define S65_COLOR_FUCHSIA	0xf81f
#define S65_COLOR_GRAY		0x8410
#define S65_COLOR_GREEN		0x0c00
#define S65_COLOR_MAROON	0x8000
#define S65_COLOR_NAVY		0x0010
#define S65_COLOR_OLIVE		0x8400
#define S65_COLOR_PURPLE	0x8010
#define S65_COLOR_RED		0xf800
#define S65_COLOR_SILVER	0xc618
#define S65_COLOR_TEAL		0x0410
#define S65_COLOR_YELLOW	0xffe0

#define S65_COLOR_WHITE	0xffff
#define S65_COLOR_BLACK 0x0000
#define S65_COLOR_RED   0xf800
#define S65_COLOR_LIME	0x07e0
#define S65_COLOR_BLUE  0x001F

#endif
