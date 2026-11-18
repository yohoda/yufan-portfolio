#include "stm32f10x.h"                  // Device header
#include "delay.h" 

void  Key_1_Init(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_1;           //按键四和按键三
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_SetBits( GPIOB,GPIO_Pin_1 | GPIO_Pin_13);
}

void  Key_2_Init(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6;           //按键一和按键二
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_SetBits( GPIOA,GPIO_Pin_7 | GPIO_Pin_6);
}

void  Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//打开GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//打开GPIOA时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;				//定义结构体变量
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_13;//用B1口和B13口
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//输出速度,但输入用不上,写50没影响
	GPIO_Init(GPIOB,&GPIO_InitStructure);				//GPIOB初始化
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6;//用A7口和A6口
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//输出速度,但输入用不上,写50没影响
	GPIO_Init(GPIOA,&GPIO_InitStructure);				//GPIOA初始化
}
/**
  * 函    数：按键获取键码
  * 参    数：无
  * 返 回 值：按下按键的键码值，范围：0~2，返回0代表没有按键按下
  * 注意事项：此函数是阻塞式操作，当按键按住不放时，函数会卡住，直到按键松手
  */
uint8_t Key_GetNumber(void)
{
	uint8_t KeyNum = 0;							//默认为0,没有按键按下返回0
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7) == 0)//读取A7口的输入,如果按下则代表按键1按下
	{
		delay_ms(20);							//按键消抖
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7) == 0);//直到松手往下走
		delay_ms(20);							//按键消抖
		KeyNum = 1;								//返回键码1
	}
	
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6) == 0)//读取A6口的输入,如果按下则代表按键2按下
	{
		delay_ms(20);							//按键消抖
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6) == 0);//直到松手往下走
		delay_ms(20);							//按键消抖
		KeyNum = 2;								//返回键码2
	}

	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) == 0)//读取B1口的输入,如果按下则代表按键3按下
	{
		delay_ms(20);							//按键消抖
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) == 0);//直到松手往下走
		delay_ms(20);							//按键消抖
		KeyNum = 3;								//返回键码3
	}

	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) == 0)//读取B13口的输入,如果按下则代表按键4按下
	{
		delay_ms(20);							//按键消抖
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) == 0);//直到松手往下走
		delay_ms(20);							//按键消抖
		KeyNum = 4;								//返回键码3
	}
	return KeyNum;
}
