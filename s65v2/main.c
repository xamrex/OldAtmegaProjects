// - ---------------------------------------
// ---- Timer na attiny2313 + lcd CBS0084

#include <avr/io.h>
#include <inttypes.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

// ------------------------
// #include "harddef.h"
#include "makra.h"
#include "delay.h"
#include "s65.h"
#include "usart.h"
#include "colors_rgb565.h"

#define SAPER_LOGO 0


#if SAPER_LOGO == 1
	#include "saper_logo.h"
	#include "saper_logo_color.h"
#endif

int main(void) {
	//uint8_t x,y,i=0;
	//delay1ms(100);
	usart_Init();
	usart_SendStrP((prog_char*)PSTR("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\nInit..."));
	
	s65_init();
	
	usart_SendStrP((prog_char*)PSTR("\r\nInit... END.\r\nTESTING :D:D:D:D\r\n"));
	
	//s65_fill(S65_COLOR_BLACK);
	//s65_fill(S65_COLOR_RED);
	//s65_fill(S65_COLOR_GREEN);
	s65_fill(S65_COLOR_BLUE);
	delay1ms(50);
	s65_fill(S65_COLOR_WHITE);
	
	#if S65_ROTATE_90==0
	
	s65_strP(1,8,(prog_char*)PSTR("ABCDEFGHIJKLMNOPQRSTUVWXYZ"),S65_COLOR_WHITE,S65_COLOR_BLUE);
	s65_strP(1,20,(prog_char*)PSTR("abcdefghijklmnopqrstuvwxyz"),S65_COLOR_LIME,S65_COLOR_BLACK);
	s65_strP(1,32,(prog_char*)PSTR("01234567890<>?,./:\";'{}|["),COLOR_WHEAT,S65_COLOR_RED);
	s65_strP(1,44,(prog_char*)PSTR("]\\=-+_!@#$%^&*()"),S65_COLOR_BLACK,COLOR_PERU);
	// functions test:
	s65_lineH(1,1,128,S65_COLOR_BLUE);
	s65_lineV(1,64,109,S65_COLOR_FUCHSIA);
	
	s65_line(10,65,128,125,S65_COLOR_OLIVE);
	s65_line(10,125,128,65,S65_COLOR_OLIVE);
	
	
	#if SAPER_LOGO == 1
		#define mrec_x 12
		#define mrec_y 72
		s65_load((prog_uint8_t*)&img,mrec_x,mrec_y,40,36,S65_COLOR_WHITE,S65_COLOR_SILVER);
		s65_rect(mrec_x-2,mrec_y-2,mrec_x+40+2,mrec_y+36+2,S65_COLOR_MAROON);
		s65_rect(mrec_x-1,mrec_y-1,mrec_x+40+1,mrec_y+36+1,S65_COLOR_MAROON);
		
		#define rec_x 74
		#define rec_y 70
		s65_load_color((prog_uint16_t*)&color_logo,rec_x,rec_y,50,50);
		s65_rect(rec_x-1,rec_y-1,rec_x+50+1,rec_y+50+1,COLOR_ORANGE);
		s65_rect(rec_x-2,rec_y-2,rec_x+50+2,rec_y+50+2,COLOR_ORANGE);
		
	#endif
	
	
	s65_rectFill(10,130,125,150,S65_COLOR_BLUE);
	s65_strP(32,132,(prog_char*)PSTR("S65 LCD drivers"), S65_COLOR_BLUE, S65_COLOR_AQUA);
	s65_strP(42,141,(prog_char*)PSTR("by Saper_2"), S65_COLOR_BLUE, S65_COLOR_YELLOW);
	
	s65_strP(15,152,(prog_char*)PSTR("S65-L2F50 / S65-LS020"),COLOR_WHITE, COLOR_BLACK);
	s65_strP(31,160,(prog_char*)PSTR("S65-LPH8836"),COLOR_WHITE, COLOR_BLACK);
	s65_strP(18,168,(prog_char*)PSTR("Driver version: 1.4"),COLOR_WHITE, COLOR_BLACK);
	
	#else
	// rotated display mode
	
	s65_strP(20,4,(prog_char*)PSTR("ABCDEFGHIJKLMNOPQRSTUVWXYZ"),S65_COLOR_WHITE,S65_COLOR_BLUE);
	s65_strP(20,16,(prog_char*)PSTR("abcdefghijklmnopqrstuvwxyz"),S65_COLOR_LIME,S65_COLOR_BLACK);
	s65_strP(20,28,(prog_char*)PSTR("01234567890<>?,./:\";'{}|["),COLOR_WHEAT,S65_COLOR_RED);
	s65_strP(43,40,(prog_char*)PSTR("]\\=-+_!@#$%^&*()"),S65_COLOR_BLACK,COLOR_PERU);
	// functions test:
	
	/*
	s65_lineH(1,1,128,S65_COLOR_BLUE);
	s65_lineV(1,64,109,S65_COLOR_FUCHSIA);
	s65_line(10,65,128,125,S65_COLOR_OLIVE);
	s65_line(10,125,128,65,S65_COLOR_OLIVE);
	*/
	
	#if SAPER_LOGO == 1
		#define mrec_x 12
		#define mrec_y 58
		s65_load((prog_uint8_t*)&img,mrec_x,mrec_y,40,36,S65_COLOR_WHITE,S65_COLOR_SILVER);
		s65_rect(mrec_x-2,mrec_y-2,mrec_x+40+2,mrec_y+36+2,S65_COLOR_MAROON);
		s65_rect(mrec_x-1,mrec_y-1,mrec_x+40+1,mrec_y+36+1,S65_COLOR_MAROON);
		
		#define rec_x 100
		#define rec_y 50
		s65_load_color((prog_uint16_t*)&color_logo,rec_x,rec_y,50,50);
		s65_rect(rec_x-1,rec_y-1,rec_x+50+1,rec_y+50+1,COLOR_ORANGE);
		s65_rect(rec_x-2,rec_y-2,rec_x+50+2,rec_y+50+2,COLOR_ORANGE);
		
	#endif
	
	//s65_rectFill(10,130,125,150,S65_COLOR_BLUE);
	s65_strP(24,100,(prog_char*)PSTR("S65 LCD drivers by saper_2"), S65_COLOR_BLUE, S65_COLOR_AQUA);
	
	s65_strP(15,112,(prog_char*)PSTR("S65-L2F50/S65-LS020/S65-LPH8836"),COLOR_WHITE, COLOR_BLACK);
	s65_strP(40,122,(prog_char*)PSTR("Driver version: 1.4"),COLOR_WHITE, COLOR_BLACK);	
	
	#endif
	
	s65_strP(40,75,(prog_char*)PSTR("Compilation for:"), COLOR_GOLD, COLOR_BLACK);
	s65_strP(40,83,(prog_char*)PSTR("ravmar@elektroda"), COLOR_GOLD, COLOR_BLACK);
	
	
	// */
	for (;;) {
		//s65_fill(S65_COLOR_WHITE);
		//s65_fill(S65_COLOR_BLACK);
	}
}
