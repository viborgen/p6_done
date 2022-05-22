/*
 * PWM.h
 *
 * Created: 06-04-2022 14:51:48
 *  Author: jakob
 */ 


#ifndef PWM_H_
#define PWM_H_
void pwm_init(void);
void pwm_init2(void);
void InitMotorDriver(void);
void PWM(int direction, float dutycycle);
void PWM2(int direction, float dutycycle);



#endif /* PWM_H_ */