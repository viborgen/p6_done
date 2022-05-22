#ifndef CONTROLLER_H_
#define CONTROLLER_H_
void PID_pendul(float r, float y,float *ms_p, float *EK1_p, float *EK2_p, float *ms_pK1_p);
void PI_motor(int r, float y, float *PWM_motor, float *PWM1_motor, float *EK1_motor);
void PI_x(float r, float y,float *rads_x, float *EK1_x, float *radsK1_x);
#endif /* CONTROLLER_H_ */