/*
************************************************************************************************************
**                                                                                                        **
**   Driver for LCD with LS020 and LPH8836 controller from S65 mobile phone (cell phone?/handy phone?).   **
**   LS020 - not tested, reason: LS020 version got +17V on RST and GND_17V on RS and got fried up :D.     **
**   Fully tested with LPH8836 (green PCB in display) and in horizontal position.                         **
**                                                                                                        **
**   KNOWN BUGS:                                                                                          **
**   - function s65_line have some problems with drawing line where we want ...                           **
**   - problems with loading color images from flash (at end of image might show up few dark pixels) -    **
**     I don't know why this happens but i've noticed it's not problem of my code but linker or           **
**     compiler(??) (or just my ATMega32 getting old :/ )                                                 **
**     Solution for this is adding extra few pixels at the end of array with image                        **
**   - ????                                                                                               **
**   TO DO:                                                                                               **
**   - HARDWARE SPI for LPH8836 support                                                                   **
**   - something else (beside LS020 support...)?                                                          **
**                                                                                                        **
**                                                                                                        **
**   ABOUT:                                                                                               **
**         s65.c & s65.h - CREATED by Saper_2  (mailto:nolinno11@gmail.com or GG:4476700)                 **
**                                                                                                        **
**                                                                                                        **
**   CHANGE LOG:                                                                                          **
**   1.0 - Basic (and working) initalization for LPH8836 and screen fill                                  **
**   1.1 - Added full graphic support with writing (except s65_load and s65_load_color functions)         **
**   1.2 - Added functions s65_load and s65_load_color                                                    **
**   1.3 - Added full support for L2F50 displays and correncted s65_line functionœ                         **
**   That's all so far :D                                                                                 **
**                                                                                                        **
************************************************************************************************************
************************************************************************************************************


*/

#include <avr/io.h>
#include <inttypes.h>
#include <avr/pgmspace.h>
#include <stdlib.h>

#include "makra.h"
#include "delay.h"
#include "s65.h"

//#if defined(S65_DEBUG)
	#include "usart.h"
//#endif

#if S65_CHAR == 1
	#include "font5x8.h"
#endif


#define XCHG(a, b, var_type) \
{\
	var_type temp##a##b; \
	temp##a##b = b; \
	b = a; \
	a = temp##a##b;	\
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++                                                                                                ++
// ++                                   SPI SENDING ROUTINIES                                        ++
// ++                                                                                                ++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#if S65_SPI_SOFTWARE==1

/*
LCD S65 support SPI Mode0, SPI bus state when idle (/CS=H): CLK=Low. 
When bus active: On rising edge CLK - data line is sampled, on faling edge CLK - data line is setup.
Data order: from MSB to LSB .
*/

void s65_spi_byte(uint8_t b) {
	#if defined(S65_DEBUG)
	usart_SendStrP((prog_char*)PSTR("\r\nSending byte: 0x"));
	usart_SendHexByte(b);
	#endif
	for (uint8_t i=0;i<8;i++) {
		if (b & 0x80) S65_PORT |= (1<<S65_DTA);
			else S65_PORT &= ~(1<<S65_DTA);
		b <<= 1;
		//nop;
		S65_PORT |= (1<<S65_CLK); // rising
		//delay1us(S65_SPI_DELAY_US); 
		nop;
		nop;
		S65_PORT &= ~(1<<S65_CLK); // falinig
		//delay1us(S65_SPI_DELAY_US); 
		nop;
		nop;
	}
	//S65_PORT &= ~(1<<S65_CLK); // pull down
}

void s65_spi_word(uint16_t b) {
	#if defined(S65_DEBUG)
	usart_SendStrP((prog_char*)PSTR("\r\nSending word: 0x"));
	usart_SendHexByte(b>>8);
	usart_SendHexByte(b);
	#endif
	s65_spi_byte(b>>8);
	s65_spi_byte(b);
}

// ++++++++++++++++++++ HARDWARE SPI ++++++++++++++++++++++++++++++
#else

void s65_spi_byte(uint8_t b) {
	#if defined(S65_DEBUG)
	usart_SendStrP((prog_char*)PSTR("\r\nSending byte: 0x"));
	usart_SendHexByte(b);
	#endif
	/*for (uint8_t i=0;i<8;i++) {
		if (b & 0x80) S65_PORT |= (1<<S65_DTA);
			else S65_PORT &= ~(1<<S65_DTA);
		b <<= 1;
		nop;
		S65_PORT |= (1<<S65_CLK); // rising
		delay1us(S65_SPI_DELAY_US); 
		S65_PORT &= ~(1<<S65_CLK); // falinig
		delay1us(S65_SPI_DELAY_US); 
	}
	S65_PORT &= ~(1<<S65_CLK); // pull down
	*/
	
}

#endif



// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++                                                                                                ++
// ++                                   S65 INITALIZATION FOR LS020                                  ++
// ++                                                                                                ++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#if defined(S65_TYPE_LS020)

/*
void s65_cmd8(uint8_t cmd) {
	#if defined(S65_DEBUG)
	usart_SendStrP((prog_char*)PSTR("\r\n **** Sending COMMAND 0x"));
	usart_SendHexByte(cmd);
	usart_SendStrP((prog_char*)PSTR(" ****"));
	#endif
	S65_PORT |= (1<<S65_RS); // Command
	s65_select;
	//nop;
	s65_spi_byte(cmd);
	//nop;
	s65_deselect;
	#if S65_DATA_CMD_DELAY > 0
	delay1us(S65_DATA_CMD_DELAY);
	#endif
	#if defined(S65_DEBUG)
	usart_SendStrP((prog_char*)PSTR("\r\n **** END COMMAND SEND ****"));
	#endif
}*/

void s65_cmd(uint16_t cmd) {
	#if defined(S65_DEBUG)
	usart_SendStrP((prog_char*)PSTR("\r\n **** Sending COMMAND 0x"));
	usart_SendHexByte(cmd);
	usart_SendStrP((prog_char*)PSTR(" ****"));
	#endif
	S65_PORT |= (1<<S65_RS); // Command
	s65_select;
	//nop;
	s65_spi_byte(cmd>>8);
	s65_spi_byte(cmd);
	//nop;
	s65_deselect;
	#if S65_DATA_CMD_DELAY > 0
	delay1us(S65_DATA_CMD_DELAY);
	#endif
	#if defined(S65_DEBUG)
	usart_SendStrP((prog_char*)PSTR("\r\n **** END COMMAND SEND ****"));
	#endif
}
	
void s65_data(uint16_t data) {
	#if defined(S65_DEBUG)
	usart_SendStrP((prog_char*)PSTR("\r\n **** Sending DATA 0x"));
	usart_SendHexByte(data>>8);
	usart_SendHexByte(data);
	usart_SendStrP((prog_char*)PSTR(" ****"));
	#endif
	S65_PORT &= ~(1<<S65_RS); // data
	s65_select;
	//nop;
	s65_spi_byte(data>>8);
	s65_spi_byte(data);
	//nop;
	s65_deselect;
	#if S65_DATA_CMD_DELAY > 0
	delay1us(S65_DATA_CMD_DELAY);
	#endif
	#if defined(S65_DEBUG)
	usart_SendStrP((prog_char*)PSTR("\r\n **** END DATA SEND ****"));
	#endif
}

prog_uint16_t s65_init_tab[] = {  // 35words
	0xfdfd, 0xfdfd, // 2
	// delay1ms(80)
	0xef00, 0xee04, 0x1b04, 0xfefe, 0xfefe, 0xef90, 0x4a04, 0x7f3f, 
	0xee04, 0x4306, // 10
	// delay1ms(20); 
	
	0xef90, 0x0983, 0x0800, 0x0baf, 0x0a00, 0x0500, 0x0600, 0x0700, 
	0xef00, 0xee0c, 0xef90, 0x0080, 0xefb0, 0x4902, 0xef00, 0x7f01, 
	0xe181, 0xe202, 0xe276, 0xe183, // 20
	// delay1ms(100)
	
	0x8001, 0xef90, 0x0000  // 3
	};
	
	

void s65_init(void) {
	uint8_t i;
	
	S65_PORT |= 1<<S65_CS | 1<<S65_RS | 1<<S65_CLK | 1<<S65_DTA | 1<<S65_RST;
	S65_PORT &= ~(1<<S65_CLK);
	S65_DDR |= 1<<S65_CS | 1<<S65_RS | 1<<S65_CLK | 1<<S65_DTA | 1<<S65_RST;
	// reset pulse
	S65_PORT &= ~(1<<S65_RST);
	delay1ms(20);
	S65_PORT |= 1<<S65_RST;
	delay1ms(20);

	for (i=0;i<35;i++) {
		s65_cmd(pgm_read_word((prog_uint16_t*)&s65_init_tab[i]));
		if (i== 1) delay1ms(20);
		if (i==11) delay1ms(20);
		if (i==31) delay1ms(100);
	}
	delay1ms(10);
}


void s65_fill(uint16_t color) {
	// from start
	s65_cmd(0xef90);
	s65_cmd(0x0500);
	s65_cmd(0x0600);
	s65_cmd(0x0700);

	for (uint16_t t=0;t<(132*176);t++) {
		s65_data(color);
	}
}	

//#if (S65_FUNC_DRAW_PROG==1 || S65_FUNC_DRAW==1)
// ------------------------------------- always enabled :P
void swap_bytes(uint8_t *b1, uint8_t *b2) {
	uint8_t b=*b1;
	*b1=*b2;
	*b2=b;
}
//#endif

#if S65_FUNC_DRAW==1
void s65_draw(uint8_t x1, uint8_t y1,uint8_t x2, uint8_t y2, uint16_t *data) {
	/*
	if (x2 > x1) swap_bytes(&x1,&x2);
	if (y2 > y1) swap_bytes(&y1,&y2);
	
	uint16_t count=(x2-x1)*(y2-y1), i=0;
	
	s65_cmd(0x05);
	s65_data(0x0038);
	s65_cmd(0x17);
	i=(x2<<8)|x1;
	s65_data(i); //lcd_comdat(x+CHAR_W-1, x);
	s65_cmd(0x16);
	i=0;
	i=(y2<<8)|y1;
	s65_data(i); //lcd_comdat(y+CHAR_H-1, y);
	s65_cmd(0x21);
	i=0;
	i=(x<<8)|y;
    s65_data(i);
    s65_select;
	s65_spi_byte(0x76);
	for (i=0;i<count-1;i++) {
		s65_spi_word((uint16_t*)&data[i]);
	}
	s65_deselect;
	*/
}
#endif // S65_FUNC_DRAW==1

#if S65_FUNC_DRAW_PROG==1
void s65_draw_P(uint8_t x1, uint8_t y1,uint8_t x2, uint8_t y2, prog_uint16_t *data) {
	/*
	if (x2 > x1) swap_bytes(&x1,&x2);
	if (y2 > y1) swap_bytes(&y1,&y2);
	
	uint16_t count=(x2-x1)*(y2-y1), i;
	
	s65_cmd(0x05);
	s65_data(0x0038);
	s65_cmd(0x17);
	s65_data(x2,x1); //lcd_comdat(x+CHAR_W-1, x);
	s65_cmd(0x16);
	s65_data(y2,y1); //lcd_comdat(y+CHAR_H-1, y);
	s65_cmd(0x21);
    s65_data(x,y);
    s65_select;
	s65_spi_byte(0x76);
	for (i=0;i<count-1;i++) {
		s65_spi_word(pgm_read_word((prog_uint16_t*)&data[i]));
	}
	s65_deselect;
	*/
}
#endif // S65_FUNC_DRAW_PROG==1

#if S65_FUNC_PIXEL==1

void s65_frame(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
	//uint16_t i;
	
	//if (x1>x2) swap_bytes(&x1,&x2);
	//if (y1>y2) swap_bytes(&y1,&y2);
	#if S65_ROTATE_90==1		
		// use +90 disp orientation
		s65_cmd(0xef80); // framing ON ?
		s65_cmd(0x1805); // 
		s65_cmd(0x1200+x1);
		s65_cmd(0x1500+x2);
		s65_cmd(0x1300+(175-y1));
		//s65_cmd(0x1300+(y1));
		s65_cmd(0x1600+(175-y2));
		//s65_cmd(0x1600+(y2));
	#else		
		s65_cmd(0xef80); // framing ON ?
		s65_cmd(0x1800); // 
		s65_cmd(0x1200+x1);
		s65_cmd(0x1500+x2);
		s65_cmd(0x1300+y1);
		s65_cmd(0x1600+y2);
	#endif
	
}

void s65_pix(uint8_t x, uint8_t y, uint16_t color) {

	/*s65_cmd(0xef90); // partail screen write
	#if S65_ROTATE_90 == 1
		{
			uint16_t tmp=y;
			y=x;
			x=tmp;
		}
		s65_cmd(0x0504);
		s65_cmd(0x0800+(x));
		s65_cmd(0x0900+(x));
		//s65_cmd(0x0a00+(175-x));
		s65_cmd(0x0a00+(y));
		s65_cmd(0x0b00+(y));
		//s65_cmd(0x0b00+(175-x));
	#else
		s65_cmd(0x0500);
		s65_cmd(0x0800+x);
		s65_cmd(0x0900+x);
		s65_cmd(0x0a00+y);
		s65_cmd(0x0b00+y);
	#endif
	*/
	//s65_cmd(
	// color
	//s65_cmd(0x22);
	s65_frame(x,y,x,y);
	s65_data(color);
}
#endif // S65_FUNC_PIXEL==1


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++                                                                                                ++
// ++                                 FUNCTIONS FOR S65 LPH8836 LCD                                  ++
// ++                                                                                                ++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#elif defined(S65_TYPE_LPH)

void s65_cmd(uint8_t cmd) {
	#if defined(S65_DEBUG)
	usart_SendStrP((prog_char*)PSTR("\r\n **** Sending COMMAND 0x"));
	usart_SendHexByte(cmd);
	usart_SendStrP((prog_char*)PSTR(" ****"));
	#endif
	S65_PORT |= (1<<S65_RS); // Command
	s65_select;
	//nop;
	s65_spi_byte(0x74);
	s65_spi_byte(0x00);
	s65_spi_byte(cmd);
	//nop;
	s65_deselect;
	#if S65_DATA_CMD_DELAY > 0
	delay1us(S65_DATA_CMD_DELAY);
	#endif
	#if defined(S65_DEBUG)
	usart_SendStrP((prog_char*)PSTR("\r\n **** END COMMAND SEND ****"));
	#endif
}
	
void s65_data(uint16_t data) {
	#if defined(S65_DEBUG)
	usart_SendStrP((prog_char*)PSTR("\r\n **** Sending DATA 0x"));
	usart_SendHexByte(data>>8);
	usart_SendHexByte(data);
	usart_SendStrP((prog_char*)PSTR(" ****"));
	#endif
	S65_PORT &= ~(1<<S65_RS); // data
	s65_select;
	//nop;
	s65_spi_byte(0x76);
	s65_spi_byte(data>>8);
	s65_spi_byte(data);
	//nop;
	s65_deselect;
	#if S65_DATA_CMD_DELAY > 0
	delay1us(S65_DATA_CMD_DELAY);
	#endif
	#if defined(S65_DEBUG)
	usart_SendStrP((prog_char*)PSTR("\r\n **** END DATA SEND ****"));
	#endif
}

uint8_t s65_init_cmd[34] PROGMEM = {
	0x07, 0x02, 0x04, 0x0C, 0x0D, 0x0E, // command seq 1 [6]
	0x03, 0x0E, // command seq 2 [2]
	0x0D, // command seq 3 [1]
	0x01, 0x0F, 0x0B, 0x11, 0x06, 0x05, 0x14, 0x15, 
	0x16, 0x17, 0x20, 0x30, 0x31, 0x32, 0x33, 0x34, 
	0x35, 0x36, 0x37, 0x3A, 0x3B, // -------------- command seq 4 [21]
	// ========           SUM=30 bytes     =================
	0x07, 0x07, 0x07, 0x07 
};

uint16_t s65_init_data[34] PROGMEM = {
	0x0000, 0x0400, 0x0000, 0x0001, 0x0616, 0x1010, // data seq 1 [6]
	0x000C, 0x2D1F, // data seq 2 [2]
	0x0616, // data seq 3 [1]
	0x0215, 0x0000, 0x0000, 0x0000, 0x0000, 0x0030, 0xAF00, 0x0000, 
	0x8300, 0xAF00, 0x0000, 0x0000, 0x0400, 0x0207, 0x0700, 0x0005, 
	0x0703, 0x0707, 0x0007, 0x1200, 0x0009, // --------------------- data seq 4 [21]	
	// =======         SUM=30words     =========================
	0x0005, 0x0025, 0x0027, 0x0037
	
};

void s65_init(void) {
	#if S65_SPI_SOFTWARE==1
		S65_PORT |= 1<<S65_CS | 1<<S65_RS | 1<<S65_CLK | 1<<S65_DTA;
		S65_PORT &= ~(1<<S65_CLK | 1<<S65_RST);
		S65_DDR |= 1<<S65_CS | 1<<S65_RS | 1<<S65_CLK | 1<<S65_DTA | 1<<S65_RST;
	#else // HARDWARE SPI
	// pre configure ports...
		S65_PORT |= 1<<S65_CS | 1<<S65_RS | 1<<S65_CLK | 1<<S65_DTA;
		S65_PORT &= ~(1<<S65_CLK | 1<<S65_RST);
		S65_DDR |= 1<<S65_CS | 1<<S65_RS | 1<<S65_CLK | 1<<S65_DTA | 1<<S65_RST;
		
	#endif
	// RESET is pulled down above ^^^^
	delay1ms(15);
	S65_PORT |= 1<<S65_RST;
	delay1ms(60);
	// run init sequence
	for (uint8_t i=0;i<34;i++) {
		s65_cmd(pgm_read_byte((prog_uint8_t*)&s65_init_cmd[i]));
		s65_data(pgm_read_word((prog_uint16_t*)&s65_init_data[i]));
		delay1ms(8);
	}
	// ready ?
}

void s65_fill(uint16_t color) {
	// from start
	s65_cmd(0x16);
	s65_data(0x8300);
	s65_cmd(0x17);
	s65_data(0xaf00);
	s65_cmd(0x21);
	s65_data(0x0000);
	s65_cmd(0x22);
	// block wirte...
	s65_select;
	s65_spi_byte(0x76);
		// loop...
		for (uint16_t t=0;t<(132*176);t++) {
			s65_spi_word(color);
		}
	s65_deselect;
	// ----	
}

//#if (S65_FUNC_DRAW_PROG==1 || S65_FUNC_DRAW==1)
// ------------------------------------- always enabled :P
void swap_bytes(uint8_t *b1, uint8_t *b2) {
	uint8_t b=*b1;
	*b1=*b2;
	*b2=b;
}
//#endif

#if S65_FUNC_DRAW==1
void s65_draw(uint8_t x1, uint8_t y1,uint8_t x2, uint8_t y2, uint16_t *data) {
	if (x2 > x1) swap_bytes(&x1,&x2);
	if (y2 > y1) swap_bytes(&y1,&y2);
	
	uint16_t count=(x2-x1)*(y2-y1), i=0;
	
	s65_cmd(0x05);
	s65_data(0x0038);
	s65_cmd(0x17);
	i=(x2<<8)|x1;
	s65_data(i); //lcd_comdat(x+CHAR_W-1, x);
	s65_cmd(0x16);
	i=0;
	i=(y2<<8)|y1;
	s65_data(i); //lcd_comdat(y+CHAR_H-1, y);
	s65_cmd(0x21);
	i=0;
	i=(x<<8)|y;
    s65_data(i);
    s65_select;
	s65_spi_byte(0x76);
	for (i=0;i<count-1;i++) {
		s65_spi_word((uint16_t*)&data[i]);
	}
	s65_deselect;
}
#endif // S65_FUNC_DRAW==1

#if S65_FUNC_DRAW_PROG==1
void s65_draw_P(uint8_t x1, uint8_t y1,uint8_t x2, uint8_t y2, prog_uint16_t *data) {
	if (x2 > x1) swap_bytes(&x1,&x2);
	if (y2 > y1) swap_bytes(&y1,&y2);
	
	uint16_t count=(x2-x1)*(y2-y1), i;
	
	s65_cmd(0x05);
	s65_data(0x0038);
	s65_cmd(0x17);
	s65_data(x2,x1); //lcd_comdat(x+CHAR_W-1, x);
	s65_cmd(0x16);
	s65_data(y2,y1); //lcd_comdat(y+CHAR_H-1, y);
	s65_cmd(0x21);
    s65_data(x,y);
    s65_select;
	s65_spi_byte(0x76);
	for (i=0;i<count-1;i++) {
		s65_spi_word(pgm_read_word((prog_uint16_t*)&data[i]));
	}
	s65_deselect;
}
#endif // S65_FUNC_DRAW_PROG==1

#if S65_FUNC_PIXEL==1

void s65_frame(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
	uint16_t i;
	
	//if (x1>x2) swap_bytes(&x1,&x2);
	//if (y1>y2) swap_bytes(&y1,&y2);
	#if S65_ROTATE_90==1
		// 90st frame rotation...
		s65_cmd(0x05);
		s65_data(0x0030);
	#else
		// 90st frame rotation...
		s65_cmd(0x05);
		s65_data(0x0038);
	#endif
	
	#if S65_ROTATE_90==1 // right rotation
		s65_cmd(0x16);
		i=(x2<<8)|x1;
		s65_data(i);
		s65_cmd(0x17);
		i=(y2<<8)|y1;
		s65_data(i);
	#else
		s65_cmd(0x17);
		swap_bytes(&x1,&x2);
		i=((175-x2)<<8)|(175-x1);
		s65_data(i);
		s65_cmd(0x16);
		i=(y2<<8)|y1;
		s65_data(i);
	#endif
}

void s65_pix(uint8_t x, uint8_t y, uint16_t color) {

	s65_cmd(0x21);
	#if S65_ROTATE_90 == 1
		s65_data((uint16_t)((x<<8)|y)); // x,y
	#else
		s65_data((uint16_t)(((175-x)<<8)|y)); // x,y
	#endif
	// color
	s65_cmd(0x22);
	s65_data(color);
}
#endif // S65_FUNC_PIXEL==1


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++                                                                                                ++
// ++                           FUNCTIONS FOR S65 L2F50 LCD by EPSON                                 ++
// ++                                                                                                ++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#elif defined(S65_TYPE_L2F50)

void s65_cmd(uint8_t cmd) {
	#if defined(S65_DEBUG)
	usart_SendStrP((prog_char*)PSTR("\r\n **** Sending COMMAND 0x"));
	usart_SendHexByte(cmd);
	usart_SendStrP((prog_char*)PSTR(" ****"));
	#endif
	S65_PORT &= ~(1<<S65_RS); // Command
	s65_select;
	//nop;
	s65_spi_byte(cmd);
	s65_spi_byte(0x00);
	//nop;
	s65_deselect;
	#if S65_DATA_CMD_DELAY > 0
	delay1us(S65_DATA_CMD_DELAY);
	#endif
	#if defined(S65_DEBUG)
	usart_SendStrP((prog_char*)PSTR("\r\n **** END COMMAND SEND ****"));
	#endif
}
	
void s65_data16(uint16_t data) {
	#if defined(S65_DEBUG)
	usart_SendStrP((prog_char*)PSTR("\r\n **** Sending DATA 0x"));
	usart_SendHexByte(data>>8);
	usart_SendHexByte(data);
	usart_SendStrP((prog_char*)PSTR(" ****"));
	#endif
	S65_PORT |= (1<<S65_RS); // data
	s65_select;
	//nop;
	s65_spi_byte(data>>8);
	s65_spi_byte(data);
	//nop;
	s65_deselect;
	#if S65_DATA_CMD_DELAY > 0
	delay1us(S65_DATA_CMD_DELAY);
	#endif
	#if defined(S65_DEBUG)
	usart_SendStrP((prog_char*)PSTR("\r\n **** END DATA SEND ****"));
	#endif
}

void s65_data00(uint8_t data) {
	#if defined(S65_DEBUG)
	usart_SendStrP((prog_char*)PSTR("\r\n **** Sending DATA_00 0x"));
	usart_SendHexByte(data);
	usart_SendStrP((prog_char*)PSTR(" ****"));
	#endif
	S65_PORT |= (1<<S65_RS); // data
	s65_select;
	//nop;
	s65_spi_byte(data);
	s65_spi_byte(0x00);
	//nop;
	s65_deselect;
	#if S65_DATA_CMD_DELAY > 0
	delay1us(S65_DATA_CMD_DELAY);
	#endif
	#if defined(S65_DEBUG)
	usart_SendStrP((prog_char*)PSTR("\r\n **** END DATA SEND ****"));
	#endif
}

void s65_init(void) {
	uint8_t i;
	
	static const uint8_t disctl[9] PROGMEM = {
												0x4C, 0x01, 0x53, 0x00, 0x02, 0xB4, 0xB0, 0x02, 
												0x00
											};
	static const uint8_t gcp64_0[29] PROGMEM = {
												0x11, 0x27, 0x3C, 0x4C, 0x5D, 0x6C, 0x78, 0x84,
												0x90, 0x99, 0xA2, 0xAA, 0xB2, 0xBA, 0xC0, 0xC7,
												0xCC, 0xD2, 0xD7, 0xDC, 0xE0, 0xE4, 0xE8, 0xED,
												0xF0, 0xF4, 0xF7, 0xFB, 0xFE
											};
	static const uint8_t gcp64_1[34] PROGMEM = {
												0x01, 0x03, 0x06, 0x09, 0x0B, 0x0E, 0x10, 0x13,
												0x15, 0x17, 0x19, 0x1C, 0x1E, 0x20, 0x22, 0x24,
												0x26, 0x28, 0x2A, 0x2C, 0x2D, 0x2F, 0x31, 0x33,
												0x35, 0x37, 0x39, 0x3B, 0x3D, 0x3F, 0x42, 0x44,
												0x47, 0x5E
											};
	static const uint8_t gcp16[15] PROGMEM = {
												0x13, 0x23, 0x2D, 0x33, 0x38, 0x3C, 0x40, 0x43,
												0x46, 0x48, 0x4A, 0x4C, 0x4E, 0x50, 0x64
											};

	#if S65_SPI_SOFTWARE==1
		S65_PORT |= 1<<S65_CS | 1<<S65_RS | 1<<S65_CLK | 1<<S65_DTA;
		S65_PORT &= ~(1<<S65_CLK | 1<<S65_RST);
		S65_DDR |= 1<<S65_CS | 1<<S65_RS | 1<<S65_CLK | 1<<S65_DTA | 1<<S65_RST;
	#else // HARDWARE SPI
	// pre configure ports...
		S65_PORT |= 1<<S65_CS | 1<<S65_RS | 1<<S65_CLK | 1<<S65_DTA;
		S65_PORT &= ~(1<<S65_CLK | 1<<S65_RST);
		S65_DDR |= 1<<S65_CS | 1<<S65_RS | 1<<S65_CLK | 1<<S65_DTA | 1<<S65_RST;
		
	#endif
	// RESET is pulled down above ^^^^
	delay1ms(15);
	S65_PORT |= 1<<S65_RST;
	delay1ms(60);


	s65_cmd(0xbc);
	s65_data00(0x2a); // 0x2A=565 mode, 0x0A=666mode, 0x3A=444mode
	
	s65_cmd(0xca);
	for (i=0;i<9;i++) {
		s65_data00(pgm_read_byte(&disctl[i]));
	}
	
	s65_cmd(0xcb);
	for (i=0;i<29;i++) {
		s65_data00(pgm_read_byte(&gcp64_0[i]));
		s65_data00(0x00);
	}
	for (i=0;i<34;i++) {
		s65_data00(pgm_read_byte(&gcp64_1[i]));
		s65_data00(0x01);
	}
	
	s65_cmd(0xcc);
	for (i=0;i<15;i++) {
		s65_data00(pgm_read_byte(&gcp16[i]));
	}
  
	s65_cmd(0xcd);
	s65_data00(0x00);
	
	s65_cmd(0xd0);
	s65_data00(0x00);
	
	s65_cmd(0x94);

	s65_cmd(0x15);
	s65_data00(0x08);
	s65_data00(0x01);
	s65_data00(0x8b);
	s65_data00(0x01);
	
	s65_cmd(0x75);
	s65_data00(0x00);
	s65_data00(0x8f);

	s65_cmd(0xaa);
	s65_data00(0x00);
	s65_data00(0xaf);
	s65_data00(0xaf);
	s65_data00(0x03);

	s65_cmd(0xab);
	s65_data00(0x00);
  
	s65_cmd(0xaf);
	// ready ?
}

void s65_fill(uint16_t color) {

	// start, end: X
	s65_cmd(0x15); 
	s65_data00(8+0); // start X
	s65_data00(0x01); // why ?
	s65_data00(8+S65_WIDTH-1); // end X
	s65_data00(0x01); // why ?
	
	// start, end: Y
	s65_cmd(0x75);
	s65_data00(0x00); // start y
	s65_data00(S65_HEIGHT-1); // end y
	
	s65_cmd(0x5c); // MEM WRITE
	s65_select;
	for (uint16_t t=0;t<(132*176);t++) {
		s65_data16(color);
	}
	s65_deselect;
}

//#if (S65_FUNC_DRAW_PROG==1 || S65_FUNC_DRAW==1)
// ------------------------------------- always enabled :P
void swap_bytes(uint8_t *b1, uint8_t *b2) {
	uint8_t b=*b1;
	*b1=*b2;
	*b2=b;
}
//#endif

#if S65_FUNC_PIXEL==1

void s65_frame(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
	//uint16_t i;
	
	if (x1>x2) swap_bytes(&x1,&x2);
	if (y1>y2) swap_bytes(&y1,&y2);
	
	#if S65_ROTATE_90==1
		// 90st frame rotation...
		//s65_cmd(0x05);
		//s65_data(0x0030);
	#else
		// 90st frame rotation...
		//s65_cmd(0x05);
		//s65_data(0x0038);
	#endif
	
	// start, end: X
	s65_cmd(0x15); 
	s65_data00(8+x1); // start X
	s65_data00(0x01); // why ?
	s65_data00(8+x2); // end X
	s65_data00(0x01); // why ?
	
	// start, end: Y
	s65_cmd(0x75);
	s65_data00(y1); // start y
	s65_data00(y2); // end y

	#if S65_ROTATE_90==1 // right rotation
		//s65_cmd(0x16);
		//i=(x2<<8)|x1;
		//s65_data(i);
		//s65_cmd(0x17);
		//i=(y2<<8)|y1;
		//s65_data(i);
	#else
		//s65_cmd(0x17);
		//swap_bytes(&x1,&x2);
		//i=((175-x2)<<8)|(175-x1);
		//s65_data(i);
		//s65_cmd(0x16);
		////i=(y2<<8)|y1;
		//s65_data(i);
	#endif
}

void s65_pix(uint8_t x, uint8_t y, uint16_t color) {

	s65_frame(x,y,x,y);
	s65_cmd(0x5c);
	s65_data16(color);
	#if S65_ROTATE_90 == 1
		//s65_data((uint16_t)((x<<8)|y)); // x,y
	#else
		//s65_data((uint16_t)(((175-x)<<8)|y)); // x,y
	#endif
	// color

}
#endif // S65_FUNC_PIXEL==1


#else
	#error No display type selected!
#endif

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


#if S65_LINE_H == 1
void s65_lineH(uint8_t x, uint8_t y, uint8_t w, uint16_t color) {
	if (x >= S65_WIDTH) x = S65_WIDTH-1;
	if (y >= S65_HEIGHT) y = S65_HEIGHT-1;
	w = w+x;
	if (w>= S65_WIDTH) w = S65_WIDTH-1;
	
	//s65_frame(x,y,w,y);
	for (;x<=w;x++) s65_pix(x,y,color);
}
#endif

#if S65_LINE_V == 1
void s65_lineV(uint8_t x, uint8_t y, uint8_t h, uint16_t color) {
	
	if (x >= S65_WIDTH) x = S65_WIDTH-1;
	if (y >= S65_HEIGHT) y = S65_HEIGHT-1;
	h = h+y;
	if (h>= S65_HEIGHT) h = S65_HEIGHT-1;
	
	s65_frame(x,y,x,y+h);
	for (;y<=h;y++) s65_pix(x,y,color);
}
#endif

#if S65_LINE == 1
void s65_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color) {
	int16_t dx,dy,sx,sy,f;
	uint16_t i;
	
	if (x1 > x2) {
		XCHG(x1,x2,uint8_t);
		XCHG(y1,y2,uint8_t);
	}
	
	if (y1 > y2) {
		XCHG(x1,x2,uint8_t);
		XCHG(y1,y2,uint8_t);
	}
	
	if (x1 == x2) {
		s65_lineV(x1,y1,(y2-y1),color);
		return;
	}
	
	if (y1 == y2) {
		s65_lineH(x1,x1,(x2-x1),color);
		return;
	}

	// drawinf window
	#if !defined(S65_TYPE_L2F50)
	s65_frame(x1,y1,x2,y2);
	#endif
	
	dx = x2 - x1;
	dy = y2 - y1;
	
	if (dx < 0) {
		dx = -dx;
		sx = -1;
	} else {
		sx = 1;
	}
	
	if (dy < 0) {
		dy = -dy;
		sy = -1;
	} else {
		sy = 1;
	}

	dx <<= 1; // Shift Dx , Left , 1  '-2
	dy <<= 1; // Shift Dy , Left , 1  '-2
	
	s65_pix(x1,y1,color); // Call Lcdpixel(x1 , Y1 , Pixelmode)

	if (dx > dy) {
		i = dx;
		i >>= 1;
		f = dy-i;
		while (x1!=x2) {
			if (f >= 0) {
				y1 += sy; //Y1 = Y1 + Stepy
				f -= dx; //Fraction = Fraction - Dx
			}
         x1 += sx;
			f += dy;
			s65_pix(x1,y1,color);
		}
	} else {
      i = dy;
		i >>= 1;
		f = dx-i;
		while (y1!=y2) {
			if (f >= 0) {
				x1 += sx;
				f -= dy; //Fraction = Fraction - Dy
			}
			y1 += sy;
			f += dx;
			s65_pix(x1,y1,color);
		}
	}
}
#endif

#if S65_RECT == 1
void s65_rect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color) {
	uint8_t w,h;
	if (x1 >= S65_WIDTH) x1 = S65_WIDTH-1;
	if (y1 >= S65_HEIGHT) y1 = S65_HEIGHT-1;
	if (x2 >= S65_WIDTH) x2 = S65_WIDTH-1;
	if (y2 >= S65_HEIGHT) y2 = S65_HEIGHT-1;
	if (x1 > x2) { w = x1; x1 = x2; x2 = w; }
	if (y1 > y2) { h = y1; y1 = y2; y2 = h; }
	w = x2-x1;
	h = y2-y1;
	if (w>= S65_WIDTH) w = S65_WIDTH-1;
	if (h>= S65_HEIGHT) h = S65_HEIGHT-1;
	
	s65_frame(x1,y1,x2,y2);
	
	s65_lineH(x1,y1,w,color);
	s65_lineH(x1,y2,w,color);
	s65_lineV(x1,y1+1,h-2,color);
	s65_lineV(x2,y1+1,h-2,color);

}
#endif

#if S65_RECT_FILL == 1
void s65_rectFill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color) {
	uint8_t y;
	if (x1 >= S65_WIDTH) x1 = S65_WIDTH-1;
	if (y1 >= S65_HEIGHT) y1 = S65_HEIGHT-1;
	if (x2 >= S65_WIDTH) x2 = S65_WIDTH-1;
	if (y2 >= S65_HEIGHT) y2 = S65_HEIGHT-1;
	if (x1 > x2) { y = x1; x1 = x2; x2 = y; }
	if (y1 > y2) { y = y1; y1 = y2; y2 = y; }
	
	s65_frame(x1,y1,x2,y2);
	
	//for (;x1<=x2;x1++) {
	//	for (y=y1;y<=y2;y++) {
	//		s65_pix(x1,y,color);
	//	}
	//}
	for (;x1<=x2;x1++) {
		s65_lineV(x1,y1,y2-y1,color);
	}
}
#endif

#if S65_CHAR == 1
uint8_t s65_char(uint8_t x, uint8_t y, char znak, uint16_t bg, uint16_t fg) {
	if ((znak > (GPH_CHAR_COUNT+GPH_CHAR_START)) || (znak < GPH_CHAR_START)) znak = GPH_CHAR_START;
	// calculate char location in array
	uint16_t t=(znak-GPH_CHAR_START)*GPH_CHAR_WIDTH;
	
	#if GPH_CHAR_HEIGHT <= 8
		uint8_t b; // 8bit chars
	#elif ((GPH_CHAR_HEIGHT <= 16) && (GPH_CHAR_HEIGHT > 8))
		uint16_t b; // 16bit chars
	#elif GPH_CHAR_HEIGHT > 16
		uint32_t b; // 32bit chars
	#endif
	
	#if !defined(S65_TYPE_L2F50) // in L2F50 lcd module this is not needed
	s65_frame(x,y,GPH_CHAR_WIDTH+x,GPH_CHAR_HEIGHT+y);
	#endif
	

	// /*
	//	for (uint8_t i=(GPH_CHAR_WIDTH+x);i>x;i--) { // do not flip character
		for (uint8_t i=x;i<(GPH_CHAR_WIDTH+x);i++) { // flip vertically character
		// draw x bytes of char - char byte data orientation vertically
		// get column byte of char	
		#if GPH_CHAR_HEIGHT <= 8
			b = pgm_read_byte(&GPH_FONT_NAME[t++]);
		#elif ((GPH_CHAR_HEIGHT <= 16) && (GPH_CHAR_HEIGHT > 8))
			b = pgm_read_word(&GPH_FONT_NAME[t++]);
		#elif GPH_CHAR_HEIGHT > 16
			b = pgm_read_dword(&GPH_FONT_NAME[t++]);
		#endif
		// window to draw
		
		for (uint8_t j=y;j<(y+GPH_CHAR_HEIGHT);j++) {
			#if (GPH_FONT_COLUMN_UP_DOWN == 1)
				#if GPH_CHAR_HEIGHT <= 8
					if (b & 0x80) { // up to 8bit font height
				#elif ((GPH_CHAR_HEIGHT <= 16) && (GPH_CHAR_HEIGHT > 8))
					if (b & 0x8000) { // up to 16bit font height
				#elif GPH_CHAR_HEIGHT > 16
					if (b & 0x80000000) { // up to 32bit font height
				#endif
			#else
				#if GPH_CHAR_HEIGHT <= 8
					if (b & 0x01) { // up to 8bit font height
				#elif ((GPH_CHAR_HEIGHT <= 16) && (GPH_CHAR_HEIGHT > 8))
					if (b & 0x0001) { // up to 16bit font height
				#elif GPH_CHAR_HEIGHT > 16
					if (b & 0x00000001) { // up to 32bit font height
				#endif
			#endif
				#if S65_ROTATE_90==1
					s65_pix(j,i,fg);
				#else
					s65_pix(i,j,fg);
				#endif
			} else { // draw background (if needed...)??
				#if S65_ROTATE_90==1
					s65_pix(j,i,bg);
				#else
					s65_pix(i,j,bg);
				#endif
			}
			#if (GPH_FONT_COLUMN_UP_DOWN == 1)
				b <<= 1;
			#else
				b >>= 1;
			#endif
		}
	}
	// */
	return GPH_CHAR_WIDTH;
}
#endif

#if S65_STR == 1
uint8_t s65_str(uint8_t x, uint8_t y, char* str, uint16_t bg, uint16_t fg) {
	while(*str != 0) {
		s65_char(x, y, *str++, bg, fg);
		x += GPH_CHAR_WIDTH;
	}
	return x;
}	
#endif

#if S65_STR_P == 1
uint8_t s65_strP(uint8_t x, uint8_t y, char* str, uint16_t bg, uint16_t fg) {
	
	//#if S65_ROTATE_90 != 1
		char znak;
		while (0 != (znak = pgm_read_byte(str++))) {
			s65_char(x, y, znak, bg, fg);
			x += GPH_CHAR_WIDTH;
		}
		return x;
	/*#else
		// reverse character order in displaying string - i had some problems so i wrote this...
		uint8_t len=0;
		char* s2=str;
		char znak;
		
		while (0 != pgm_read_byte(s2++)) len++;
		//len++;
		while(len) {
			len--;
			znak = pgm_read_byte(str+len);
			s65_char(x, y, znak, bg, fg);
			x += GPH_CHAR_WIDTH;		
		}
		return x;
	#endif*/
}		
#endif

#if S65_DEC == 1
uint8_t s65_dec(uint8_t x, uint8_t y, int val, uint16_t bg, uint16_t fg) {
	char bufor[10];
	return s65_str(x,y,itoa(val, bufor, 10), bg, fg);
}	
#endif

#if S65_HEX_BYTE == 1
uint8_t s65_hexByte(uint8_t x, uint8_t y, uint8_t val, uint16_t bg, uint16_t fg) {
	if (((val >> 4) & 0x0f) > 0x09) {
		s65_char(x,y,'7'+((val >> 4) & 0x0f), bg, fg);
	} else {
		s65_char(x,y,'0'+((val >> 4) & 0x0f), bg, fg);
	}
	x += GPH_CHAR_WIDTH;
	if ((val & 0x0f) > 0x09) {
		s65_char(x,y,'7'+(val & 0x0f), bg, fg);
	} else {
		s65_char(x,y,'0'+(val & 0x0f), bg, fg);
	}
	return x+=GPH_CHAR_WIDTH;
}	
#endif

#if S65_LOAD == 1
void s65_load(prog_uint8_t* pData, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t bg, uint16_t fg) {
	uint8_t b,y2,x2,t=0;
	//h--;
	w+= x;// = (w-1) + x;
	h+= y;// = (h-1) + y;
	
	if ((w > S65_WIDTH) || (h > S65_HEIGHT)) return;
	
	s65_frame(x,y,w,h);
	
	for (y2=y;y2<h;y2++) {
		x2=x;
		while (x2<w) {
			b = pgm_read_byte(pData++);
			for (t=0;t<8;t++) {
				if (b & 0x80) {
					s65_pix((x2+t),y2,fg);
				} else {
					s65_pix((x2+t),y2,bg);
				}
				b <<= 1;
			}
			x2+=8;
		}
	}
}
#endif

#if S65_LOAD_COLOR == 1
void s65_load_color(prog_uint16_t* pData, uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
	uint8_t y2,x2;
	uint16_t b;
	//h--;
	w+= x;// = (w-1) + x;
	h+= y;// = (h-1) + y;
	
	if ((w > S65_WIDTH) || (h > S65_HEIGHT)) return;
	
	s65_frame(0,0,175,131);
	
	//usart_SendStrP((prog_char*)PSTR("\r\nCOLOR LOAD PROCEDURE+++++++++++++++++++"));
	
	for (y2=y;y2<h;y2++) {
		x2=x;
		while (x2<w) {
			b = pgm_read_word(pData++);
			s65_pix(x2,y2,b);
			/*
			usart_SendStrP((prog_char*)PSTR("\r\n     COLUMN.ROWxCOLOR: "));
			usart_SendDec(y2);
			usart_SendChar('.');
			usart_SendDec(x2);
			usart_SendChar('x');
			usart_SendBinByte(b>>8);
			usart_SendBinByte(b);
			usart_SendChar(',');
			usart_SendDec(pData);
			*/
			x2++;
		}
	}
}

#endif

