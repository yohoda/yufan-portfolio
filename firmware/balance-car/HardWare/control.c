#include "stm32f10x.h"                  // Device header
#include "sys.h"

float Med_Angle=-10.5;		//机械中值
float Target_Speed=0;	//目标速度
float Turn_Speed=0;		//转向速度
int g_iAbnormalSpinFlag = 0;//异常转动标志
unsigned char g_cMotorDisable = 0;//值等于0时电机正常转动，否则停止转动
u8 Fore,Back,Left,Right;
float 
	Vertical_Kp=360,//-550x0.6
	Vertical_Kd=1.44;//-2.4x0.6

float 
	Velocity_Kp=0.8,//KPbKI
	Velocity_Ki=0.004;//0.01;
float 
	Turn_Kd=0,//вKPbKD
	Turn_Kp=0;
int length;
#define SPEED_Y 40   //()
#define SPEED_Z 100//() 


int Vertical_out,Velocity_out,Turn_out;//b&&в 

int Vertical(float Med,float Angle,float gyro_Y);//′k
int Velocity(int Target,int encoder_left,int encoder_right);
int Turn(int gyro_Z,int RC); 
/*============== 硬件参数定义 ==============*/
#define ENCODER_MAX       40    // 编码器单次最大脉冲值
#define PWM_MAX          7200   // PWM最大值
#define SPEED_RATIO      (PWM_MAX/ENCODER_MAX) // 速度换算系数 180

/*============== PID控制结构体 ==============*/
typedef struct {
    float Kp;
    float Ki;
    float Kd;
    float integral;
    float prev_error;
} PID_Controller;

/*============== 全局控制变量 ==============*/
PID_Controller vertical_pid = {-300.0f, 0.0f, -1.1f, 0.0f, 0.0f};   // 直立环
PID_Controller speed_pid    = {-0.3f,   -0.0015f, 0.0f, 0.0f, 0.0f};  // 速度环
PID_Controller steer_pid    = {0.6f,   0.0f,   0.2f, 0.0f, 0.0f};  // 转向环

/*============== 直立环PD控制 ==============*/
int VerticalControl(float target_angle, float actual_angle, float gyro) {
    // 角度偏差计算
    float error = target_angle - actual_angle;
    
    // PD控制计算
    float output = vertical_pid.Kp * error 
                 + vertical_pid.Kd * gyro;
    
    // 输出限幅
    output = fmaxf(fminf(output, PWM_MAX), -PWM_MAX);
    return (int)output;
}

/*============== 速度环PI控制 ==============*/
int SpeedControl(int target_speed, int encoder_left, int encoder_right) {
    // 速度计算（取双轮平均值）
    int actual_speed = (encoder_left + encoder_right) / 2;
    int error = target_speed - actual_speed;

    // 积分项处理
    speed_pid.integral += error * speed_pid.Ki;
    speed_pid.integral = fmaxf(fminf(speed_pid.integral, ENCODER_MAX), -ENCODER_MAX);

    // PI控制计算
    float output = speed_pid.Kp * error + speed_pid.integral;

    // 速度环输出映射到PWM范围
    output *= SPEED_RATIO;
    output = fmaxf(fminf(output, PWM_MAX*0.3), -PWM_MAX*0.3); // 限制速度环最大影响
    
    return (int)output;
}

/*============== 转向环PD控制 ==============*/
int SteeringControl(int target_turn, int gyro_z) {
    float error = target_turn - gyro_z;
    
    // 微分项计算
    float derivative = error - steer_pid.prev_error;
    steer_pid.prev_error = error;

    // PD控制计算
    float output = steer_pid.Kp * error 
                 + steer_pid.Kd * derivative;
    
    // 转向输出映射到PWM范围
    output *= 50.0f;  // 根据实测转向灵敏度调整
    output = fmaxf(fminf(output, PWM_MAX*0.2), -PWM_MAX*0.2);
    
    return (int)output;
}

/*============== 电机输出合成 ==============*/
void MotorOutputSynthesis(void) {
    // 各环控制量计算
    int vertical_out = VerticalControl(Med_Angle, Pitch, gyroy);
    int speed_out = SpeedControl(Target_Speed, Encoder_Left, Encoder_Right);
    int steer_out = SteeringControl(Turn_Speed, gyroz);

    // 电机输出合成
    int motor_left  = vertical_out + speed_out - steer_out;
    int motor_right = vertical_out + speed_out + steer_out;

    // 最终输出限幅
    motor_left  = (int)fmaxf(fminf(motor_left, PWM_MAX), -PWM_MAX);
    motor_right = (int)fmaxf(fminf(motor_right, PWM_MAX), -PWM_MAX);

    // 加载到电机
    Motor_SetSpeed(motor_left, motor_right);
}

/*============== 中断服务函数示例 ==============*/
void EXTI9_5_IRQHandler(void) {
    if(EXTI_GetITStatus(EXTI_Line5)) {
        // 1.传感器数据采集
        Encoder_Left  = -Encoder_Get(TIM2);  // 编码器反向补偿
        Encoder_Right = Encoder_Get(TIM4);
        mpu_dmp_get_data(&Pitch, &Roll, &Yaw);

        // 2.控制量计算
       // Target_Speed = SpeedMapping(Fore, Back, Left, Right); // 根据输入信号映射目标速度
        MotorOutputSynthesis();

        // 3.异常状态检测
        if(GFP_abs(Pitch) > 30.0f) 
		Stop(&Med_Angle,&Pitch);;  // 跌倒保护
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
}
