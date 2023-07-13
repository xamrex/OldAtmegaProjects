#ifndef USART_H_INCLUDED_
#define USART_H_INCLUDED_

// UBRR = ( F_CPU / (16 * BAUD) ) - 1
// UBRR=12 => FCPU=4MHz & BAUD=19200 ; Error=0,2%
// UBRR=25 => FCPU=4MHz & BAUD=9600 ; Error=0,2%
// UBRR=12 => FCPU=8MHz & BAUD=38400 ; Error=0,2%
#define USART_UBRR 12

void usart_Init(void);
void usart_SendChar(uint8_t c);
void usart_SendStr(char* str);
void usart_SendStrP(prog_char* str);
void usart_SendDec(int val);
void usart_SendHexByte(uint8_t data);
void usart_SendBinByte(uint8_t bin);

#endif
