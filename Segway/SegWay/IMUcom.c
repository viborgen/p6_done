#include <avr/io.h>
#include "softI2C.h"
#include "IMUcom.h"

uint8_t nack;
uint8_t data;

void wakeUpIMU(void){
	STWI_Start( &PORTA,1,2); // SDA pin1, SCL pin2
	STWI_WriteByte(0b11010000);
	STWI_WriteByte(0x6b);
	STWI_WriteByte(0);
	STWI_Stop();
}

void writeReg(uint8_t addr, uint8_t data){
	STWI_Start( &PORTA,1,2);
	nack = STWI_WriteByte(0b11010000);
	if (nack != 1)
	{
		STWI_WriteByte(addr);
		STWI_WriteByte(data);
		STWI_Stop();
	}
}

uint8_t readReg(uint8_t addr){
	STWI_Start(&PORTA,1,2);
	STWI_WriteByte(0b11010000);
	STWI_WriteByte(addr);
	
	STWI_Restart();
	STWI_WriteByte(0b11010001);
	
	data = STWI_ReadByte(STWI_NACK);
	STWI_Stop();
	return data;
}

