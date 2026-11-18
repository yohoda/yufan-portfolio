#ifndef __MOTOR_H
#define __MOTOR_H
extern int PWM_MAX,PWM_MIN;	// PWM限幅变量
extern int16_t Speed1,Speed2;
void Motor_Init(void);
void Motor_SetSpeed(int16_t Speed1,int16_t Speed2);
void Forward(void);
void Backward(void);
void Turnleft(void);
void Turnright(void);
void SpinLeft(void);  
void SpinRight(void); 

void Limit(int *motoA,int *motoB);
int GFP_abs(int p);
void Load(int moto1,int moto2);
void Stop(float *Med_Jiaodu,float *Jiaodu);
#include "sys.h" 

#define Ain1  PAout(5)
#define Ain2  PAout(4)

#define Bin1  PBout(14)
#define Bin2  PBout(15)

#endif
