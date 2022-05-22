#define loop_until_bit_is_set(sfr, bit) do { } while (bit_is_clear(sfr, bit))

#define F_CPU 32000000UL

#include <avr/io.h>
#include <math.h>
#include <stdio.h>
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>
#include "UART.h"
#include "softI2C.h"
#include "IMUcom.h"
#include "DataConversion.h"
#include "Controller.h"
#include "PWM.h"
#include "Accelerometer.h"
#include "timerSetup.h"
#include "enkoder.h"

//Regulator variabler
//Pendul PID
float ref = 0.0;float deg;float ms_p = 0;float EK1_pendul = 0;float EK2_pendul = 0;float ms_pk1 = 0;
//Komplimentærfiltervinkel
float Angle_y;
//X PI
	//PI X1
	float rads_x = 0;float EK1_x = 0;float radsK1_x = 0;
	//PI X2
	float rads_x2 = 0;float EK1_x2 =0;float radsK1_x2 = 0;
//Motor PI
	//PI Motor1
	float PWM1_motor = 0;float EK1_motor = 0;float PWMvalue;
	//PI Motor2
	float PWM1_motor2 = 0;float EK1_motor2 = 0;float PWMvalue2;
//Motorstyring samt enkoderdata
int dir = 0; //retning på motorer
float encoderAflaest, encoder2Aflaest;
//joystik
int joySpeed;int joyYaw;int joySpeedDir;int joyYawDir;float out;
int joyCount = 0;


void ChgCPUFreq(void){
	OSC.CTRL = (1 << 1);
	loop_until_bit_is_set(OSC.STATUS, 1); // venter på 32 MHz stabiliseres
	//_delay_ms(1);
	CCP = (0xD8 << 0); //ændrings beskyttelse
	//_delay_ms(1);
	CLK.CTRL = (1 << 0); //ændrer clock
}

int main(void)
{
	ChgCPUFreq(); //Skifter CPU frekvens til 32MHz 
	InitUSARTF0();//starter Uart F0
	wakeUpIMU(); //Starter IMU
	writeReg(28,0b00000000);//´sætter acelerometer scale til +-2 G
	InitMotorDriver(); //Starter Motordriver 
	pwm_init(); //Starter PWM
	pwm_init2();
	timerINI(); //Starter timer
	
	
    while (1) 
    {	///Vinkelregulator///
		if (TCE1_CNT > 33058) //33058 = 242 Hz //44444 = 180 Hz 
		{	
			TCE1_CNT = 0; //Nulstiller Tælleren 
		    KomFilter(&Angle_y); //Henter komplementær vinkel
			deg = Angle_y;
			//omregner vinkel til radianer. 
			deg = deg*0.017;
			if (encoderAflaest > 0.3) //resser robotten således den ikke vælter Pga. moment.
			{
				ref=0;
			}
			
			PID_pendul(ref,deg,&ms_p,&EK1_pendul,&EK2_pendul,&ms_pk1); //Udregner input til hastugheds regulator
			
			///Bestemmer retning af motor./// 
			if ( ms_p > 0 ){
				PWM(dir,0);
				PWM2(dir,0);
				dir = 1;
				
			}
			else if(ms_p < 0) {
				PWM(dir,0);
				PWM2(dir,0);
				dir = 0;
				ms_p = ms_p*-1;
				
			}
			joyCount++; //Software counter til at læse joystick
			
		}
		if(joyCount > 34){ //240/35 = 7 Hz 
					if (USARTF0.STATUS & USART_RXCIF_bm) //checker om der er modtaget data på UART
					{
						readJoystick(&joySpeed,&joyYaw,&joySpeedDir,&joyYawDir,&out); //Læster Data
						ref = out; //sæter input til vinkelregulator til input fra joystick
					}
					joyCount = 0;	//restter joystick counter
		}
		
		///Hastighedsregulator///
		if(TCC1_CNT > 13986){ //13986 = 572 Hz  //16000 = 500 Hz //6153 = 1.3KHz
			TCC1_CNT = 0; //Nulstiller tæller
			PI_x(ms_p,encoderAflaest,&rads_x,&EK1_x,&radsK1_x); //Udregner input til vinkelhastigheds regulatoren
			PI_x(ms_p,encoder2Aflaest,&rads_x2,&EK1_x2,&radsK1_x2);
		}
		if(TCD0_CNT > 6993){ //6993 = 1144 Hz
			TCD0_CNT = 0; //Nulstiller tæller 
			encoderAflaest = encoderHastighed(); //Aflæser hastighed ud fra enkoderne
			encoder2Aflaest = encoder2Hastighed();
			PI_motor(rads_x,encoderAflaest*61, &PWMvalue,&PWM1_motor,&EK1_motor); //Udregner mængden af PWM til motorne
			PI_motor(rads_x2,encoder2Aflaest*61, &PWMvalue2,&PWM1_motor2,&EK1_motor2);
		}
		PWM(dir,PWMvalue); //styrrer Motor PWM og retning. 
		PWM2(dir,PWMvalue2);
    }
}