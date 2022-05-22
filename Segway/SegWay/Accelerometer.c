#include "Accelerometer.h"
#include <avr/io.h>
#include <math.h>

inline void AccToDeg(float dataX, float dataY,float dataZ,float *n){
	*n = atan2f(dataX,(sqrtf((dataY*dataY)+(dataZ*dataZ))))* 180.0/3.14; //Udrenger vinkel i grader ud fra acellerometer data. 
}