/*
 * odczytgodzin.h
 *
 *  Created on: 2011-06-10
 *       Autor: XamreX
 */

#ifndef ODCZYTGODZIN_H_
#define ODCZYTGODZIN_H_

#define PCF8583_ADDR 0xA2

uint8_t dec2bcd(uint8_t dec);
uint8_t bcd2dec(uint8_t bcd);
uint8_t bsek[1];
uint8_t bmin[1];
uint8_t bgodz[1];

void zapisgodziny (void);
void aktualizacja(void);
void odczytgodzinyLCD(void);
void odczytgodzinyNokia(void);
void odczytgodzinyNokiaanalog(void);

#endif /* ODCZYTGODZIN_H_ */
