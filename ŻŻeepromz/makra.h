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

// Zamiana dw�ch element�w
#define XCHG(a, b, var_type) \
{\
	var_type temp##a##b; \
	temp##a##b = b; \
	b = a; \
	a = temp##a##b;	\
}
// /\ Ta konstrukcja daje mi 100 % pewno�ci, �e nazwa zmiennej tymczasowej b�dzie inna ni� zmiennych a i b

// Wyb�r warto�ci maksymalnej i minimalnej
#define MAX(a, b) (((a)>=(b)) ? (a) : (b))
#define MIN(a, b) (((a)<=(b)) ? (a) : (b))

// Dost�p do pami�ci zewn�trznej
#define EXMEM __attribute__ ((section (".exram")))

#endif //MAKRA_H_INCLUDED
