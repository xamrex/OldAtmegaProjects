//////////////////////////////////////////////////////////////////////////////////////////////////
// makra.h - makra pomocnicze
//
// Autor: Rados³aw Koppel          Kompilator: WinAVR 20050214
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef MAKRA_H_INCLUDED
#define MAKRA_H_INCLUDED

// Makra upraszczaj¹ce dostêp do portów
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

// Iloœæ elementów tablicy
#define ELEMS(p) (sizeof(p)/sizeof(p[0]))

// Zamiana dwóch elementów
#define XCHG(a, b, var_type) \
{\
	var_type temp##a##b; \
	temp##a##b = b; \
	b = a; \
	a = temp##a##b;	\
}
// /\ Ta konstrukcja daje mi 100 % pewnoœci, ¿e nazwa zmiennej tymczasowej bêdzie inna ni¿ zmiennych a i b

// Wybór wartoœci maksymalnej i minimalnej
#define MAX(a, b) (((a)>=(b)) ? (a) : (b))
#define MIN(a, b) (((a)<=(b)) ? (a) : (b))

// Dostêp do pamiêci zewnêtrznej
#define EXMEM __attribute__ ((section (".exram")))

#endif //MAKRA_H_INCLUDED
