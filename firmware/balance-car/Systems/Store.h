#ifndef __STORE_H
#define __STORE_H

#define BALANCE_KP_IDX 1    // 直立环Kp占用索引1-2（32位浮点）
#define BALANCE_KI_IDX 3
#define BALANCE_KD_IDX 5
#define SPEED_KP_IDX   7
#define SPEED_KI_IDX   9
#define SPEED_KD_IDX   11
#define TURN_KP_IDX    13
#define TURN_KI_IDX    15
#define TURN_KD_IDX    17

extern uint16_t Store_Data[];
void Store_Init(void);
void Store_Save(void);
void Store_Clear(void);

#endif
