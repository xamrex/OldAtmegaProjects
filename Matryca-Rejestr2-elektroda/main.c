#include <avr/io.h>
#include "matryca.h"
#include <util/delay.h>

int main(void){

	InitSpi();
	InitTimer();
	SendSpi(0);
	napis("ARTUR JEST MADRY");


		while(1) // tutaj w kolko przesuwam text
		{
				for (uint8_t i=0;i<(dlugosczdania*6)+41;i++){
					przesow=i;
					_delay_ms(55);

				 }
		}
}
