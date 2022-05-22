#include <avr/io.h>
#include <math.h>

//PI hastigheds regulator
float T_x = 1.0/572.0;
float k_p_x = 120, k_i_x = 960;
void PI_x(float r, float y,float *rads_x, float *EK1_x,float *radsK1_x){
	float EK_x = r-y;
	*rads_x = EK_x*(k_p_x+k_i_x*T_x)-k_p_x*(*EK1_x)+*radsK1_x;
	*radsK1_x = *rads_x;
	*EK1_x = EK_x;
}

//PI motor regulator
float T_motor = 1.0/1144.0;
float k_p_motor = 2300, k_i_motor = 7314;
void PI_motor(int r, float y, float *PWM_motor, float *PWM1_motor, float *EK1_motor){
	float EK_motor = r-y;
	*PWM_motor = EK_motor*(k_p_motor+k_i_motor*T_motor)-k_p_motor*(*EK1_motor)+*PWM1_motor;
	*PWM1_motor = *PWM_motor;
	*EK1_motor = EK_motor;
}

//PID pendul regulator
float T_pendul = 1.0/242.0; 
float k_p_p = 2.7, k_i_p = 21.6, k_d_p = 0.0001; //HUSL. KP 5.5 KD 0.0007
int count;

void PID_pendul(float r, float y,float *ms_p, float *EK1_p, float *EK2_p, float *ms_pK1_p){
	if(count > 5 ){
		r = r/5;
		count = 0;
	}
	float EK_p = r - y; //Finder nuværende fejl
	*ms_p = EK_p*(k_p_p+k_i_p*T_pendul+k_d_p/T_x)+*EK1_p*((-k_p_p)-2*k_d_p/T_pendul)+*EK2_p*k_d_p/T_pendul + *ms_pK1_p; //beregner nyt output
	*ms_pK1_p = *ms_p; //nuværende output gemmes : U(k-1) = U(k)
	*EK2_p = *EK1_p; //gammel fejl gemmes igen : E(k-2) = E(k-1)
	*EK1_p = EK_p; //nuværende fejl gemmes : E(k-1) = E(k)
	if (fabs(y) > 20*0.017)
	{
		*ms_p = 0;
	}
	count++;
}
