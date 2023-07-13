//////////////////////////////////////////////////////////////////////////////////////////////////
// makra.h - makra pomocnicze
//
// Autor: Rados�aw Koppel          Kompilator: WinAVR 20050214
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef MAKRA_H_INCLUDED
#define MAKRA_H_INCLUDED

// Makra upraszczaj�ce dost�p do port�w
// *** Port
#define PORT(x) XPORT(x)
#define XPORT(x) (PORT##x)
// *** Pin
#define PIN(x) XPIN(x)
#define XPIN(x) (PIN##x)
// *** DDR
#define DDR(x) XDDR(x)
#define XDDR(x) (DDR##x)

// NOPek
#define NOP() {asm volatile("nop"::);}

// Ilo�� element�w tablicy
#define ELEMS(p) (sizeof(p)/sizeof(p[0]))


#endif //MAKRA_H_INCLUDED
