#include "stm32f10x.h"                  // Device header
#include "MPU6050.h" 
#include "MPU6050Reg.h" 
#include "Motor.h" 
#include "encoder.h" 
#include <math.h>
#include "delay.h"
#include <string.h>
#include <stdio.h>
#include "stm32f10x_flash.h"
#include "Store.h"
#include "MyFlash.h"
#include "serial.h"

// 硬件参数常量
#define WHEEL_DIAMETER      0.065f    // 车轮直径65mm（0.065米）
#define WHEEL_CIRCUMFERENCE (PI * WHEEL_DIAMETER) // 周长≈0.204米
#define ENCODER_PPR         11        // 编码器每转11脉冲
#define GEAR_RATIO          30        // 减速比30:1
#define PI 					3.141592653589793f

// 加速度计和陀螺仪灵敏度定义
#define ACCEL_SCALE_16G   2048.0f  // ±16g对应灵敏度：2048 LSB/g 除1000
#define GYRO_SCALE_2000DPS 16.4f   // ±2000°/s对应灵敏度：16.4 LSB/(°/s) 

#define SPEED_FILTER_SIZE 5  // 滤波窗口大小

#define PWM_MAX 7200  // 根据PWM分辨率调整（如12位为4095）
// 类型定义
typedef struct {
    char name[3];        // 参数标识，如"SP"代表速度环P
    float Kp, Ki, Kd;
    float MaxIntegral;
    float Integral;
    float LastError;
    uint8_t need_save;   // 标记是否需要保存
} PID_Controller;
typedef struct {
    float Q_angle, Q_gyro, R_angle;
    float angle, bias;
    float P[2][2];
} KalmanFilter;


typedef struct {
    float buffer[SPEED_FILTER_SIZE];
    uint8_t index;
} SpeedFilter;

// 主控制循环
void BalanceCar_Update(void);
// 私有全局变量
static KalmanFilter s_kalman;         // 卡尔曼滤波器实例

static SpeedFilter left_speed_filter = {0}, right_speed_filter = {0};   // 速度滤波
extern float g_turn_target;
volatile float g_current_angle = 0.0f;
 PID_Controller s_balance_pid;  // 直立环
 PID_Controller s_speed_pid;    // 速度环
 PID_Controller s_turn_pid;     // 转向环
// 读取原始数据并转换为物理量
void MPU6050_GetPhysicalData(float *ax, float *ay, float *az, float *gx, float *gy, float *gz) {
    int16_t AX, AY, AZ, GX, GY, GZ;
    MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
    
    // 转换为实际物理量
    *ax = AX / ACCEL_SCALE_16G;      // 单位：g
    *ay = AY / ACCEL_SCALE_16G;
    *az = AZ / ACCEL_SCALE_16G;
    *gx = GX / GYRO_SCALE_2000DPS;  // 单位：°/s
    *gy = GY / GYRO_SCALE_2000DPS;
    *gz = GZ / GYRO_SCALE_2000DPS;
}

//速度滤波
float SpeedFilter_Update(SpeedFilter *filter, float new_speed) {
    filter->buffer[filter->index] = new_speed;
    filter->index = (filter->index + 1) % SPEED_FILTER_SIZE;
    
    // 计算平均值
    float sum = 0.0f;
    for (uint8_t i = 0; i < SPEED_FILTER_SIZE; i++) {
        sum += filter->buffer[i];
    }
    return sum / SPEED_FILTER_SIZE;
}

// 获取速度（单位：米/秒）
float Encoder_GetSpeed(TIM_TypeDef* TIMx, float dt) {
    int16_t pulses = (int16_t)TIMx->CNT;
    TIMx->CNT = 0; // 清零计数器
    
    // 计算线速度 = (脉冲数/PPR/减速比) * 车轮周长 / dt
    return (pulses / (float)ENCODER_PPR / GEAR_RATIO) * WHEEL_CIRCUMFERENCE / dt;
}


// PID初始化
void PID_Init(PID_Controller* pid, float kp, float ki, float kd, float max_i) {
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
    pid->MaxIntegral = max_i;
    pid->Integral = 0;
    pid->LastError = 0;
}

// PID计算（微分先行）
float PID_Update(PID_Controller* pid, float target, float current, float dt) {
    float error = target - current;
    pid->Integral += error * dt;
    
    // 积分限幅
    if (pid->Integral > pid->MaxIntegral) pid->Integral = pid->MaxIntegral;
    else if (pid->Integral < -pid->MaxIntegral) pid->Integral = -pid->MaxIntegral;
    
    // 微分项使用当前误差变化率
    float derivative = (error - pid->LastError) / dt;
    pid->LastError = error;
    
    return pid->Kp * error + pid->Ki * pid->Integral + pid->Kd * derivative;
}


// 卡尔曼滤波初始化
void Kalman_Init(KalmanFilter* kf) {
    kf->Q_angle = 0.001f;    // 角度过程噪声（越小越信任模型）
    kf->Q_gyro = 0.003f;     // 陀螺仪零偏过程噪声
    kf->R_angle = 0.5f;      // 加速度计观测噪声（越小越信任观测）
    kf->angle = 0.0f;
    kf->bias = 0.0f;
    kf->P[0][0] = 1.0f;      // 初始角度方差
    kf->P[0][1] = kf->P[1][0] = 0.0f;
    kf->P[1][1] = 1.0f;      // 初始零偏方差
}

// 卡尔曼滤波更新
float Kalman_Update(KalmanFilter* kf, float acc_angle, float gyro_rate, float dt) {
    //----- 预测步骤 -----
    // 1. 更新角度和零偏
    kf->angle += (gyro_rate - kf->bias) * dt;
    
    // 2. 更新协方差矩阵 P = F * P * F^T + Q
    //    F是状态转移矩阵：[1, -dt; 0, 1]
    float F[2][2] = {{1, -dt}, {0, 1}};
    float P_temp[2][2] = {0};
    
    // 计算 F * P
    P_temp[0][0] = F[0][0] * kf->P[0][0] + F[0][1] * kf->P[1][0];
    P_temp[0][1] = F[0][0] * kf->P[0][1] + F[0][1] * kf->P[1][1];
    P_temp[1][0] = F[1][0] * kf->P[0][0] + F[1][1] * kf->P[1][0];
    P_temp[1][1] = F[1][0] * kf->P[0][1] + F[1][1] * kf->P[1][1];
    
    // 计算 (F * P) * F^T
    kf->P[0][0] = P_temp[0][0] * F[0][0] + P_temp[0][1] * F[0][1] + kf->Q_angle;
    kf->P[0][1] = P_temp[0][0] * F[1][0] + P_temp[0][1] * F[1][1];
    kf->P[1][0] = P_temp[1][0] * F[0][0] + P_temp[1][1] * F[0][1];
    kf->P[1][1] = P_temp[1][0] * F[1][0] + P_temp[1][1] * F[1][1] + kf->Q_gyro;

    //----- 更新步骤 -----
    // 1. 计算残差 y = z - H * x
    float y = acc_angle - kf->angle;
    
    // 2. 计算残差协方差 S = H * P * H^T + R
    float S = kf->P[0][0] + kf->R_angle;
    
    // 3. 计算卡尔曼增益 K = P * H^T / S
    float K[2] = {kf->P[0][0]/S, kf->P[1][0]/S};
    
    // 4. 更新状态估计
    kf->angle += K[0] * y;
    kf->bias += K[1] * y;
    
    // 5. 更新协方差矩阵 P = (I - K * H) * P
    float P00_prev = kf->P[0][0];
    float P01_prev = kf->P[0][1];
    
    kf->P[0][0] -= K[0] * P00_prev;
    kf->P[0][1] -= K[0] * P01_prev;
    kf->P[1][0] -= K[1] * P00_prev;
    kf->P[1][1] -= K[1] * P01_prev;

    // 同步到全局变量
    g_current_angle = kf->angle;
    
    return kf->angle;
}

float Get_TurnTarget(void) {
    return g_turn_target;
}

float Get_Current_Angle(void) {
    return g_current_angle;
}

// 在电机驱动前检查倾角
void Motor_Safety_Check(void) {
    if(fabsf(Get_Current_Angle()) > 35.0f) {
        Stop(); // 超过45度急停
		s_balance_pid.Integral = 0.0f;  // 重置积分
        s_speed_pid.Integral = 0.0f;
    }
}

// 参数加载（在Store_Init后调用）
void Load_PID_Params(void) {
    s_balance_pid.Kp = StoreToFloat(&Store_Data[BALANCE_KP_IDX]);
    s_balance_pid.Ki = StoreToFloat(&Store_Data[BALANCE_KI_IDX]);
    s_balance_pid.Kd = StoreToFloat(&Store_Data[BALANCE_KD_IDX]);
	
    s_speed_pid.Kp = StoreToFloat(&Store_Data[SPEED_KP_IDX]);
    s_speed_pid.Ki = StoreToFloat(&Store_Data[SPEED_KI_IDX]);
    s_speed_pid.Kd = StoreToFloat(&Store_Data[SPEED_KD_IDX]);
	
	s_turn_pid.Kp = StoreToFloat(&Store_Data[TURN_KP_IDX]);
    s_turn_pid.Ki = StoreToFloat(&Store_Data[TURN_KI_IDX]);
    s_turn_pid.Kd = StoreToFloat(&Store_Data[TURN_KD_IDX]);
	

}

// 参数保存
void Save_PID_Params(void) {
    FloatToStore(&Store_Data[BALANCE_KP_IDX], s_balance_pid.Kp);
    FloatToStore(&Store_Data[BALANCE_KI_IDX], s_balance_pid.Ki);
    FloatToStore(&Store_Data[BALANCE_KD_IDX], s_balance_pid.Kd);

	FloatToStore(&Store_Data[SPEED_KP_IDX], s_speed_pid.Kp);
    FloatToStore(&Store_Data[SPEED_KI_IDX], s_speed_pid.Ki);
    FloatToStore(&Store_Data[SPEED_KD_IDX], s_speed_pid.Kd);
	
	FloatToStore(&Store_Data[TURN_KP_IDX], s_turn_pid.Kp);
    FloatToStore(&Store_Data[TURN_KI_IDX], s_turn_pid.Ki);
    FloatToStore(&Store_Data[TURN_KD_IDX], s_turn_pid.Kd);
    
	Store_Save(); // 调用现有保存函数
}

void Balance_Init(void)
{
    /* 1. 初始化卡尔曼滤波器 */
    Kalman_Init(&s_kalman);

    /* 2. 初始化PID控制器结构体 */
    PID_Init(&s_balance_pid, 0.0f, 0.0f, 0.0f, 0.0f);  // 临时占位值，后续会被覆盖
    PID_Init(&s_speed_pid, 0.0f, 0.0f, 0.0f, 0.0f);
    PID_Init(&s_turn_pid, 0.0f, 0.0f, 0.0f, 0.0f);

    /* 3. 加载Flash中的PID参数 */
    Load_PID_Params();  // 从Store_Data加载参数到PID控制器

    /* 4. 首次运行检测：如果参数全为0，设置默认值并保存 */
    if (s_balance_pid.Kp == 0.0f && 
        s_balance_pid.Ki == 0.0f && 
        s_balance_pid.Kd == 0.0f &&
        s_speed_pid.Kp == 0.0f &&
        s_speed_pid.Ki == 0.0f &&
        s_speed_pid.Kd == 0.0f &&
        s_turn_pid.Kp == 0.0f &&
        s_turn_pid.Ki == 0.0f &&
        s_turn_pid.Kd == 0.0f)
    {
        // 设置默认参数
        s_balance_pid.Kp = -500.0f;     // 直立环P
        s_balance_pid.Ki = 0.0f;      // 直立环I
        s_balance_pid.Kd =7.0f;      // 直立环D

        s_speed_pid.Kp = -0.44f;        // 速度环P
        s_speed_pid.Ki = -0.0022f;       // 速度环I
        s_speed_pid.Kd = 0.0f;        // 速度环D

        s_turn_pid.Kp = 20.0f;         // 转向环P
        s_turn_pid.Ki = 0.0f;         // 转向环I
        s_turn_pid.Kd = 0.6f;         // 转向环D

        // 保存默认参数到Flash
        Save_PID_Params();
    }

}

void BalanceCar(void)
{
	
	
	 float dt = 0.005f;
          
            // 1. 读取传感器数据
            float ax, ay, az, gx, gy, gz;
            MPU6050_GetPhysicalData(&ax, &ay, &az, &gx, &gy, &gz);
            
            // 2. 计算卡尔曼滤波角度
            float acc_angle = atan2(ax, az) * 180.0f / PI;
            float angle = Kalman_Update(&s_kalman, acc_angle, gy, dt);
            
            // 3. 读取编码器速度
            float speed_left = Encoder_GetSpeed(TIM2, dt);
            float speed_right = -Encoder_GetSpeed(TIM4, dt);
			speed_left = SpeedFilter_Update(&left_speed_filter, speed_left);
			speed_right = SpeedFilter_Update(&right_speed_filter, speed_right);
	
            float speed_avg = (speed_left + speed_right) * 0.5f;
            // 新增转向约束逻辑
			if (fabs(Get_TurnTarget()) < 0.1f) {  // 无转向指令
				s_turn_pid.Kd = 0.6f;  // 开启约束
			} else {
				s_turn_pid.Kd = 0.0f;  // 关闭约束
			}
            // 4. PID计算
            float balance_out = PID_Update(&s_balance_pid, 13.0f, angle, dt);
            float speed_out = PID_Update(&s_speed_pid, 0.0f, speed_avg, dt);
            float turn_out = PID_Update(&s_turn_pid, Get_TurnTarget(), 0.0f, dt);
            
            // 5. 综合输出
            //float pwm_left = balance_out + speed_out + turn_out;
           // float pwm_right = balance_out + speed_out - turn_out;
			float pwm_left = balance_out ;
            float pwm_right = balance_out ;


			pwm_left = fmaxf(fminf(pwm_left, PWM_MAX), -PWM_MAX);
			pwm_right = fmaxf(fminf(pwm_right, PWM_MAX), -PWM_MAX);
			
			Motor_SetSpeed(pwm_left, pwm_right);
			Motor_Safety_Check();
			//Delay_ms(5);
			
}

//pid 设置指令
void Process_Command(char *cmd) {
    char param[20];
    float value;
    float kp,ki,kd;
	// 参数设置命令
   if (Serial_RxFlag==1){ 
	if(sscanf(cmd, "balance_kp %f", &value) == 1) {
        s_balance_pid.Kp = value;
        Serial_Printf("Set Balance Kp: %.2f\r\n", value);
    } 
	else if(sscanf(cmd, "balance_ki %f", &value) == 1) {
        s_balance_pid.Ki = value;
        Serial_Printf("Set Balance Ki: %.2f\r\n", value);
    }
	else if(sscanf(cmd, "balance_kd %f", &value) == 1) {
        s_balance_pid.Kd = value;
        Serial_Printf("Set Balance Kd: %.2f\r\n", value);
    }
	
	else if(sscanf(cmd, "speed_kp %f", &value) == 1) {
        s_speed_pid.Kp = value;
        Serial_Printf("Set Speed Ki: %.4f\r\n", value);
    } 
	else if(sscanf(cmd, "speed_ki %f", &value) == 1) {
        s_speed_pid.Ki = value;
        Serial_Printf("Set Speed Ki: %.4f\r\n", value);
    }
    else if(sscanf(cmd, "speed_kd %f", &value) == 1) {
        s_speed_pid.Kd = value;
        Serial_Printf("Set Speed Kd: %.4f\r\n", value);
    }
	
	else if(sscanf(cmd, "turn_kp %f", &value) == 1) {
        s_turn_pid.Kp = value;
        Serial_Printf("Set Tpeed Ki: %f\r\n", value);
    } 
	else if(sscanf(cmd, "turn_ki %f", &value) == 1) {
        s_turn_pid.Ki = value;
        Serial_Printf("Set Turn Ki: %f\r\n", value);
    }
    else if(sscanf(cmd, "turn_kd %f", &value) == 1) {
        s_turn_pid.Kd = value;
        Serial_Printf("Set Turn Kd: %f\r\n", value);
    }
	// 保存命令：save
    else if(strcmp(cmd, "save") == 0) {
        Save_PID_Params();
        Serial_Printf("Parameters saved to Flash!\r\n");
    }
    else {
        Serial_Printf("Invalid command. Usage:\r\n"
               "balance_kp [value]  - 设置直立环P\r\n"
               "speed_ki [value]    - 设置速度环I\r\n"
               "save                - 保存到Flash\r\n");
    }
	// 打印指令
	if(strcmp(cmd, "print") == 0) {
    Serial_Printf("=== Current PID ===\r\n");
    Serial_Printf("Balance: Kp=%.2f Ki=%.3f Kd=%.2f\r\n", 
           s_balance_pid.Kp, s_balance_pid.Ki, s_balance_pid.Kd);
   Serial_Printf("Speed:   Kp=%.2f Ki=%.4f Kd=%.2f\r\n", 
           s_speed_pid.Kp, s_speed_pid.Ki, s_speed_pid.Kd);
	}
	
	if(sscanf(cmd, "set_balance %f %f %f", &kp, &ki, &kd) == 3) {
    s_balance_pid.Kp = kp;
    s_balance_pid.Ki = ki;
    s_balance_pid.Kd = kd;
	}
	
	if(sscanf(cmd, "set_speed %f %f %f", &kp, &ki, &kd) == 3) {
    s_speed_pid.Kp = kp;
    s_speed_pid.Ki = ki;
    s_speed_pid.Kd = kd;
	}	
	
	if(sscanf(cmd, "set_turn %f %f %f", &kp, &ki, &kd) == 3) {
    s_turn_pid.Kp = kp;
    s_turn_pid.Ki = ki;
    s_turn_pid.Kd = kd;
	}
	
	if(strcmp(param, "balance_kp") == 0) 
	{
		if(value < 0 || value > 200) {
		Serial_Printf("ERROR: Kp must be 0-200\r\r\n");
        return;
		}
		if(strcmp(param, "balance_kp") == 0) {
		if(value < 0 || value > 200) {
		Serial_Printf("ERROR: Kp must be 0-200\r\r\n");
        return;
		}
		s_balance_pid.Kp = value;
		}
	}
	
	else if(strcmp(cmd, "get_angle") == 0) {
    char msg[32];
    Serial_Printf(msg, "{\"angle\":%.2f}\r\r\n", Get_Current_Angle());
    Serial_SendString(msg);
}
	
	//复位指令
	else if (strcmp(cmd, "reset_params") == 0) {
        s_balance_pid.Kp = -450.0f;     // 直立环P
        s_balance_pid.Ki = 0.0f;      // 直立环I
        s_balance_pid.Kd =-1.92;      // 直立环D

        s_speed_pid.Kp = -0.44;        // 速度环P
        s_speed_pid.Ki = -0.0022;       // 速度环I
        s_speed_pid.Kd = 0.0f;        // 速度环D

        s_turn_pid.Kp = 20;         // 转向环P
        s_turn_pid.Ki = 0.0f;         // 转向环I
        s_turn_pid.Kd = 0.6f;         // 转向环D

		Save_PID_Params();
		
		Serial_Printf("OK: Parameters reset to default\r\r\n");
}
	
		Serial_RxFlag=0;
   }
}

