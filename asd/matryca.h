/*
 * main.h
 *
 *  Created on: 2011-07-28
 *       Autor: XamreX
 */
#include <avr/eeprom.h>

#ifndef MAIN_H_
#define MAIN_H_

#define MOSI PB5
#define SCK PB7
#define CS PB4

#define tryb 1 // 0 dla znakow 1 dla bufora !!! (powtorzyc w matryca.c) !!!

#define r1 PA0
#define r2 PA1
#define r3 PA2
#define r4 PA3
#define r5 PA4
#define r6 PA5
#define r7 PA6
#define r8 PA7


//oblsuga klawisza
#define K1_pin (1<<PB3)
#define K1_down !(PINB & K1_pin)

#define max_dl_zdania 40 //Potrzebne do zarezerowowania odpowiednio duzej tablicy
#if tryb==1
uint8_t  bufor[256];	//rezerowanie tablicy mowiacej jaki piksel ma sie swieci
#endif

uint8_t txt[max_dl_zdania]; //tabela przechowywuj¹ca tekst który uklazuje siê na wyœwitlaczu
uint8_t dlugosczdania; // zmienna ktora przechowuje dlugosc tekstu(ilosc liter)

volatile uint8_t nrwysw; //zmienna do wlaczania wybranej kolumny wyswietlacza
volatile uint32_t znak,znak1,znak2,znak3,znak4,znak5,znak6,znak7,znak8,znak9;
volatile uint8_t przesow;
volatile uint8_t timer;
volatile uint8_t coileprzesuw;
volatile uint8_t flagaprzesuwu;

void SendSpi(uint32_t bajt);
void InitSpi(void);
void InitTimer(void);
void clear (void);
void pixel(uint8_t a, uint8_t b);
void update(void);
void napis (char * str);
void dlugosc(void);
void skopiuj(void);
void porty_init(void);
void sprawdzcfg(void);

uint32_t przesuniecie(uint32_t liczba, int8_t ile);


#endif /* MAIN_H_ */
