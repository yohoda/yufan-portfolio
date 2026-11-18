#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"
#include "Ultrasonic.h"
#include "timer.h"
#include "delay.h"

#define ECHO_GPIO_PORT    GPIOA
#define ECHO_GPIO_PIN     GPIO_Pin_12

int EXTI_flag=0;
void EXTI_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	//	开启GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);		//	开启功能复原IO时钟
 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);	
	GPIO_InitStruct.GPIO_Pin = ECHO_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; //GPIO_Mode_IN_FLOATING;	//	浮空输入
	GPIO_Init(ECHO_GPIO_PORT,&GPIO_InitStruct);				//	根据GPIO_InitStruct中指定的参数初始化外设GPIOA寄存器
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource12);	//	选择GPIOA管脚的1管脚用作外部中断线
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line12;					//	选中外部中断线0
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;		//	设置EXTI线路为中断请求
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	//	上升沿，下降沿都触发
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;	//	使能
	EXTI_Init(&EXTI_InitStruct);		//	根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	//选择中断向量	下面很重要
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    //抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    //响应优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
    //使能中断向量
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    //初始化中断向量配置
    NVIC_Init(&NVIC_InitStructure);
	
}
void  EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line12))				//	检查指定的EXTI线路触发请求发生与否
    {
		if(GPIO_ReadInputDataBit(ECHO_GPIO_PORT, ECHO_GPIO_PIN) == 1)
		{
			TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);		//	
			//USART1_printf(USART1,"\r\nwan\r\n");
			//delay_ms(10000);
			//EXTI_flag = 1;
		}				
		else
		{
			TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);
			//USART1_printf(USART1,"\r\ntin\r\n");
			//EXTI_flag = 0;
		}
		
		EXTI_ClearITPendingBit(EXTI_Line12);			//	清除EXTI线路挂起位
       
    }
 
} 
