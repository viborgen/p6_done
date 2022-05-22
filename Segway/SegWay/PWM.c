#include <stdint.h>
#include <stdio.h>
#define F_CPU 32000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "PWM.h"

int periode = 1650;

void pwm_init(){
	PORTC_DIRSET = PIN1_bm;          //Vælger PIN C1 som output pin for PWM signal
	TCC0_PER = periode;           //Anvender periode
	TCC0_CTRLB |= 0x03;           //Single slope mode
	TCC0_CTRLB |= 0x20;           //channel selection CCBEN //CCB enable
	TCC0_CTRLA |= 0x01;           //clock selection clk/1
}

void pwm_init2(){
	PORTE_DIRSET = PIN1_bm;          //Vælger PIN C1 som output pin for PWM signal
	TCE0_PER = periode;           //Anvender periode
	TCE0_CTRLB |= 0x03;           //Single slope mode
	TCE0_CTRLB |= 0x20;           //channel selection CCBEN //CCB enable
	TCE0_CTRLA |= 0x01;           //clock selection clk/1
}

void InitMotorDriver(void){
	/***** nødvendigt for motordriver ******/
	//definere de pins der anvendes i motordriver
	PORTC.DIRSET = (1 << 0);
	PORTC.DIRSET = (1 << 2);
	PORTC.DIRSET = (1 << 3);
	//always high pins defineres og aktiveres
	PORTB.DIRSET = (1 << 7);
	PORTF.DIRSET = (1 << 7);
	PORTF.OUTSET = PIN7_bm; //enable
	PORTB.OUTSET = PIN7_bm;// slew
	
	//Always low
	PORTC.OUTCLR = PIN0_bm; //d1
	
	//init inputs motor 2
	PORTE.DIRSET = (1 << 3);
	PORTE.DIRSET = (1 << 2);
	
	//always low init motor 2
	PORTE.DIRSET = (1 << 0);
	PORTE.OUTCLR = PIN0_bm;
}


void PWM(int direction, float dutycycle){ //PWM funktion til motor1 
	float duty = 0; //nulstiller dutycycle
	if(direction == 1){ //fremad
		PORTC.OUTSET = PIN3_bm; //input 2
		PORTC.OUTCLR = PIN2_bm; //input 1
	}
	else{ //baglens
		PORTC.OUTCLR = PIN3_bm; //input 2
		PORTC.OUTSET = PIN2_bm; //input 1
	}
	pwm_init(); //pwm kaldes Prøv at fjerne denne til ud for at sparer tid
	
	if(dutycycle > 65000){ //hvis regulator returnere PWM værdi over 65000 cutter den der
		duty = 65000;
	}
	else if(dutycycle < 0){ //negative PWM værdier fra regulator bliver 0
		duty = 0;
	}
	else{ //hvis intet af ovenstående er gældende er det fint
		duty = dutycycle;	
	}
	TCC0_CCBBUF = periode*(duty/65000);       //Duty cycle vælges i register
	while((TCC0_INTFLAGS & 0x01) == 0);     //wait for the new compare value to be loaded
	TCC0_INTFLAGS = 0x00;                   //clear the interrupt flag
}

void PWM2(int direction, float dutycycle){ //PWM funktion motor2
	float duty = 0;
	if(direction == 1){ //fremad
		PORTE.OUTSET = PIN2_bm; //input 2
		PORTE.OUTCLR = PIN3_bm; //input 1
	}
	else{ //baglens
		PORTE.OUTCLR = PIN2_bm; //input 2
		PORTE.OUTSET = PIN3_bm; //input 1
	}
	
	pwm_init2(); //pwm kaldes
	
	if(dutycycle > 65000){
		duty = 65000;
	}
	else if(dutycycle < 0){
		duty = 0;
	}
	else{
		duty = dutycycle;
	}
	
	TCE0_CCBBUF = periode*(duty/65000);       //Duty cycle vælges i register
	while((TCE0_INTFLAGS & 0x01) == 0);     //wait for the new compare value to be loaded
	TCE0_INTFLAGS = 0x00; //clear the interrupt flag
}


