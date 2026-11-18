#ifndef __TIMER_H
#define __TIMER_H

extern unsigned int cnt;
void TIM2_Init(void);
void TIM2_IRQHandler(void);
void TIM4_Init(u16 arr,u16 psc);
void TIM4_IRQHandler(void);

#endif
