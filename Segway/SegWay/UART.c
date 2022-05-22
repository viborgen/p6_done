#include <avr/io.h>
#include "UART.h"
#include <math.h>

int count = 0;

void InitUSARTF0(){
	PORTF.OUT = 0b00001000; //s�tte pin 3 h�j
	PORTF.DIR = 0b00001000; // s�tte pin 3 til output
	USARTF0.BAUDCTRLA = 12; // s�tter baudrate, valgt ud fra datablad, 9600 bps.
	USARTF0.BAUDCTRLB = 0b01000000; //S�tter baudrate, valgt ud fra datablad, 9600 bps
	USARTF0.CTRLA = 0; //abryder interrupts p� port F
	USARTF0.CTRLC = 0b00000011;  // v�lger asynkron USART, frav�lger parity bit, 1 stop bit, 8 data bits 
	USARTF0.CTRLB = 0b00011000; // t�nder transmitter og reciever
}

void SendChar(char c){
	while(!(USARTF0.STATUS & USART_DREIF_bm)) //sender kun hvis data bufferen er tom
	USARTF0.DATA = c;
}

char ReceiveChar(){
	while(!(USARTF0.STATUS & USART_RXCIF_bm ) & count < 5){ //venter p� en ny character er modtager i databufferen
		count++;
	}
		count =0;
		return USARTF0_DATA;
}

void SendString(char *text) //Sender en string 
{
	while(*text) //G�r igennem alle karaktere i en string
	{
		SendChar(*text++); //incrementer index i string 
	}
	SendChar('\n');
	
}


int ReceiveRegIn(int n){
	int temp=0;

	for (int i = 0;i<n;i++){
		temp = pow(10,n-1)/pow(10,i)*((int)ReceiveChar()-48)+temp;
		
	}
	return temp+1;
}