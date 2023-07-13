#include <avr/io.h>
void zapal_bit(volatile uint8_t *port, char pin){
*port&=~(1<<pin);
}

int main(void) {
DDRB=0XFF;
PORTB=0xff;
zapal_bit(&PORTB,7);
}
