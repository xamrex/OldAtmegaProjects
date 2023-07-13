/*
 * zegar.c
 *
 *  Created on: 2011-03-11
 *       Autor: Miros³aw Kardaœ
 */

#include <avr/io.h>
#include "I2C_SOFT/i2c_soft.h"
#include "LCD/lcd44780.h"
#include "headery.h"


void rtc_odczyt (void){
char *DniTyg[8]={
		"Pn", "Wt", "\x80r", "Cz", "Pt", "So", "Nd"
};
uint8_t znak_S[]={4,14,16,16,14,1,17,14};
lcd_defchar(0x80, znak_S); //Dodanie znaku Œ do wysw.

	//odczyt 4 bajtów do bufora od adresu 0x01 z pamiêci RAM naszego RTC
			I2C_read_buf( PCF8583_ADDR, 0x01, 8, bufor );
			/* ODCZYTYWANIE SEK,MIN,GODZ */
			sekundy = bcd2dec( bufor[1] );
			minuty = bcd2dec( bufor[2] );
			godziny = bcd2dec( bufor[3] );
			/* ODCZYTYWANIE ROKU */
			rokfree = (bufor[7]*4);
			rokfree = (rokfree +(bufor[4]>>6));
			/* ODCZYTYWANIE DNI MCA(1-31) */
			dnimca = (0b00111111 & bufor[4]);
			dnimca = bcd2dec(dnimca);
			/* ODCZYTYWANIE NR MCA(1-12) */
			nrmca = (0b00011111 & bufor[5]);
			nrmca = bcd2dec(nrmca);
			/* ODCZYTYWANIE NAZWY DNIA(Pn-Nd) */
			dztyg = (bufor[5]>>5);
			// wyœwietlenie czasu na LCD
			lcd_locate(0,0);
			if( godziny < 10 ) lcd_str("0");
			lcd_int(godziny);
			lcd_str(":");
			if( minuty < 10 ) lcd_str("0");
			lcd_int(minuty);
			lcd_str(":");
			if( sekundy < 10 ) lcd_str("0");
			lcd_int(sekundy);


			lcd_locate(1,0);
			lcd_str(DniTyg[dztyg]); //wusw nazwednia (pn-nd)
			lcd_str(":");
			if (dnimca<10) lcd_str("0");
			lcd_int(dnimca); //wysw. dzien miesiaca (1-31)
			lcd_str("-");
			if (nrmca<10) lcd_str("0");
			lcd_int(nrmca); //wysw. dzien miesiaca (1-12)
			lcd_str("-");
			lcd_str("20");
			if (rokfree<10) lcd_str("0");
			lcd_int(rokfree); //wysw. rok

			// POPRAWIENIE WYSW. ROKU //

				if (nrmca==12 && dnimca==31 && godziny==23 && minuty==59 && sekundy==59 &&((bufor[4]>>6) ==3)){
					I2C_read_buf( PCF8583_ADDR, 0x08, 1, bufor );
					rokfree = bufor[7];
					bufor[0]=rokfree+1;
					I2C_write_buf( PCF8583_ADDR, 0x08, 1, bufor );
			}


//				PORTB=~sekundy;

				PORTB=(PORTB & 0b11000000)|(~sekundy & ~0b11000000); //maskowanie aby wykorzystac jeszce 2 wolne bity
				PORTD ^=(1<<PC3); //dwukropek

}

