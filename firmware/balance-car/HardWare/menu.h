#ifndef __MENU_H__
#define __MENU_H__

int menu1(void);
int menu2_Motor(void);
int menu2_YunTai(void);
int menu2_YaoGan(void);
int menu2_MPU6050(void);
int menu2_PID(void);
int menu2_Clock(void);
int menu2_Music(void);
int menu2_Set(void);
void menu_do(void);
extern uint8_t KeyNum;	
extern float Pitch,Roll,Yaw;
extern PID_Controller s_balance_pid, s_speed_pid, s_turn_pid;
extern int menu2;
#endif
