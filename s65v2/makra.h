// ------------- Makra
#ifndef MAKRA_H_INCLUDED
#define MAKRA_H_INCLUDED

// *** PORT
#define PORT(x) XPORT(x)
#define XPORT(x) (PORT##x)
// *** PIN
#define PIN(x) XPIN(x)
#define XPIN(x) (PIN##x)
// *** DDR
#define DDR(x) XDDR(x)
#define XDDR(x) (DDR##x)

#define NOP() {asm volatile("nop"::);}

#endif
