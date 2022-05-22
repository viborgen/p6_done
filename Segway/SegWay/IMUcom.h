/*
 * IMUcom.h
 
 * Created: 29-03-2022 21:10:33
 *  Author: jakob
 */ 


#ifndef IMUCOM_H_
#define IMUCOM_H_

void wakeUpIMU(void);
void writeReg(uint8_t addr, uint8_t data);
uint8_t readReg(uint8_t addr);


#endif /* IMUCOM_H_ */