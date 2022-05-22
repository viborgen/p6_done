#include <avr/io.h>
#include "enkoder.h"

float encoderHastighed(){
	float speed = ((TCF0_CNT * 1.2109240)/100);  //0.0010585/(1/1144) = 1.2109240//0.0010585/(1/1000)=1.0585    //0.0010585/0.0004166666667 = 2.5404); //0.0010585 = værdi der omregner antallet af interrupts til cm/s ved en tidsperide//0.00041666 = 2.4 KHz
	//der deles med 100 for at opnå m/s i stedet for cm/s
	TCF0_CNT= 0;
	return speed;
}
float encoder2Hastighed(){
	float speed2 = ((TCD1_CNT* 1.2109240)/100);
	TCD1_CNT= 0;
	return speed2;
}