/*
 * UART.h
 *
 * Created: 29-03-2022 21:01:31
 *  Author: jakob
 */ 


#ifndef UART_H_
#define UART_H_

void InitUSARTF0(void);
void SendChar(char c);
char ReceiveChar();
void SendString(char *text);
void InitUSARTD0(void);
void SendCharD0(char c);
char ReceiveCharD0();
void SendStringD(char *text);
int ReceiveRegIn(int n);


#endif /* UART_H_ */