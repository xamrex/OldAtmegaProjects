/*
 * headery.h
 *
 *  Created on: 2011-03-11
 *       Autor: Miros³aw Kardaœ
 */

#ifndef HEADERY_H_
#define HEADERY_H_

/****************** zegar.c************************************/
#define PCF8583_ADDR 0xA2
#define dwukropek (1<<PD3)
uint8_t bcd2dec(uint8_t bcd);
void rtc_odczyt (void);

char nazwadnia;
uint8_t bufor[9];		// rezerwacja bufora 4 bajty
uint8_t sekundy, minuty, godziny,dztyg,dnimca,rokmod,rok,nrmca,rokfree;
uint8_t flaga,Timer1, Timer2, Timer3, Timer4; // zmienne timerow

/****************** main.c************************************/

uint8_t dec2bcd(uint8_t dec);
void zapis(void);


#endif /* HEADERY_H_ */
