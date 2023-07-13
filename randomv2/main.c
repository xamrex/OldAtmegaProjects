#include <avr/io.h>
#include <stdlib.h>
#include "LCD/lcd44780.h"


uint8_t wylosowana;

int main(void){
lcd_init();
lcd_cls();
lcd_locate(0,0);

wylosowana=rand();
lcd_int(RAND_MAX);
}

