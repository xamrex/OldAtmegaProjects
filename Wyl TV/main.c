#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main (void){

	char buffer [50];
	  int n, a=5, b=3;
	  sprintf (buffer, "%d plus %d is %d", 221, b, a+b);
	  sprintf (buffer, "%d", 10);

	  return 0;


}
