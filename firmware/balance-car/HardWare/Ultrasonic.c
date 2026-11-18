#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "Ultrasonic.h"
#include "delay.h"

#define TRIG_GPIO_PORT    GPIOA
#define TRIG_GPIO_PIN     GPIO_Pin_15
#define ECHO_GPIO_PORT    GPIOA
#define ECHO_GPIO_PIN     GPIO_Pin_12
#define SERVO_GPIO_PORT   GPIOB
#define SERVO_GPIO_PIN    GPIO_Pin_0   // TIM3_CH3

 uint32_t g_distance_mm = 0;
const uint32_t TIM3_ARR = 19999;  // 20ms周期

volatile uint32_t echo_start = 0;
volatile uint32_t echo_end = 0;


// TIM3初始化（同时用于PWM和超声波计时）
void TIM3_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    // 配置PB0为复用推挽输出（TIM3通道3）
    GPIO_InitStructure.GPIO_Pin = SERVO_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SERVO_GPIO_PORT, &GPIO_InitStructure);

    // 时基配置（1MHz时钟，20ms周期）
    TIM_TimeBaseStructure.TIM_Period = 19999;        // 20ms
    TIM_TimeBaseStructure.TIM_Prescaler = 71;        // 72MHz/72=1MHz
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    // PWM通道配置
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 1500;            // 初始1.5ms
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);         // 通道3初始化

    TIM_Cmd(TIM3, ENABLE);
    TIM_CtrlPWMOutputs(TIM3, ENABLE);
}

// 设置舵机角度（0-180度）
void Servo_SetAngle(float angle)
{	
	angle = angle > 180 ? 180 : (angle < 0 ? 0 : angle);
    uint16_t Compare = (uint16_t)((angle / 180.0f) * 2000.0f + 500.0f);
    TIM_SetCompare3(TIM3, Compare);
}

void EXTI_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    
    // 配置ECHO引脚
    GPIO_InitStruct.GPIO_Pin = ECHO_GPIO_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(ECHO_GPIO_PORT, &GPIO_InitStruct);
    
    // 外部中断配置
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource12);
    EXTI_InitStruct.EXTI_Line = EXTI_Line12;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line12))
    {
        if(GPIO_ReadInputDataBit(ECHO_GPIO_PORT, ECHO_GPIO_PIN))
        {
            // 上升沿记录起始时间
            echo_start = TIM3->CNT;
        }
        else
        {
            // 下降沿计算时间差
            echo_end = TIM3->CNT;
            uint32_t duration = (echo_end >= echo_start) ? 
                              (echo_end - echo_start) : 
                              (20000 - echo_start + echo_end);
            
            // 计算距离（0.34mm/us / 2 = 0.17mm/us）
            g_distance_mm = (uint32_t)(duration * 0.17f);
        }
        EXTI_ClearITPendingBit(EXTI_Line12);
    }
}

void Ultrasonic_Trigger(void)
{
    GPIO_ResetBits(TRIG_GPIO_PORT, TRIG_GPIO_PIN);
    delay_us(2);
    GPIO_SetBits(TRIG_GPIO_PORT, TRIG_GPIO_PIN);
    delay_us(15);
    GPIO_ResetBits(TRIG_GPIO_PORT, TRIG_GPIO_PIN);
}

void Ultrasonic_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // 释放PA15的JTAG功能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    // 配置Trig引脚
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = TRIG_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TRIG_GPIO_PORT, &GPIO_InitStructure);
    
    TIM3_Init();
    EXTI_Config();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}

uint32_t Get_Distance(void)
{
    return g_distance_mm;
}

void Ultrasonic(void)
	
{
		// 舵机扫描测试
        for(int angle = 0; angle <= 180; angle += 30)
	{
            Servo_SetAngle(angle);
            delay_ms(500);
            
            // 触发测距
            Ultrasonic_Trigger();
            delay_ms(100);  // 等待测量
            
            uint32_t dist = Get_Distance();
			//OLED_ShowNum(2,1,dist,16);
	}
		
}
