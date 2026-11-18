#include "sys.h"
#include "key.h"
#include "stm32f10x.h"                  // Device header 

unsigned int cnt=0;
/**
  * @brief  定时器4初始化（1ms中断示例）
  * @param  arr : 自动重装载值（决定定时周期）
  * @param  psc : 预分频系数（时钟分频）
  * @note   时钟源频率 = APB1总线时钟 * 2 = 72MHz（当APB1 prescaler≠1时）
  *         定时器时钟 = 72MHz / (psc + 1)
  *         定时周期 = (arr + 1) * (1/定时器时钟)
  *         示例：arr=7199, psc=0 → 1ms中断（72MHz/(0+1)/(7199+1)=10kHz → 0.1ms）
  */
void TIM4_Init(uint16_t arr, uint16_t psc)
{
    TIM_TimeBaseInitTypeDef  TIM_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    /* 1. 使能定时器时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    /* 2. 配置时基单元 */
    TIM_InitStruct.TIM_Period = arr;         // 自动重装载值
    TIM_InitStruct.TIM_Prescaler = psc;       // 预分频系数
    TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;  // 时钟分频（与滤波相关）
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
    TIM_TimeBaseInit(TIM4, &TIM_InitStruct);

    /* 3. 使能更新中断 */
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);  // 使能更新中断

    /* 4. 配置NVIC */
    NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;  // 调整优先级更合理
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    /* 5. 启动定时器 */
    TIM_Cmd(TIM4, ENABLE);
}

/**
  * @brief  TIM4全局中断服务函数
  * @note   中断周期由TIM4_Init参数决定
  */
//void TIM4_IRQHandler(void)
//{
//    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
//    {
//        cnt++;  // 中断计数器（可用于计时）
//        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  // 清除中断标志
//    }
//}

/**
  * @brief  定时器3初始化（1ms中断示例）
  * @param  arr : 自动重装载值（决定定时周期）
  * @param  psc : 预分频系数（时钟分频）
  * @note   时钟源频率 = APB1总线时钟 * 2 = 72MHz（当APB1 prescaler≠1时）
  *         定时器时钟 = 72MHz / (psc + 1)
  *         定时周期 = (arr + 1) * (1/定时器时钟)
  *         示例：arr=7199, psc=0 → 1ms中断（72MHz/(0+1)/(7199+1)=10kHz → 0.1ms）
  */
void TIM3_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
 
    //定时器TIM3初始化
    TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值   
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
    TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE ); //失能指定的TIM3中断,允许更新中断
 
    //中断优先级NVIC设置
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 8;  //从优先级8级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
 
 
    TIM_Cmd(TIM3, ENABLE);  //使能TIM3                    
}
////定时器3中断服务程序
//void TIM3_IRQHandler(void)   //TIM3中断
//{
//    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
//    {
//		cnt++;
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  	//清除TIM3更新中断标志
//    }
//}
// 



/**
  * 函    数：定时中断初始化
  * 参    数：无
  * 返 回 值：无
  */
void TIM2_Init(void)
{
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);			//开启TIM2的时钟
	
	/*配置时钟源*/
	TIM_InternalClockConfig(TIM2);		//选择TIM2为内部时钟，若不调用此函数，TIM默认也为内部时钟
	
	/*时基单元初始化*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//定义结构体变量
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//计数器模式，选择向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;				//计数周期，即ARR的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;				//预分频器，即PSC的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;			//重复计数器，高级定时器才会用到
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);				//将结构体变量交给TIM_TimeBaseInit，配置TIM2的时基单元	
	
	/*中断输出配置*/
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);						//清除定时器更新标志位
																//TIM_TimeBaseInit函数末尾，手动产生了更新事件
																//若不清除此标志位，则开启中断后，会立刻进入一次中断
																//如果不介意此问题，则不清除此标志位也可
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);					//开启TIM2的更新中断
	
	/*NVIC中断分组*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				//配置NVIC为分组2
																//即抢占优先级范围：0~3，响应优先级范围：0~3
																//此分组配置在整个工程中仅需调用一次
																//若有多个中断，可以把此代码放在main函数内，while循环之前
																//若调用多次配置分组的代码，则后执行的配置会覆盖先执行的配置
	
	/*NVIC配置*/
	NVIC_InitTypeDef NVIC_InitStructure;						//定义结构体变量
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;				//选择配置NVIC的TIM2线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//指定NVIC线路的抢占优先级为2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//指定NVIC线路的响应优先级为1
	NVIC_Init(&NVIC_InitStructure);								//将结构体变量交给NVIC_Init，配置NVIC外设
	
	/*TIM使能*/
	TIM_Cmd(TIM2, ENABLE);			//使能TIM2，定时器开始运行
}

/* 定时器中断函数，可以复制到使用它的地方
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
*/
