////////////////////////////////////////////////////////////////////////////////////////////
// harddef.h - definicja sprzętu dla programu testującego możliwości wyświetlacza z noki3310
//     przeznaczenie: "Programowanie procesorów w języku C (EdW)"
//
////////////////////////////////////////////////////////////////////////////////////////////
#ifndef HARDDEF_H_INCLUDED
#define HARDDEF_H_INCLUDED

// Przypisanie wyprowadzeń wyświetlacza
#define LCD_PORT B
#define LCD_RES 4
#define LCD_CS 3
#define LCD_SIO 5
#define LCD_SCK 7

// Sprzętowy SPI
#define LCD_HARDWARE_SPI

#endif //HARDDEF_H_INCLUDED
