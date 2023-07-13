/*-------------------------------------------------------------------------
  Example main program for LCD PCD8544 (LCD of Nokia 3310)


  Copyright (c) 2008, Fandi Gunawan <fandigunawan@gmail.com>
   http://fandigunawan.wordpress.com

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (__at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

   In other words, you are welcome to use, share and improve this program.
   You are forbidden to forbid anyone else to use, share and improve
   what you give them.   Help stamp out software-hoarding!

-------------------------------------------------------------------------*/
/*
   Many thanks to Jakub Lasinski for reviewing and for giving
   suggestion to my code
*/


#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>


#include "pcd8544.h"
#include "main.h"


int main()
{
    LcdInit();


    //display image
    LcdContrast(88);
    LcdClear();
 //  LcdImage(waitImage);
  // LcdPixel(1,1,PIXEL_ON);
  // LcdPixel(1,2,PIXEL_ON);
  // LcdUpdate();
  // LcdPixel(2,9,PIXEL_ON);


  // LcdLine(2,17,30,32,PIXEL_ON);
   LcdUpdate();
  //  LcdGotoXYFont(1,1);
    //  LcdFStr(FONT_1X,(unsigned char*)PSTR("Test"));




}
