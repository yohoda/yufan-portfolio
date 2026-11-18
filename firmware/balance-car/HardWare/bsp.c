#include "stm32f10x.h"                  // Device header
#include "MPU6050.h" 
#include "MPU6050Reg.h" 
#include "Motor.h" 
#include "encoder.h" 
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "Ultrasonic.h"
#include "serial.h"
#include "pwm.h"
#include "trace.h"
#include "PID.h"
#include "key.h"
#include "Store.h"
#include "menu.h"
#include "MYRTC.h"
#include "exti.h"


void bsp_Init(void)
	
{	delay_init();
	NVIC_Config();
	uart1_init(115200);	
	
	OLED_Init();
	OLED_Clear();
	
	Key_Init();
	
	MPU_Init();
	mpu_dmp_init();
	MPU6050_EXTI_Init();
	
	MyRTC_Init();
	
	Ultrasonic_Init();
	Encoder_Init();
	Motor_Init();
	
	
	Trace_Init();
	
	
	
	//Serial_Init();
	//Store_Init();
	
	//Balance_Init();
    Servo_SetAngle(90);  // 初始位置
	int menu2;

}
