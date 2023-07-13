/* ***********************************************************************
**
**  Copyright (C) 2005  Christian Kranz
**
**  Siemens S65 Display Control for L2F50 type of display
**  EPSON L2F50 compatible
*********************************************************************** */
// file disp.c
//

// avr specific includes for local usage
#include <avr/pgmspace.h>  // AVR specific functions to access the program memory
#include <math.h>
// my public includes
#include "disp.h"
#include "colors_rgb565.h"

const uint16_t pot10[4] ={10,100,1000,10000}; //do funkcji ubi 2 dec



// ascii table, starting with character blank (32)
// size is 8x14



const uint8_t ascii_tab[1][16] PROGMEM = {
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // space (32)

};


uint16_t backcolor=0xFFFF;    // color of background
uint16_t textcolor=0x0000;


void port_init()
{
  uint8_t tmp;

  PORTB &= ~_BV(LCD_RESET);
  DDRB |= _BV(LCD_RESET);

  PORTB |= _BV(LCD_MOSI);
  DDRB |= _BV(LCD_MOSI);

  PORTB &= ~_BV(LCD_CS);
  DDRB |= _BV(LCD_CS);

  PORTB &= ~_BV(LCD_SCK);
  DDRB |= _BV(LCD_SCK);

  PORTB |= _BV(LCD_MISO);

  PORTB |= _BV(LCD_RS);  // not used from LPH display
  DDRB |= _BV(LCD_RS);

  // setup SPI Interface
  SPCR = _BV(MSTR) | _BV(SPE);
  SPSR = 1;  // double speed bit

  tmp=SPSR;
}


void lcd_write(uint8_t dat)
{
  SPDR= dat;
  while (!(SPSR & _BV(SPIF)));
}


void lcd_cmd(uint8_t dat)
{
  PORTB &= ~_BV(LCD_RS);

  lcd_write(dat);
  lcd_write(0x00);

  PORTB |= _BV(LCD_RS);
}

void lcd_dat0(uint8_t dat)
{
  lcd_write(dat);
  lcd_write(0x00);
}


void lcd_dat16(uint16_t dat)
{
  lcd_write(dat>>8);
  lcd_write(dat);
}


void lcd_cspulse(void)
{
  PORTB |= _BV(LCD_CS);
  asm volatile("nop");
  PORTB &= ~_BV(LCD_CS);
}


void lcd_init_c(void)
/*
  Init the Siemens S65 Display with EPSON controller

  (c) Christian Kranz, 10/2005
*/
{
  uint8_t i;
  static const uint8_t disctl[9] PROGMEM = {0x4C, 0x01, 0x53, 0x00, 0x02, 0xB4, 0xB0, 0x02, 0x00};
  static const uint8_t gcp64_0[29] PROGMEM =
                      {0x11,0x27,0x3C,0x4C,0x5D,0x6C,0x78,0x84,0x90,0x99,0xA2,0xAA,0xB2,0xBA,
                       0xC0,0xC7,0xCC,0xD2,0xD7,0xDC,0xE0,0xE4,0xE8,0xED,0xF0,0xF4,0xF7,0xFB,
                       0xFE};
  static const uint8_t gcp64_1[34] PROGMEM =
                     {0x01,0x03,0x06,0x09,0x0B,0x0E,0x10,0x13,0x15,0x17,0x19,0x1C,0x1E,0x20,
                      0x22,0x24,0x26,0x28,0x2A,0x2C,0x2D,0x2F,0x31,0x33,0x35,0x37,0x39,0x3B,
                      0x3D,0x3F,0x42,0x44,0x47,0x5E};
  static const uint8_t gcp16[15] PROGMEM =
                      {0x13,0x23,0x2D,0x33,0x38,0x3C,0x40,0x43,0x46,0x48,0x4A,0x4C,0x4E,0x50,0x64};

  // generate clean display reset
  PORTB &= ~_BV(LCD_RESET); // reset display
  PORTB |= _BV(LCD_CS);  // CS is high during reset release
  PORTB |= _BV(LCD_RS);  // RS is set to high
  _delay_ms(10);
  PORTB |= _BV(LCD_RESET);  // release reset
  _delay_ms(35);

  PORTB &= ~_BV(LCD_CS);  // select display



  lcd_cmd(DATCTL);
 lcd_dat0(0x2A);  // 0x2A=565 mode, 0x0A=666mode, 0x3A=444mode

 lcd_dat0(0);  //332 mode
  lcd_cspulse();

  lcd_cmd(DISCTL);
  for (i=0; i<9; i++)
  {
    lcd_dat0(pgm_read_byte(&disctl[i]));
  }

  lcd_cmd(GCP64);
  for (i=0; i<29; i++)
  {
    lcd_dat0(pgm_read_byte(&gcp64_0[i]));
    lcd_dat0(0x00);
  }
  for (i=0; i<34; i++)
  {
    lcd_dat0(pgm_read_byte(&gcp64_1[i]));
    lcd_dat0(0x01);
  }

  lcd_cmd(GCP16);
  for (i=0; i<15; i++)
  {
    lcd_dat0(pgm_read_byte(&gcp16[i]));
  }

  lcd_cmd(GSSET);
  lcd_dat0(0x00);

  lcd_cmd(OSSEL);
  lcd_dat0(0x00);

  lcd_cmd(SLPOUT);

//  _delay_ms(7);

  lcd_cmd(SD_CSET);
  lcd_dat0(0x08);
  lcd_dat0(0x01);
  lcd_dat0(0x8B);
  lcd_dat0(0x01);

  lcd_cmd(SD_PSET);
  lcd_dat0(0x00);
  lcd_dat0(0x8F);


  lcd_cmd(ASCSET);
  lcd_dat0(0x00);
  lcd_dat0(0xAF);
  lcd_dat0(0xAF);
  lcd_dat0(0x03);

  lcd_cmd(SCSTART);
  lcd_dat0(0x00);

  PORTB &= ~_BV(LCD_RS);
  lcd_dat0(DISON);




  PORTB |= _BV(LCD_CS);  // deselect display
}




void fill_screen(uint16_t color)
{
  uint16_t i;

  PORTB &= ~_BV(LCD_CS);  // select display

  lcd_cmd(SD_CSET);
  lcd_dat0(0x08);  // start is 8, not 0
  lcd_dat0(0x01);
  lcd_dat0(0x8B);  // is 00x8B = 0x83+8
  lcd_dat0(0x01);

  lcd_cmd(SD_PSET);
  lcd_dat0(0x00);
  lcd_dat0(0xAF);

  lcd_cmd(RAMWR);

  // start data transmission
  for (i=0; i<DISP_W*DISP_H; i++)
    lcd_dat16(color);


  PORTB |= _BV(LCD_CS);  // deselect display
}
////////////////////////////////////////
unsigned char dot (uint8_t x, uint8_t y, uint16_t color)
{
	//warunek sprawdzajacy czy wymiary nie przekraczaja wymiarow wyswietlacza, jesli tak to wywala errora
	if ((x>131) || (y>175)){print_str(0,0,"Zle wymiary DOT", COLOR_WHITE,COLOR_RED); }

  PORTB &= ~_BV(LCD_CS);  // select display

  lcd_cmd(SD_CSET);
  lcd_dat0(8+x);  // poczatek jest od 8,
  lcd_dat0(0x01);
  lcd_dat0(0x8b);  // is 00x8B = 0x83+8, koniec adresowania w kolumnach (139, bo 139-8=131,)
  lcd_dat0(0x01);

  lcd_cmd(SD_PSET);
  lcd_dat0(y);		//poczatek adresowania w wierszach
  lcd_dat0(0xAF);

  lcd_cmd(RAMWR);

  // start data transmission
   lcd_dat16(color);
  PORTB |= _BV(LCD_CS);  // deselect display

  return 0;
}
////////////////////////////////////////////////////////
void ubi2dec( uint16_t val,  char *strg, uint8_t nzero)
/*
 wartosc liczbowa do tablicy, i czy z zerami
  convert unsigned int16 value to a character string
  stringlength has to be 6 character 65536 + the final 0

  if nzero==0, no leading zeros
  if nzero==1, with leading zeros
*/
{
  uint8_t dec,idx=1,odx=0;

  do
  {
    idx--;
    for (dec=0; val>=pot10[idx]; val-=pot10[idx])
    {
      dec++;
    }
    nzero+=dec;
    if (nzero) strg[odx++]='0'+dec;
  } while(idx);
  strg[odx++]='0'+val;
  strg[odx]=0;
}



void put_char(uint8_t x, uint8_t y, char c, uint16_t bgcolor, uint16_t txtcolor)
{
  uint8_t h,ch,p,mask,he;

  PORTB &= ~_BV(LCD_CS);  // select display

  lcd_cmd(SD_CSET);
  lcd_dat0(0x08+x);  // start is 8, not 0
  lcd_dat0(0x01);
  lcd_dat0(0x08+x+CHAR_W-1);  // end is 00x8B = 0x83+8
  lcd_dat0(0x01);

  lcd_cmd(SD_PSET);
  lcd_dat0(y);
  if (y < DISP_H-CHAR_H)
  {
    he=CHAR_H;
    lcd_dat0(y+CHAR_H-1);
  }
  else
  {
    he=DISP_H-y;
    lcd_dat0(DISP_H-1);
  }

  lcd_cmd(RAMWR);

  for (h=0; h<he; h++) // every column of the character
  {
        ch=pgm_read_byte(&ascii_tab[ c-32 ][h]);

        mask=0x80;
        for (p=0; p<CHAR_W; p++)  // write the pixels
        {
          if (ch&mask)
          {
            lcd_dat16(txtcolor);
          }
          else
          {
            lcd_dat16(bgcolor);
          }
          mask=mask/2;
        }  // for p
  }
  PORTB |= _BV(LCD_CS);  // deselect display

}
void print_str(uint8_t x, uint8_t y, char *txt, uint16_t bgcolor, uint16_t txtcolor)
{
  uint8_t i=0;
  while (txt[i]!=0)
  {
    put_char(x+i*CHAR_W,y,txt[i],bgcolor,txtcolor);
    i++;
  }

}
void line(uint8_t x0,uint8_t y0 ,uint8_t x1,uint8_t y1, uint16_t color){

		float m,y,dx,dy; //liczy dlugosc linie x i y

		dy=y1-y0;
		dx=x1-x0;


		m=dy/dx;
		y=y0;

		for(uint8_t i=x0;i<=x1;i++)
		   {
				dot(i,y,color);
				  y+=m;

		   }


}
unsigned char LcdLine ( unsigned char x1, unsigned char x2, unsigned char y1, unsigned char y2, uint16_t color )
{
    int dx, dy, stepx, stepy, fraction;
    unsigned char response;
    //warunek sprawdzajacy czy wymiary nie przekraczaja wymiarow wyswietlacza, jesli tak to wywala errora
    	if ((x1>131) || (x2>131)|| (y1>175)|| (y2>175)) {print_str(0,0,"Zle wymiary LINE", COLOR_WHITE,COLOR_RED);}
    /* Calculate differential form */
    /* dy   y2 - y1 */
    /* -- = ------- */
    /* dx   x2 - x1 */

    /* Take differences */
    dy = y2 - y1;
    dx = x2 - x1;

    /* dy is negative */
    if ( dy < 0 )
    {
        dy    = -dy;
        stepy = -1;
    }
    else
    {
        stepy = 1;
    }

    /* dx is negative */
    if ( dx < 0 )
    {
        dx    = -dx;
        stepx = -1;
    }
    else
    {
        stepx = 1;
    }

    dx <<= 1;
    dy <<= 1;

    /* Draw initial position */
    response = dot( x1, y1, color );
    if(response)
        return response;

    /* Draw next positions until end */
    if ( dx > dy )
    {
        /* Take fraction */
        fraction = dy - ( dx >> 1);
        while ( x1 != x2 )
        {
            if ( fraction >= 0 )
            {
                y1 += stepy;
                fraction -= dx;
            }
            x1 += stepx;
            fraction += dy;

            /* Draw calculated point */
            response = dot( x1, y1, color );
            if(response)
                return response;

        }
    }
    else
    {
        /* Take fraction */
        fraction = dx - ( dy >> 1);
        while ( y1 != y2 )
        {
            if ( fraction >= 0 )
            {
                x1 += stepx;
                fraction -= dy;
            }
            y1 += stepy;
            fraction += dx;

            /* Draw calculated point */
            response = dot( x1, y1, color );
            if(response)
                return response;
        }
    }


    return 0;
}

unsigned char LcdRect ( unsigned char x1, unsigned char x2, unsigned char y1, unsigned char y2, uint16_t color )
{
	unsigned char tmpIdxX,tmpIdxY;
	unsigned char response;

	/* Checking border */
	if ( ( x1 > DISP_W ) ||  ( x2 > DISP_W ) || ( y1 > DISP_H ) || ( y2 > DISP_H ) )
		/* If out of border then return */
		return 1;

	if ( ( x2 > x1 ) && ( y2 > y1 ) )
	{
		for ( tmpIdxY = y1; tmpIdxY < y2; tmpIdxY++ )
		{
			/* Draw line horizontally */
			for ( tmpIdxX = x1; tmpIdxX < x2; tmpIdxX++ )
            {
				/* Draw a pixel */
				response = dot( tmpIdxX, tmpIdxY, color );
                if(response)
                    return response;
            }
		}

	}
    return 0;
}
void kolo (uint8_t x1, uint8_t y1, uint8_t r, uint16_t color){
uint8_t x,y;
float i=0;
const float ii=0.5/r;

	do{
		x=x1;
		y=y1;

	  x+=round(sin(i)*r); //{40 to promieñ okrêgu};
	  y+=round(cos(i)*r);

	   dot(x,y,color);//1
	   dot(x1-(x-x1),y,color);//4
	   dot(x,y1-(y-y1),color);//2
	   dot(x1-(x-x1),y1-(y-y1),color);//3

	   i+=ii;

	}while  (i<M_PI/2);
}

void kolof (uint8_t x1, uint8_t y1, uint8_t r, uint16_t color){
uint8_t x,y;
float i=0;
const float ii=1.0/r;

	do{
		x=x1;
		y=y1;

	  x+=round(sin(i)*r); //{40 to promieñ okrêgu};
	  y+=round(cos(i)*r);



		  for(int b=y1-(y-y1);b<=y;b++){

				  dot(x,b,color);//1
				  dot(x1-(x-x1),b,color);//1

		  }

	   //dot(x,y,color);//1
	  // dot(x1-(x-x1),y,color);//4
	 //  dot(x,y1-(y-y1),color);//2
	 //  dot(x1-(x-x1),y1-(y-y1),color);//3




	   i+=ii;

	}while  (i<M_PI/2);
}

