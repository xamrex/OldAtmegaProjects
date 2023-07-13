/*
 * rs232.h
 *
 *  Created on: 2011-08-17
 *       Autor: XamreX
 */

#ifndef RS232_H_
#define RS232_H_
void USART_Transmit (unsigned char data);
void USART_napis (char * str);
unsigned char USART_Recive(void);
void USART_init(uint32_t baud);

#endif /* RS232_H_ */
