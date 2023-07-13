#include 	<avr/io.h>
#include 	<avr/interrupt.h>
#include 	<inttypes.h>
#include 	<avr/eeprom.h>
#include	<avr/sleep.h>
#include	<stdlib.h>
#include 	<math.h>
#include 	<avr/pgmspace.h>
#include <util/delay.h>
#include "3510i.h"


#define EEPROM  __attribute__ ((section (".eeprom")))
#define FLASH   __attribute__ ((section (".progmem.gcc")))

int main(void) {
  uint8_t x;


LcdInit(); //LCD initialisieren



  LCD_SetColor(0, 0, 0); //Farbe auf schwarz setzen
  LCD_Clear(); //Display füllen

  //Farbverläufe erstellen
  for(x = 0; x < 16; x++) {
    //roten Farbverlauf erstellen
	  LCD_SetColor(x, 0, 0); //von schwarz bis rot
    LCD_Rect(x*2, 0, 2, 4, true);

    LCD_SetColor(15, x, x); //von rot bis weiß
    LCD_Rect(x*2+32, 0, 2, 4, true);

    //grünen Farbverlauf erstellen
	  LCD_SetColor(0, x, 0); //von schwarz bis grün
    LCD_Rect(x*2, 4, 2, 4, true);

    LCD_SetColor(x, 15, x); //von grün bis weiß
    LCD_Rect(x*2+32, 4, 2, 4, true);

    //grünen Farbverlauf erstellen
	  LCD_SetColor(0, 0, x); //von schwarz bis grün
    LCD_Rect(x*2, 8, 2, 4, true);

    LCD_SetColor(x, x, 15); //von grün bis weiß
    LCD_Rect(x*2+32, 8, 2, 4, true);

    //grauen Farbverlauf erstellen
    LCD_SetColor(15-x, 15-x, 15-x);
    LCD_Rect(x*2+64, 0, 2, 12, true);
  }

	LCD_SetColor(15, 15, 0); //gelb

  //Eine Box mit zwei Diagonalen zeichnen
	LCD_Rect(0, 14, 30, 17, false);
  LCD_Line(0, 14, 29, 30);
  LCD_Line(29, 14, 0, 30);

  LCD_SetColor(15, 12, 0); //orange
  LCD_Circle(65, 30, 15, true); //Einen gefüllten Kreis zeichnen

  LCD_SetColor(15, 0, 15); //pink
  LCD_Circle(65, 30, 10, false); //Einen nicht gefüllten Kreis zeichnen

  LCD_SetColor(10, 10, 10); //ca. 70% grau
  LCD_Rect(2, 35, 25, 15, false); //Ein nicht gefülltes Rechteck zeichnen

  LCD_SetColor(5, 5, 5); //ca. 30% grau
  LCD_Rect(15, 40, 25, 5, true); //Ein gefülltes Rechteck zeichnen

  LCD_SetColor(12, 14, 15); //etwa himmelblau

  uint8_t mytext[] = "Wersja -  ";
  mytext[9] = Ver;
  x= LCD_StringWidth(mytext); //Breite des Strings ermitteln
  LCD_RoundBox(1, 53, x+14, 10, 2); //Eine Abgerundete Box zeichnen

  LCD_SetColor(0, 0, 0); //schwarz
  LCD_String(mytext, 3, 55);



	while(1) {

	}

	return 0;
}
