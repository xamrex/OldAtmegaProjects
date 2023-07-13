/*
 * main.h
 *
 *  Created on: 2011-07-28
 *       Autor: XamreX
 */

#ifndef MAIN_H_
#define MAIN_H_

#define MOSI PB5
#define SCK PB7
#define CS PB4


#define max_dl_zdania 40 //Potrzebne do zarezerowowania odpowiednio duzej tablicy

uint8_t txt[max_dl_zdania]; //tabela przechowywuj¹ca tekst który uklazuje siê na wyœwitlaczu
uint8_t dlugosczdania; // zmienna ktora przechowuje dlugosc tekstu(ilosc liter)

volatile uint8_t nrwysw; //zmienna do wlaczania wybranej kolumny wyswietlacza
volatile uint8_t przesow;

void SendSpi(uint32_t bajt);
void InitSpi(void);
void InitTimer(void);
void napis (char * str);
uint32_t przesuniecie(uint32_t liczba, int8_t ile);


#endif /* MAIN_H_ */
