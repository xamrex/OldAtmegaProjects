/*
 * ir_decode.h
 *
 *  Created on: 2010-09-13
 *       Autor: Miros³aw Kardaœ
 */

#ifndef IR_DECODE_H_
#define IR_DECODE_H_

#define IR_PORT D
#define IR_PIN 6
#define IR_IN (1<<IR_PIN)

// sta³e i zmienne potrzebne na wewnêtrzne cele procedury obs³ugi RC5

#define TIMER1_PRESCALER 8

// przeliczanie tykniêæ Timera1 przy danym preskalerze na mikrosekundy
#define ir_micro_s(num) ((num)*(F_CPU/1000000)/TIMER1_PRESCALER)

// sta³e czasowa i tolerancja wyra¿one w us
#define TOLERANCE 200
#define MIN_HALF_BIT 	ir_micro_s(889	- TOLERANCE)
#define MAX_HALF_BIT 	ir_micro_s(889 + TOLERANCE)
#define MAX_BIT ir_micro_s((889+889) + TOLERANCE)




// Makra upraszczaj¹ce dostêp do portów
// *** PORT
#define PORT(x) XPORT(x)
#define XPORT(x) (PORT##x)
// *** PIN
#define PIN(x) XPIN(x)
#define XPIN(x) (PIN##x)
// *** DDR
#define DDR(x) XDDR(x)
#define XDDR(x) (DDR##x)





// ------------------------------------------- definicje klawiszy PILOTA IR

#define OK 23
#define cursor_left 21
#define cursor_right 22
#define cursor_up 16
#define cursor_down 17
// ------------------------------------------- definicje klawiszy KONIEC

// zmienne charakterystyczne tylko dla obs³ugi RC5
extern volatile uint8_t toggle_bit;		// bit TOGGLE
extern volatile uint8_t address;		// adres
extern volatile uint8_t command;		// komenda

// flaga = 1 - informuje, ¿e odebrany zosta³ nowy kod z pilota
extern volatile uint8_t Ir_key_press_flag;

// deklaracje funkcji dla u¿ytkownika
void ir_init();


#endif /* IR_DECODE_H_ */
