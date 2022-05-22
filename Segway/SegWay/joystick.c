#include <stdint.h>
#include <stdio.h>
#define F_CPU 32000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "UART.h"
#include "joystick.h"

//joystick
int joyData;
int joySpeedRaw;
int joyYawRaw;





void readJoystick(int *joySpeed, int *joyYaw, int *joySpeedDir, int *joyYawDir, float *out){
	if((USARTF0.STATUS & USART_RXCIF_bm)){
		joyData = ReceiveRegIn(4);
		//if (joyData > 1000)
		//{
		
		joySpeedRaw = ((joyData/10.0));
		if(joySpeedRaw < 100){
			if(joySpeedRaw>55){
				*joySpeed = 10;
				*out = 2.0*0.017;
				*joySpeedDir = 1;
			}
			if(joySpeedRaw<50){
				*joySpeed = -10;
				*out = -2.0*0.017;
				*joySpeedDir = 0;
			}
			if(joySpeedRaw > 50 & joySpeedRaw < 56){
				*joySpeed = 0;
				*out = 0.0;
			}
			if(joyYawRaw == 50){
				*joyYaw = 0;
			}
		}
	}	
}