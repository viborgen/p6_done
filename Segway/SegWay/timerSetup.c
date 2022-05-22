#include <avr/io.h>
#include "timerSetup.h"


void timerINI() {
	//Timers for Regulator timing bruger C1, E1, D1 og D0
	TCC1.CTRLA = 0b0011; //prescaler clk - DIV 2 = 0010 - DIV 4 = 0011 - div 64 =0101 
	TCC1.CTRLB = TC_BYTEM_NORMAL_gc;
	TCC1.PER =  0xFFFF; //bør overvejes om den kan skiftes til ffff (65000)
	
	TCE1.CTRLA = 0b0011; //prescaler clk - DIV 2 = 0010 - DIV 4 = 0011 0111 Div 1024 for at teste hastighedsregulatoren. 
	TCE1.CTRLB = TC_BYTEM_NORMAL_gc;
	TCE1.PER =  0xFFFF; //bør overvejes om den kan skiftes til ffff (65000)
	
	TCD0.CTRLA = 0b0011; //prescaler clk - DIV 2 = 0010 - DIV 4 = 0011
	TCD0.CTRLB = TC_BYTEM_NORMAL_gc;
	TCD0.PER =  0xFFFF; //bør overvejes om den kan skiftes til ffff (65000)
	
	//Timers til PWM
	//C0 E0
	//Timers til enkoder '
	//F0 D1
	//Opsætter events til at trigger på port PC4 Trigger på rising edge
	//enkoder 1.
	PORTC.DIRCLR =PIN4_bm ; //sikre at PC4 er input uden at ændre alle andre pins
	PORTC.PIN4CTRL = 1; // PC4 danner et event på rising edge. 
	EVSYS.CH1MUX = 0b01100100; //sætter event chanel 1 til at kigge på pin PC4, svare til enkoder kanal A
	
	//opsætter timer F0 til at counte på rising edge af PC4. 
	TCF0.CTRLA = 0b00001001; //counter vruger event channel 0 som clock
	TCF0.CTRLD = 0b01001001;  //Event action(count up and down) og Event source(Chanel 1).
	TCF0.PER = 0xFFFF; //størrelse af counter til 2^16 counts. 
	TCF0.CTRLB = TC_BYTEM_NORMAL_gc;
	//PORTC.
	//Opsætter event til at trigger på port PE4
	//enkoder 2.
	
	PORTE.DIRCLR = PIN4_bm; //sikre at PE4 er input uden at ændre alle andre pins
	PORTE.PIN4CTRL = 1; //PE4 danner et event på rising edge.
	EVSYS.CH0MUX = 0b01110100; //Sætter event chanel 0 til at kigge på pin PE4
	
	//Opsætter timer D1 til at counte på rising edge af PE4
	TCD1.CTRLA = 0b00001000; //counter bruger event channel 0 som clock
	TCD1.CTRLD = 0b01001000; //Event action(count up and down) og event source (chanel 0)
	TCD1.PER = 0xFFFF; //Størresle af counter 2^16 counts
	TCD1.CTRLB = TC_BYTEM_NORMAL_gc; //16-bit (unsigned´)	
}