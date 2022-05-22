#include <avr/io.h>
#include <math.h>
#include <stdio.h>
#include "komFilter.h"




//ACCELEROMETER
float ACCx, ACCy, ACCz;
float lsbGy = 131.0;
uint8_t GY[6];
float degAcc = 0;

//GYRO
float GYx, GYy, GYz;
float lsbAcc= 16384.0; // float for at kunne dele med en int således en float fås
uint8_t ACC[6];
float T_gyro = 0.00413 ; //242 Hz = 0.00413// 180 Hz = 0.005

//Komplimentærfilter
float degKomp = 0;
float k_y = 0.98;
float Angle_y = 0;





void KomFilter(float *Angle_y) {
	for (int i = 59;i<=64;i++) {// læser register 59-64, indeholder AccXYZ data. Til senere Gyro reg 67-72. Sæt for loop til at gå fra 69-64 for acc
		ACC[i-59] = readReg(i); //ACC[0]:x, ACC[2]:y, ACC[4]:z er msb
	}
	for (int i = 67;i<=72;i++) {// læser register 59-64, indeholder AccXYZ data. Til senere Gyro reg 67-72. Sæt for loop til at gå fra 69-64 for acc
		GY[i-67] = readReg(i); //ACC[0]:x, ACC[2]:y, ACC[4]:z er msb
	}
	GYy = ((((GY[2]<<8) |GY[3])/lsbGy)); 
		
	ACCx = (((((ACC[0]<<8) |ACC[1])/lsbAcc))); //bit shifter 8 gange for at lave plads til 16 bit for så at or de 8 lsb ind, så f.eks. ACC[0]=01010101 og A[1]=10101010. 8 gange shift på A[0] << 8 =0101010100000000 |10101010=0101010110101010
	ACCy = ((((ACC[2]<<8 |ACC[3])/lsbAcc)));
	ACCz = ((((ACC[4]<<8 |ACC[5])/lsbAcc)));
			
	AccToDeg(ACCx,ACCy,ACCz,&degAcc); //Beregner vinklen fra accelerometeret
			
	degKomp = (k_y*(degKomp+(-GYy*2*T_gyro))+(1-k_y)*degAcc); //REgner vinklen med bidrag fra både gyro og acc 0.017 convetere fra grader til rad
	
	
	*Angle_y = degKomp;
}