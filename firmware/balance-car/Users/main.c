#include "stm32f10x.h"
#include "bsp.h"
int PWM_MAX=7200,PWM_MIN=-7200;	// PWM限幅变量
int MOTO1,MOTO2;

float Pitch,Roll,Yaw;	          // Pitch：俯仰角，Roll：横滚角，Yaw：偏航角
short gyrox,gyroy,gyroz;        // 角速度
short aacx,aacy,aacz;           // 加速度
int Encoder_Left,Encoder_Right; // 编码器数据（速度）
// 

//uint8_t KeyNum;		//定义用于接收按键键码的变量
//int16_t Speed1,Speed2,Speed;		//定义速度变量
//float angle;
//int main(void)
//{
//	
//	OLED_ShowString(0,0,"Speed:",OLED_8X16);
//	angle=Get_Current_Angle();
//OLED_Printf(0,16,OLED_8X16,"%.3f", s_balance_pid.Kp);
//		OLED_Printf(0,32,OLED_8X16,"%.3f",  angle);
//	OLED_Update();
//	
//	while(1)
//	{
//		if(0 == KEY1)
//		{ 
//		
//			Ultrasonic();
//		//delay_ms(10);
//			if(0 == KEY1)
//			{
//				while(0 == KEY1);
//				USART1_printf(USART1,"\r\n\r\n");
//		
//		KeyNum = Key_GetNumber();				//获取按键键码
//		if (KeyNum == 1)					//按键1按下
//		{	OLED_ShowSignedNum(50,16,Speed1,3,OLED_8X16);
//			Speed1 += 100;					//速度变量自增20
//			if (Speed1 > 7100)				//速度变量超过100后
//			{
//				Speed1= -7100;				//速度变量变为-100
//											//此操作会让电机旋转方向突然改变，可能会因供电不足而导致单片机复位
//									//若出现了此现象，则应避免使用这样的操作
//			}
//			
//		}
//		if (KeyNum == 2)					//按键1按下
//		{	OLED_ShowSignedNum(50,32,Speed2,3,OLED_8X16);
//			Speed2 += 100;					//速度变量自增20
//			if (Speed2 > 7100)				//速度变量超过100后
//			{
//				Speed2= -7100;				//速度变量变为-100
//											//此操作会让电机旋转方向突然改变，可能会因供电不足而导致单片机复位
//											//若出现了此现象，则应避免使用这样的操作
//			}
//		}
//		if (KeyNum == 3)					//按键1按下
//		{	OLED_ShowSignedNum(50,16,Speed1,3,OLED_8X16);
//			Speed1 -= 100;					//速度变量自增20
//			if (Speed1 < -7100)				//速度变量超过100后
//			{
//				Speed1= 7100;				//速度变量变为-100
//											//此操作会让电机旋转方向突然改变，可能会因供电不足而导致单片机复位
//											//若出现了此现象，则应避免使用这样的操作
//			}
//		}
//		if (KeyNum == 4)					//按键1按下
//		{	OLED_ShowSignedNum(50,32,Speed2,3,OLED_8X16);
//			Speed2 -= 100;					//速度变量自增20
//			if (Speed2 < -7100)				//速度变量超过100后
//			{
//				Speed2= 7100;				//速度变量变为-100
//											//此操作会让电机旋转方向突然改变，可能会因供电不足而导致单片机复位
//											//若出现了此现象，则应避免使用这样的操作
//			}
//		}
//		Motor_SetSpeed(Speed1,Speed2);				//设置直流电机的速度为速度变量

//		OLED_ShowNum(0,16,g_distance_mm,3,OLED_8X16);
//		OLED_ShowSignedNum(0,32,Encoder_GetSpeed(TIM2,0.005),3,OLED_8X16);	
//		OLED_ShowSignedNum(0,48,-Encoder_GetSpeed(TIM4,0.005),3,OLED_8X16);
//		OLED_Update();
//		//			}
//			while(0 == KEY1);
//		angle=Get_Current_Angle();
//		OLED_Printf(0,16,OLED_8X16,"%.3f", s_balance_pid.Kp);
//		OLED_Printf(0,32,OLED_8X16,"%.3f",  angle);
//		BalanceCar();
//		OLED_Update();
//		//Process_Command((char*)Serial_RxPacket);
//		//menu_do();
//		}
//	}



int main(void)	
{	
	bsp_Init();
  while(1)	
	{	
		menu_do();
		OLED_Update();
	} 	
}
