#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "delay.h"

void  Servo_Init(void)
{
	PWM_Init();
}

//0 500，
//180 2500

void Servo_Set_Angle(float Angle)
{
	PWM_SetCompare2(Angle/180*2000+500);
	
}

void Servo_Swing(void)                //2秒摆头一次
{
	for (int angle = 0; angle <= 180; angle++)
		{Servo_Set_Angle(angle);
            //OLED_ShowNum(1,7,angle,3);
			Delay_us(5524);
          
        }
        // 减少角度从180度到0度，共181步
        for (int angle = 180; angle >= 0; angle--) 
		{Servo_Set_Angle(angle);
         // OLED_ShowNum(1,7,angle,3);
			Delay_us(5524);
		}
}
