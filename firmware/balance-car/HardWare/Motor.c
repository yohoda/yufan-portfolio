#include "stm32f10x.h"                  // Device header
#include "motor.h"
#include "PWM.h"
extern int16_t Speed1,Speed2;
/**
  * 函    数：直流电机初始化
  * 参    数：无
  * 返 回 值：无
  */
void Motor_L_Init(void)
{
	/*开启左电机控制电路时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//开启GPIOA的时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);						//将PA4和PA5引脚初始化为推挽输出	
	
	//PWM_2_Init();												//初始化直流电机的底层PWM		
}
void Motor_R_Init(void)
{	/*开启右电机控制电路时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开启GPIOB的时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//PWM_1_Init();												//初始化直流电机的底层PWM
}

void Motor_Init(void)
{
	Motor_L_Init();
	Motor_R_Init();
	PWM_Init();
}	


/**
  * 函    数：直流电机设置速度
  * 参    数：Speed 要设置的速度，范围：-100~100
  * 返 回 值：无
  */
void Motor_SetSpeed(int16_t Speed1, int16_t Speed2)
{	

	if (Speed1 >= 0)							//如果设置正转的速度值
	{	
		GPIO_SetBits(GPIOA, GPIO_Pin_5);	//PA5置高电平
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);	//PA4置低电平，设置方向为正转
						//PWM设置为速度值
	}
	else									//否则，即设置反转的速度值
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);	//PA5置低电平
		GPIO_SetBits(GPIOA, GPIO_Pin_4);	//PA4置高电平，设置方向为反转
		Speed1=(-Speed1);			//PWM设置为负的速度值，因为此时速度值为负数，而PWM只能给正数
	}
	if (Speed2 >= 0)							//如果设置正转的速度值
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_14);	//PB14置高电平
		GPIO_ResetBits(GPIOB, GPIO_Pin_15);	//PB15置低电平，设置方向为正转
						//PWM设置为速度值
	}
	else									//否则，即设置反转的速度值
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_14);	//PB14置低电平
		GPIO_SetBits(GPIOB, GPIO_Pin_15);	//PB15置高电平，设置方向为反转
		Speed2=(-Speed2);			//PWM设置为负的速度值，因为此时速度值为负数，而PWM只能给正数
	}
	  if(g_cMotorDisable)//如果电机停止转动标志为1,则停止转动，否则正常输出PWM
     {
         PWM_SetCompare4(0);
		 PWM_SetCompare1(0);
     }
     else //否则正常输出PWM
     {
		PWM_SetCompare4(Speed1);
        PWM_SetCompare1(Speed2);
     }
	
	
	
	
}

/**
  * 函数功能：电机的正反转
  * 入口参数：
  * 返回  值：无
  */


void Forward(void)
{
		Fore=1;	
}
void Backward(void)
{
		Back=1;	
			
	}
void Turnleft(void)
{
		Left=1;	
}
void Turnright(void)
{
		Right=1;
}
//void SpinLeft(void)   //左轮后退右轮前进    
//{
//		Motor_SetSpeed(Speed,-Speed);	;
//}
//void SpinRight(void)   //右轮前进左轮后退    
//{
//		Motor_SetSpeed(-Speed,Speed);	
//}


void Limit(int *motoA,int *motoB)
{
	if(*motoA>PWM_MAX)*motoA=PWM_MAX;
	if(*motoA<PWM_MIN)*motoA=PWM_MIN;
	
	if(*motoB>PWM_MAX)*motoB=PWM_MAX;
	if(*motoB<PWM_MIN)*motoB=PWM_MIN;
}

/*绝对值函数*/
int GFP_abs(int p)
{
	int q;
	q=p>0?p:(-p);
	return q;
}

/*赋值函数*/
/*入口参数：PID运算完成后的最终PWM值*/
void Load(int moto1,int moto2)//moto1=-200：反转200个脉冲
{
	//1.研究正负号，对应正反转
	if(moto1>0)	
    Ain1=1,Ain2=0;//正转
	else 				
    Ain1=0,Ain2=1;//反转
	//2.研究PWM值
	TIM_SetCompare1(TIM1,GFP_abs(moto1));
	
  //1.研究正负号，对应正反转
	if(moto2>0)
    Bin1=1,Bin2=0;
	else 				
    Bin1=0,Bin2=1;	
  //2.研究PWM值
	TIM_SetCompare4(TIM1,GFP_abs(moto2));
}
char PWM_Zero=0,stop=0;
void Stop(float *Med_Jiaodu,float *Jiaodu)
{
	if(GFP_abs(*Jiaodu-*Med_Jiaodu)>30)
	{
		Motor_SetSpeed(PWM_Zero,PWM_Zero);
		stop=1;
	}
}
