#include "stm32f10x.h"
#include "Trace.h"
#include "sys.h"
#include "motor.h"
#include "delay.h"
#include "pid.h"
#define SPEED_Y 40  
 float g_turn_target=0.0f;

typedef enum {
    LINE_FOLLOW,
    CROSSING,
    SHARP_TURN
} TraceState;
static TraceState trace_state = LINE_FOLLOW;

#define FILTER_COUNT 3 // 连续3次检测相同状态才响应
static struct {
    int l1_count;
    int l2_count;
    int r1_count;
    int r2_count;
} filter;

/**
* Function       Trace_Init
* @brief         巡线传感器GPIO初始化接口
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   无
*/
void Trace_Init(void)
{	// 禁用JTAG/SWD，释放PC13~PC15
//	AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_DISABLE; // 慎用！会禁用调试接口
//	RCC->BDCR &= ~RCC_BDCR_RTCEN;  // 关闭RTC
//	RCC->BDCR &= ~RCC_BDCR_LSEON;  // 关闭LSE晶振
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
#ifdef USE_LINE_L1
	/*开启左一外设时钟*/
	RCC_APB2PeriphClockCmd(LineWalk_L1_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = LineWalk_L1_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化PORT*/
  	GPIO_Init(LineWalk_L1_PORT, &GPIO_InitStructure);		 
#endif

#ifdef USE_LINE_L2
	/*开启左二外设时钟*/
	RCC_APB2PeriphClockCmd(LineWalk_L2_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = LineWalk_L2_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化PORT*/
  	GPIO_Init(LineWalk_L2_PORT, &GPIO_InitStructure);	
#endif

#ifdef USE_LINE_R1
	/*开启右一外设时钟*/
	RCC_APB2PeriphClockCmd(LineWalk_R1_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = LineWalk_R1_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化PORT*/
  	GPIO_Init(LineWalk_R1_PORT, &GPIO_InitStructure);	
#endif
	
#ifdef USE_LINE_R2
	/*开启右二外设时钟*/
	RCC_APB2PeriphClockCmd(LineWalk_R2_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = LineWalk_R2_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化PORT*/
  	GPIO_Init(LineWalk_R2_PORT, &GPIO_InitStructure);	
#endif
	PWR_BackupAccessCmd(ENABLE);//允许修改RTC 和后备寄存器
    RCC_LSEConfig(RCC_LSE_OFF);//关闭外部低速外部时钟信号功能 后，PC13 PC14 PC15 才可以当普通IO用。
    BKP_TamperPinCmd(DISABLE);//关闭入侵检测功能，也就是 PC13，也可以当普通IO 使用
    PWR_BackupAccessCmd(DISABLE);//禁止修改后备寄存器
}


/**
* Function       Trace_Get
* @brief         获取巡线状态
* @param[in]     int *p_iL1, int *p_iL2, int *p_iR1, int *p_iR2  四路巡线位指针
* @param[out]    void
* @retval        void
* @par History   无
*/
void Trace_Get(int *p_iL1, int *p_iL2, int *p_iR1, int *p_iR2)
{
	*p_iL1 = GPIO_ReadInputDataBit(LineWalk_L1_PORT, LineWalk_L1_PIN);
	*p_iL2 = GPIO_ReadInputDataBit(LineWalk_L2_PORT, LineWalk_L2_PIN);
	*p_iR1 = GPIO_ReadInputDataBit(LineWalk_R1_PORT, LineWalk_R1_PIN);
	*p_iR2 = GPIO_ReadInputDataBit(LineWalk_R2_PORT, LineWalk_R2_PIN);		
}

/**
* Function       LineWalking  
* @brief         巡线模式运动
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   无
*/
void LineWalking(void)
{
	int LineL1 = 1, LineL2 = 1, LineR1 = 1, LineR2 = 1;
    static uint32_t crossing_timer = 0;

    // 获取传感器原始数据
    Trace_Get(&LineL1, &LineL2, &LineR1, &LineR2);

    // 新增：传感器滤波处理
    filter.l1_count = (LineL1==0) ? (filter.l1_count+1) : 0;
    filter.l2_count = (LineL2==0) ? (filter.l2_count+1) : 0;
    filter.r1_count = (LineR1==0) ? (filter.r1_count+1) : 0;
    filter.r2_count = (LineR2==0) ? (filter.r2_count+1) : 0;

    // 确定有效状态（需连续检测到）
    int valid_L1 = (filter.l1_count >= FILTER_COUNT);
    int valid_L2 = (filter.l2_count >= FILTER_COUNT);
    int valid_R1 = (filter.r1_count >= FILTER_COUNT);
    int valid_R2 = (filter.r2_count >= FILTER_COUNT);

    // 状态机处理
    switch(trace_state)
    {
    case LINE_FOLLOW: // 常规巡线
        if(valid_L2 && valid_R1) // 正常在线上
        {
            g_turn_target = 0.0f;
            Target_Speed = SPEED_Y;
        }
        else if(valid_R2) // 右急弯
        {
            trace_state = SHARP_TURN;
            g_turn_target = 80.0f; // 右大转弯
            Target_Speed = SPEED_Y * 0.6;
        }
        else if(valid_L1) // 左急弯
        {
            trace_state = SHARP_TURN;
            g_turn_target = -80.0f; // 左大转弯
            Target_Speed = SPEED_Y * 0.6;
        }
        else if(valid_L2) // 微左偏
        {
            g_turn_target = -30.0f;
        }
        else if(valid_R1) // 微右偏
        {
            g_turn_target = 30.0f;
        }
        else if(!valid_L2 && !valid_R1) // 脱线处理
        {
            // 保持最后转向方向
            g_turn_target = (g_turn_target > 0) ? 50.0f : -50.0f;
            Target_Speed = SPEED_Y * 0.4;
        }

        // 检测到四路口
        if(valid_L1 && valid_L2 && valid_R1 && valid_R2)
        {
            trace_state = CROSSING;
            crossing_timer = 0;
            Target_Speed = SPEED_Y * 0.3;
        }
        break;

    case CROSSING: // 过十字路口
        if(++crossing_timer > 200) // 约200ms直行后恢复巡线
        {
            trace_state = LINE_FOLLOW;
            g_turn_target = 0.0f;
            Target_Speed = SPEED_Y;
        }
        break;

    case SHARP_TURN: // 急弯处理
        if((valid_L2 && valid_R1) || (++crossing_timer > 500))
        {
            trace_state = LINE_FOLLOW;
            g_turn_target = 0.0f;
            Target_Speed = SPEED_Y;
        }
        break;
    }

    // 转向限幅
    g_turn_target = fmaxf(fminf(g_turn_target, 100.0f), -100.0f);
}
