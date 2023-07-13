
#include "3510i.h"
#include "font.h"
#include <util/delay.h>
//Variable für die Farbe
uint8_t lcd_clr[3];

//check LCD version
void readdata()
{

	unsigned char i,j;
	unsigned char ch[6];
	LCD_Out(0x09, 0);
	//cs=0;
	//sclk=0;
	LCD_PORT &= ~((LCD_CLK)+(LCD_CS)+(LCD_SIO));
	LCD_DDR &= ~(LCD_SIO);
	for (i=0;i<6;i++)
	{
		ch[i]=0;
		for (j=0;j<8;j++)
		{

			//sclk=1;
			LCD_PORT |= LCD_CLK;
			if (!(LCD_PORT&(LCD_SIO)))//sdata==0)
			{
				ch[i]=ch[i]|(1<<(7-j));
			}
			//sclk=0;
			LCD_PORT &= ~(LCD_CLK);
		}
	 }

	 switch(ch[0])
	 {
	 	case 255:
			Ver='A';
			break;
		case 127:
			Ver='B';
			break;
		case 0:
			if(ch[1]+ch[2]+ch[3]==0)
			{
				Ver='C';
			}else
			{
				Ver='D';
			}
			break;
	 }
	 //Jakby co rêczenie podmieniaæ typ:
	 //Ver='A';
	 LCD_PORT |= LCD_CS;
	 LCD_DDR |= LCD_SIO;

}
/* Daten an das LCD ausgeben
 * Nach der Beendigung von Daten muss CS kurz auf High gesetzt werden. Da das Manuelle setzen
 * nach jedem letzten Datenpaket zu umständlich ist und die Daten vor einem neuen Command so
 * wieso zu Ende sind, kann auch vor einem neuen Command CS auf high gesetzt werden.
 */
void LCD_Out(uint8_t Data, uint8_t isCmd) {
	uint8_t x;
	if(isCmd) PORTC |= LCD_CS;
	LCD_PORT &= ~(LCD_CLK|LCD_CS);

	LCD_PORT |= LCD_SIO;
	if(isCmd) LCD_PORT &= ~LCD_SIO;

	LCD_PORT |= LCD_CLK;

	for(x=0; x<8; x++)	{
		LCD_PORT &= ~(LCD_SIO|LCD_CLK);
		if(Data & 128) LCD_PORT |= LCD_SIO;
		LCD_PORT |= LCD_CLK;
		Data=Data<<1;
	}
}

/* Farbe für die nächste Zeichenoperation setzen. Es werden alle 3 Byte für die Farbe bestimmt,
 * damit auch das Zeichnen von Doppelpixeln funktioniert. Die Farbwerte sind global definiert.
 * Die drei Bytes repräsentieren die Farben folgendermaßen:
 *  _____________
 * | RRRR   GGGG | 1. Byte, 1. Pixel
 * |      +------+
 * | BBBB | RRRR | 2. Byte, 1/2. Pixel
 * +------+      |
 * | GGGG   BBBB | 3. Byte, 2. Pixel
 *  ¯¯¯¯¯¯¯¯¯¯¯¯¯
 */
void LCD_SetColor(uint8_t red, uint8_t green, uint8_t blue) {
  lcd_clr[1] = blue<<4;

  lcd_clr[0] = red<<4;
	lcd_clr[1] |= red;

  lcd_clr[2] = green<<4;
  lcd_clr[0] |= green;

  lcd_clr[2] |= blue;

  lcd_clr[0] ^= 0xFF;
	lcd_clr[1] ^= 0xFF;
	lcd_clr[2] ^= 0xFF;
}

/* Einen Pixel mit der festgelegten Farbe an die x-/y-Position setzen
 * Zwar darf man, wenn man strikt nach dem Datenblatt geht, nur zwei Pixel auf einem "Zug" setzen,
 * allerdings wird es vom Controller toleriert, wenn man nach dem zweiten Byte abbricht.
 * Netterweise wird dabei das zweite Nibble (Halbbyte) des zweiten Bytes ignoriert, sodass die
 * Rotkomponente des benachbarten Pixels nicht verändert wird.
 * Das ist der Hinsicht praktisch, da man den Displayinhalt nicht parallel im RAM des µCs ablegen
 * muss (was beim ATmega8 nicht möglich ist), da der Inhalt vom Display (noch) nicht gelesen werden kann.
 */
void LCD_Pixel(uint8_t xPos, uint8_t yPos) {
	//x-Koordinate setzen
	LCD_Out(0x2A, 1);
	LCD_Out(xPos, 0);
	LCD_Out(103,0);

	//y-Koordinate setzen
	LCD_Out(0x2B, 1);
	LCD_Out(yPos, 0);
	LCD_Out(83,0);

	//Daten an das Display senden
	LCD_Out(0x2C, 1);
	LCD_Out(lcd_clr[0], 0);
	LCD_Out(lcd_clr[1], 0);
}

/* Den Displayinhalt löschen bzw. den Displayinhalt mit der aktuellen Farbe füllen
 * Hier sieht man, wofür die drei Byte für die Farben notwendig ist.
 * Man könnte zum Löschen zwar auch die Funktion LCD_Pixel verwenden, allerdings
 * ist diese deutlich langsamer, da jedes mal die Position gesetzt werden muss.
 */
void LCD_Clear() {
	uint16_t cnt;
	LCD_Out(0x2C, 1);
	for(cnt = 0; cnt < 3283; cnt++) {
		LCD_Out(lcd_clr[0], 0);
		LCD_Out(lcd_clr[1], 0);
		LCD_Out(lcd_clr[2], 0);
	}
}

/* Eine horizontale Linie zeichnen
 * Die Funktion ist explizit vorhanden, da der Controller horizontale Linien vergleichsweise schnell
 * zeichnen kann, da er mit drei Schreibzyklen zwei Punkte zeichnen kann. Für werden u.a. die drei
 * Farbvariablen benötigt.
 */
void LCD_HLine(uint8_t xPos, uint8_t yPos, uint8_t width) {
	//den letzten Punkt zeichnen, ist zwar nur bei ungerader Breite notwendig, allerdings
	//würde eine if-Abfrage nur unnötig Speicher und Rechenzeit verbrauchen
	LCD_Pixel(xPos + width - 1, yPos);
	//Startpunkt der Linie zeichnen - hier wird eigentlich mehr ausgenutzt, dass der Pointer auf
	//die gewünschte Position gesetzt wird, der Pixel wird in der while-Schleife wieder übermalt.
	LCD_Pixel(xPos, yPos);
	//Breite durch zwei teilen, da immer zwei Pixel auf einem Rutsch gezeichnet werden
  width /= 2;
	//Ausgabe starten
	LCD_Out(0x2C, 1);
	//Pixel in der while-Schleife zeichnen - die Schleife bleibt solange aktiv,
	//bis die Breite auf 0 heruntergezählt wurde.
	while(width) {
		LCD_Out(lcd_clr[0], 0);
		LCD_Out(lcd_clr[1], 0);
		LCD_Out(lcd_clr[2], 0);
    width--;
	}
}

/* Ein Halbbyte (Nibble) aus dem Speicherbereich der Schrift lesen.
 * Dies wird für die Breitenangabe der Schrift benötigt, da hier
 * jeweils zwei Zeichenbreiten in einem Byte zusammengefasst werden
 * Damit können u. U. bis zu 112 Byte Code eingespart werden -
 * Einzige Einschränkung: Die Zeichenbreite darf 15 Pixel nicht übersteigen.
 */
uint8_t getNibble(uint8_t nibble) {
	uint8_t tmp = nibble/2;
	nibble -= tmp*2;
	if(nibble) {
		return pgm_read_byte(&charset[tmp]) & 15;
	} else {
		return pgm_read_byte(&charset[tmp]) >> 4;
	}
}

/* Ein Zeichen an das Display ausgeben
 * Die Ausgabe erfolgt transparent, beim Überschreiben muss also erst der Hintergrund
 * vorher zum Beispiel mit LCD_Rect gefüllt werden.
 * Weiterhin werden proportionale Schriften von Haus aus unterstützt.
 * Standardmäßig ist die Schrifthöhe von 8 Pixeln vorhergesehen, n-fache Höhen sind
 * zwar schon implementiert, allerdings noch gänzlich ungetestet.
 * Die Höhe macht in den meisten Fällen nur bis 3 (24px) Sinn, da ansonsten die Breite
 * nicht mehr ausreichend erhöht werden kann (maximale Breite ist momentan 15px)
 * Als Rückgabewert erhält man die Breite des Zeichens.
 */
uint8_t LCD_Char(uint8_t mychr, uint8_t xPos, uint8_t yPos) {
	mychr -= pgm_read_byte(&(charset[1])); //offset subtrahieren
	uint8_t height = pgm_read_byte(&(charset[0])) & 3;
	height++;
	uint8_t h, x, y, out;
  uint16_t cnt = 0;
	uint8_t width;

	for(x = 0; x < mychr; x++) {
		cnt += getNibble(x+6);
	}
	width = getNibble(x+6);

	cnt += (pgm_read_byte(&(charset[2]))+1)/2; //Anzahl der Definitionsbytes überspringen
	cnt +=3;
	for(h = 0; h < height; h++) {
		for(x = 0; x < width; x++) {
			out = pgm_read_byte(&(charset[cnt+x]));
			for(y = 0; y < 8; y++) {
				if(out & 1) {
					LCD_Pixel(xPos + x, yPos + y);
				}
				out >>= 1;
			}
		}
		yPos += 8;
		cnt+=width;
	}
	return width;
}

/* Die Breite eines einzelnen Zeichens ermitteln
 * Für dem Betrieb nicht zwingend notwendig, allerdings kann man damit relativ schnell
 * herausfinden, wie breit ein Zeichen ist. Ergibt zusammen mit LCD_StringWidth ein
 * perfektes Paar - bitte nur zusammen vermitteln ;)
 */
uint8_t LCD_CharWidth(uint8_t mychr) {
	mychr -= pgm_read_byte(&(charset[1])); //offset subtrahieren
	return getNibble(mychr+6);
}

/* Ermitteln der Breite eines Strings in Pixeln, baut auf LCD_CharWidth auf.
 * Dabei werden die Leerspalten zwischen den einzelnen Zeichen mit beachtet.
 */
uint8_t LCD_StringWidth(uint8_t *s) {
	uint8_t width = 0;
  while(*s) {
		width += LCD_CharWidth(*s);
    width++;
		s++;
	}
  return width-1;
}

/* Einen String an die gegebene Stelle auf das Display zeichnen.
 * Baut auf LCD_Char auf und gibt die Breite des Strings zurück
 */
uint8_t LCD_String(uint8_t *s, uint8_t xPos, uint8_t yPos) {
	uint8_t width = 0;
  while(*s) {
		width += LCD_Char(*s, xPos + width, yPos);
    width++;
		s++;
	}
  return width-1;
}

/* Ein Rechteck an die gegebene Stelle zeichnen und kann es bei Bedarf füllen.
 * Die Funktion setzt auf den wesentlich schnelleren Pendanten zu LCD_Line - LCD_HLine
 * Der Vorteil der Funktion wird an der entsprechenden Stelle beschrieben.
 */
void LCD_Rect(uint8_t xPos, uint8_t yPos, uint8_t width, uint8_t height, uint8_t filled) {
	LCD_HLine(xPos, yPos, width);
	LCD_HLine(xPos, yPos + height - 1, width);
  uint8_t y;
	if(filled == true) {
		for(y = 1; y < height; y++) {
			LCD_HLine(xPos, yPos + y, width);
		}
	} else {
		for(y = 1; y < height; y++) {
			LCD_Pixel(xPos, yPos + y);
			LCD_Pixel(xPos + width - 1, yPos + y);
		}
	}
}

/* Einen Kreis an die Position gegebene Poisition mit dem angegebenen Radius zeichnen
 * Diese Funktion stammt nicht von mir, sondern vom guten Herrn Bresenham - Wikipedia weiß mehr ;)
 * Die Funktion nutzt aus, dass Kreise symmetrisch sind. So muss nur ein Achtel gezeichnet
 * werden - der Rest kann gespiegelt werden
 * Auch hier ist eine Füllfunktion verfügbar, die, wie LCD_Rect, auf LCD_HLine setzt.
 */
void LCD_Circle(int xPos, int yPos, int radius, uint8_t filled) {
	int f = 1 - radius;
	int ddF_x = 0;
	int ddF_y = -2 * radius;
	int x = 0;
	int y = radius;

  if(filled == true) {
    LCD_HLine(xPos - radius, yPos, radius * 2);
	} else {
		LCD_Pixel(xPos, yPos + radius); //unten
		LCD_Pixel(xPos, yPos - radius); //oben
		LCD_Pixel(xPos + radius, yPos); //rechts
		LCD_Pixel(xPos - radius, yPos); //links
	}

	while(x < y) {
		if(f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x + 1;

		if(filled == true) {

      LCD_HLine(xPos - x, yPos - y, 2*x); //oben
			LCD_HLine(xPos - y, yPos - x, 2*y); //oben mitte
      LCD_HLine(xPos - y, yPos + x, 2*y); //unten mitte
      LCD_HLine(xPos - x, yPos + y, 2*x); //unten
		} else {
			LCD_Pixel(xPos + x, yPos + y); //unten rechts
			LCD_Pixel(xPos - x, yPos + y); //unten links
			LCD_Pixel(xPos + x, yPos - y); //oben rechts
			LCD_Pixel(xPos - x, yPos - y); //oben links
			LCD_Pixel(xPos + y, yPos + x); //rechts unten
			LCD_Pixel(xPos - y, yPos + x); //links unten
			LCD_Pixel(xPos + y, yPos - x); //rechts oben
			LCD_Pixel(xPos - y, yPos - x); //links oben
		}
	}
}

/* Eine beliebige Linie zeichnen
 * Hier spielt auch wieder Bresenham die Hauptrolle.
 * Das Feine an dem Algorithmus ist, dass keine Fließkommazahlen nötig sind.
 */
void LCD_Line(int x0, int y0, int x1, int y1) {
	int dy = y1 - y0;
	int dx = x1 - x0;
	int stepx, stepy;

	if (dy < 0) { dy = -dy;  stepy = -1; } else { stepy = 1; }
	if (dx < 0) { dx = -dx;  stepx = -1; } else { stepx = 1; }
	dy <<= 1;
	dx <<= 1;

	LCD_Pixel(x0, y0);
	if (dx > dy) {
		int fraction = dy - (dx >> 1);
		while (x0 != x1) {
			if (fraction >= 0) {
				y0 += stepy;
				fraction -= dx;
			}
			x0 += stepx;
			fraction += dy;
			LCD_Pixel(x0, y0);
		}
	} else {
		int fraction = dx - (dy >> 1);
		while (y0 != y1) {
			if (fraction >= 0) {
				x0 += stepx;
				fraction -= dy;
			}
			y0 += stepy;
			fraction += dx;
			LCD_Pixel(x0, y0);
		}
	}
}

/* Eine abgerundete Box zeichnen. Liebevoll nenne ich sie auch Pille.
 * Um Code zu sparen, wurden einige Werte in die tmp-Variablen ausgelagert. Das chaotisiert
 * den Quelltext zwar ein wenig, allerdings belegt er auf der CPU "nur" 180 Byte.
 * Hierzu werden die Funktionen LCD_Rect, LCD_Circle und damit auch LCD_HLine benötigt.
 */
void LCD_RoundBox(uint8_t xPos, uint8_t yPos, uint8_t width, uint8_t height, uint8_t radius) {
  uint8_t tmp = xPos+width-radius;
  uint8_t tmp2 = height - radius;
  uint8_t tmp2b = yPos + tmp2;
  uint8_t tmp3 = yPos+radius;
  uint8_t tmp4 = xPos + radius;

	LCD_Rect(tmp4, yPos, width - radius * 2, radius, true); //oben
	LCD_Rect(tmp4, tmp2b, width - radius * 2, radius+1, true); //unten
  LCD_Rect(xPos, tmp3, width, tmp2-radius, true); //mitte

	LCD_Circle(tmp, tmp3, radius, true); //rechts oben
  LCD_Circle(tmp4, tmp3, radius, true); //links oben
	LCD_Circle(tmp4, tmp2b, radius, true); //links unten
  LCD_Circle(tmp, tmp2b, radius, true); //rechts unten
}

/* Das Display initialisieren
 * Hier wird alles getan, um das LCD in einen betriebsbereiten Zustand zu bekommen.
 * Ich habe versucht, den Code auf das nötigste zu reduzieren, was mir mehr oder weniger
 * gelungen ist. Hier lässt sich sicherlich noch so manches optimieren...
 */

void LcdInit(void){

	LCD_DDR |= (LCD_CLK|LCD_SIO|LCD_CS|LCD_RST);
	LCD_PORT &= ~(LCD_CLK|LCD_SIO|LCD_CS);
 	//rest=1;
	LCD_PORT |= (LCD_RST);
	//cs=1;
	LCD_PORT |= (LCD_CS);
	//sdata=0;
	LCD_PORT &= ~(LCD_SIO);
	//sclk=1;
	LCD_PORT |= (LCD_CLK);
	//rest=0;
	LCD_PORT &= ~(LCD_RST);
	_delay_ms(50);
	//rest=1;
	LCD_PORT |= (LCD_RST);

	_delay_ms(50);

	LCD_Out(0x00, 1);      // NOP
	_delay_ms(5);

	LCD_Out(0x01, 1);      // LCD Software Reset
	_delay_ms(5);

	LCD_Out(0xC6, 1);      // Initial Escape

	LCD_Out(0xB9, 1);      // Refresh set
	LCD_Out(0x00, 0);

	readdata();

	LCD_Out(0xB5, 1); //Gamma
	LCD_Out(0x01, 0); //


	LCD_Out(0xDB, 1); //comon driver output select

	//Display Setup 1
	 if(Ver=='D')
   {
   		LCD_Out(0x04, 0);
   }else
   {
   		LCD_Out(0x00, 0);
   }

   LCD_Out(0xBE, 1);      // Power Control
   LCD_Out(0x03, 0);


   LCD_Out(0x11, 1);      // Sleep out

   LCD_Out(0xBA, 1);      // Power Control
   LCD_Out(0x7F, 0);
   LCD_Out(0x03, 0);

   LCD_Out(0x36, 1);      //
   if (Ver=='A')
   {
   		 LCD_Out(0x00|0x08,0); 	   //RGB
    }
		else
	{
	  	 LCD_Out(0x00,0); 	   //RGB
	}

   LCD_Out(0xB7, 1);      // Temperature gradient set
   unsigned char i;
   for(i=0; i<14; i++)
   {
      LCD_Out(0, 0);
   }
   LCD_Out(0x29, 1);      //display ON

   LCD_Out(0x03, 1);      // Booster Voltage ON

   _delay_ms(20);         // Booster Voltage stabilisieren lassen

   LCD_Out(0x20, 1);     //display inversion OFF

   LCD_Out(0x25, 1);      // Write contrast
   switch(Ver)							   //¶Ô±È¶ÈÉèÖÃ
   {
   		case 'A':
			LCD_Out(70, 0);  //¶Ô±È¶ÈÉèÖÃ
			break;
   		case 'B':
			LCD_Out(67, 0);  //¶Ô±È¶ÈÉèÖÃ
			break;
		case 'C':
			//LCD_Out(74, 0);
		   	LCD_Out(66, 0);
		//	LCD_Out(64, 0);   //¶Ô±È¶ÈÉèÖÃ
			break;
		case 'D':		  //¶Ô±È¶ÈÉèÖÃ
			LCD_Out(49, 0);
			break;
   }
	LCD_ColorSet(1);

}
void LCD_ColorSet(unsigned char Color)
{

if (Color==1) {
	LCD_Out(0x3a, 1);		//interface pixel format
	LCD_Out(0x03, 0);       //0x03 Îª4096É«£¬0x02Îª256É«

	}
	else
	{
	LCD_Out(0x3a, 1);		//interface pixel format
	LCD_Out(0x02, 0);       //0x03 Îª4096É«£¬0x02Îª256É«



	LCD_Out(0x2d, 1);		//µ÷É«°åÉèÖÃ
	if (Ver=='B'||Ver=='C')
	 {
	  	//red
		LCD_Out(~0x00, 0);
		LCD_Out(~0x02, 0);
		LCD_Out(~0x03, 0);
		LCD_Out(~0x04, 0);
		LCD_Out(~0x05, 0);
		LCD_Out(~0x06, 0);
		LCD_Out(~0x08, 0);
		LCD_Out(~0x0f, 0);

		//green
		LCD_Out(~0x00, 0);
		LCD_Out(~0x02, 0);
		LCD_Out(~0x03, 0);
		LCD_Out(~0x04, 0);
		LCD_Out(~0x05, 0);
		LCD_Out(~0x06, 0);
		LCD_Out(~0x08, 0);
		LCD_Out(~0x0f, 0);
		//blue
		LCD_Out(~0x00, 0);
		LCD_Out(~0x03, 0);
		LCD_Out(~0x06, 0);
		LCD_Out(~0x0f, 0);
	 }else
	{
	  	 //red
		LCD_Out(0x00, 0);
		LCD_Out(0x02, 0);
		LCD_Out(0x03, 0);
		LCD_Out(0x04, 0);
		LCD_Out(0x05, 0);
		LCD_Out(0x06, 0);
		LCD_Out(0x08, 0);
		LCD_Out(0x0f, 0);

		//green
		LCD_Out(0x00, 0);
		LCD_Out(0x02, 0);
		LCD_Out(0x03, 0);
		LCD_Out(0x04, 0);
		LCD_Out(0x05, 0);
		LCD_Out(0x06, 0);
		LCD_Out(0x08, 0);
		LCD_Out(0x0f, 0);
		//blue
		LCD_Out(0x00, 0);
		LCD_Out(0x03, 0);
		LCD_Out(0x06, 0);
		LCD_Out(0x0f, 0);
	}


   }
}
