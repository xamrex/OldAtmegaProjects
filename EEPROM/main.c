#include <avr/io.h>
#include "LCD/lcd44780.h"
#include <avr/delay.h>
#include <avr/eeprom.h>
#define KEY_PIN (1<<PB7)
#define KEY_DOWN !(PINB & KEY_PIN)
uint8_t powtorzenie;
//uint8_t zeeprom;
int main(void){
powtorzenie=eeprom_read_byte(1);
DDRB &= ~KEY_PIN;
PORTB |= KEY_PIN;

_delay_ms(10);
while(1){
if (KEY_DOWN){
powtorzenie++;
eeprom_write_byte(1, powtorzenie);
_delay_ms(200);
}

lcd_init();
lcd_str("Linia2");
lcd_locate(1,0);
lcd_int(powtorzenie);




}
}

