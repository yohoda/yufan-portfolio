#ifndef __PID_H
#define __PID_H

// 硬件参数常量
#define WHEEL_DIAMETER      0.065f    // 车轮直径65mm（0.065米）
#define WHEEL_CIRCUMFERENCE (PI * WHEEL_DIAMETER) // 周长≈0.204米
#define ENCODER_PPR         11        // 编码器每转11脉冲
#define GEAR_RATIO          30        // 减速比30:1
#define PI 					3.14

// 加速度计和陀螺仪灵敏度定义
#define ACCEL_SCALE_16G   2048.0f   // ±16g对应灵敏度：2048 LSB/g
#define GYRO_SCALE_2000DPS 16.4f    // ±2000°/s对应灵敏度：16.4 LSB/(°/s)

#define SPEED_FILTER_SIZE 5  // 滤波窗口大小

//#define PWM_MAX 7200  // 根据PWM分辨率调整（如12位为4095）

// 类型定义
typedef struct {
    float Kp, Ki, Kd;
    float MaxIntegral;
    float Integral;
    float LastError;
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

extern PID_Controller s_balance_pid, s_speed_pid, s_turn_pid;
extern volatile float g_current_angle;
extern float g_turn_target;
extern float Get_Current_Angle(void);
void MPU6050_GetPhysicalData(float *ax, float *ay, float *az, float *gx, float *gy, float *gz);
float Encoder_GetSpeed(TIM_TypeDef* TIMx, float dt) ;
void PID_Init(PID_Controller* pid, float kp, float ki, float kd, float max_i);
void Kalman_Init(KalmanFilter* kf) ;
float Kalman_Update(KalmanFilter* kf, float acc_angle, float gyro_rate, float dt);
float Get_TurnTarget(void);
void Motor_Safety_Check(void);
void Balance_Init(void);
void BalanceCar(void);
void Load_PID_Params(void);
void Save_PID_Params(void);
void Process_Command(char *cmd);

#endif
