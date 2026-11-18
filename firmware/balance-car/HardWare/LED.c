#include "stm32f10x.h"                  // Device header

/***********************************************
 * 宏定义：LED引脚配置（便于扩展）
 ***********************************************/
#define LED1_PIN     GPIO_Pin_1
#define LED2_PIN     GPIO_Pin_2
#define LED_GPIO     GPIOA

/***********************************************
 * 函数名：LED_Init
 * 功能  ：初始化所有LED的GPIO引脚
 * 参数  ：无
 * 备注  ：推挽输出，默认高电平（LED灭）
 ***********************************************/
void LED_Init(void)
{	
    // 1. 开启GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // 2. 配置GPIO结构体
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin   = LED1_PIN | LED2_PIN;  // 同时初始化两个引脚
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;     // 推挽输出
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_GPIO, &GPIO_InitStruct);
    
    // 3. 初始状态：高电平（LED灭）
    GPIO_SetBits(LED_GPIO, LED1_PIN | LED2_PIN);
}

/***********************************************
 * 函数名：LED_ON
 * 功能  ：点亮指定LED
 * 参数  ：GPIO_Pin_x（如LED1_PIN）
 ***********************************************/
void LED_ON(uint16_t led_pin)
{
    GPIO_ResetBits(LED_GPIO, led_pin);  // 低电平点亮（假设共阳接法）
}

/***********************************************
 * 函数名：LED_OFF
 * 功能  ：熄灭指定LED
 * 参数  ：GPIO_Pin_x
 ***********************************************/
void LED_OFF(uint16_t led_pin)
{
    GPIO_SetBits(LED_GPIO, led_pin);    // 高电平熄灭
}

/***********************************************
 * 函数名：LED_Toggle
 * 功能  ：翻转指定LED状态
 * 参数  ：GPIO_Pin_x
 * 优化点：使用输出寄存器读取，确保状态准确
 ***********************************************/
void LED_Toggle(uint16_t led_pin)
{
    // 读取当前输出状态（非输入状态！）
    if (GPIO_ReadOutputDataBit(LED_GPIO, led_pin)) {
        GPIO_ResetBits(LED_GPIO, led_pin);  // 当前为高电平→熄灭，切换为低电平
    } else {
        GPIO_SetBits(LED_GPIO, led_pin);     // 当前为低电平→点亮，切换为高电平
    }
}
