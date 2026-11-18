#ifndef __ULTRASSONIC_H
#define __ULTRASSONIC_H

#define TRIG_GPIO_PORT    GPIOA
#define TRIG_GPIO_PIN     GPIO_Pin_15
#define ECHO_GPIO_PORT    GPIOA
#define ECHO_GPIO_PIN     GPIO_Pin_12
#define SERVO_GPIO_PORT   GPIOB
#define SERVO_GPIO_PIN    GPIO_Pin_0   // TIM3_CH1

extern uint32_t g_echo_start;
extern uint32_t g_distance_mm;

void TIM3_Init(void);
void Servo_SetAngle(float angle);
void EXTI_Config(void);
void EXTI15_10_IRQHandler(void);
void Ultrasonic_Init(void);
void Ultrasonic_Trigger(void);
uint32_t Get_Distance(void);
void Ultrasonic(void);

#endif
