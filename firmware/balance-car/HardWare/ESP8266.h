/*ESP8266.h*/
#ifndef __ESP8266_H_
#define __ESP8266_H_

#include "stm32f10x.h"

#define ESP_BUF_SIZE   680     //ESP缓冲区长度

#define ESP_RX_END    0x01     //接收数据结束
#define ESP_RX_ALLOW  0x00     //允许接收数据/正在接收数据

#define ACK_SUCCESS   1
#define ACK_DEFEAT    0


#pragma pack(1)
typedef struct 
{
    unsigned char buf[ESP_BUF_SIZE];
	unsigned short int length;
    unsigned short int fornt;               //ESP队列头指针
    unsigned short int rear;               //ESP队列尾指针
}ESP_BufTypeDef;            //定义ESP循环队列缓冲区结构体

#pragma pack()

extern ESP_BufTypeDef ESP_RX_BUF;      //Wifi串口接收缓冲区
extern ESP_BufTypeDef ESP_TX_BUF;      //Wifi串口发送缓冲区，实际没使用
extern u8 ESP_RX_STATE;            //接收完成状态

extern int WEATHER;
extern int TEMPERATURE;

//网络串口时钟
#define ESP_RCC       RCC_APB1Periph_USART2

//网络串口发送端口
#define ESP_TX_PIN    GPIO_Pin_2    //PA2
#define ESP_TX_GPIO   GPIOA

//网络串口接收端口
#define ESP_RX_PIN    GPIO_Pin_3   //PA3
#define ESP_RX_GPIO   GPIOA

#define ESP_USART     USART2

//wifi模块使能端口
#define ESP_CS_RCC    RCC_APB2Periph_GPIOA
#define ESP_CS_PIN    GPIO_Pin_4   //PA4
#define ESP_CS_GPIO   GPIOA

#define ESP_CS_H   GPIO_SetBits(ESP_CS_GPIO, ESP_CS_PIN) //ESP CS pin high
#define ESP_CS_L   GPIO_ResetBits(ESP_CS_GPIO, ESP_CS_PIN) //ESP CS pin low



void ESP_Usart_Config(void);
void ESP_Rxbuf_Init(void);     /*初始化缓冲区函数*/

u8 ESP_Connect_TCP(const char *tcp_addr, char *point);
void ESP_Set_Link_Mode(char mode);
u8 ESP_Quit_Trans(void);
u8 ESP_Read_Quene_Data(ESP_BufTypeDef* Rx_buf);
u8 ESP_Check_Ack(char *string);
u8 ESP_Send_Cmd(char *cmd, char *ack, u16 waittime);
void ESP_Send_Data(u8 *data);
void ESP_Init(void);             /*初始化模块函数，主函数调用*/


u8 ESP_Set_Stationmode(void);
u8 ESP_Set_APmode(char *ap_ssid, char *ap_pwd, char chl, char ecn);
u8 ESP_Connect_AP(const char *wifi_ssid, const char *wifi_password);
void ESP_Set_Link_Mode(char mode);
u8 ESP_Connect_TCP(const char *tcp_addr, char *point);
void ESP_Set_Trans(void);

u8 ESP_Start_Trans(void);

u8 ESP_Init_Time(void);       /*初始化时间函数，主函数调用*/
u8 ESP_Fefresh_NET_Massage(const char *tcp_addr, char *point, void (*function)());

u8 ESP_Get_Wifiinfo(char *wifi_info);

void GET_Net_Time(void);      /*获取时间函数，主函数调用*/
void Get_NET_Waether(void);   /*获取天气函数，主函数调用*/
int Get_Day(char *d);
int Get_Week(char *w);
int Get_Moonth(char *m);
int Get_Year(char *y);
int Get_Times(char *h, char *m, char *s);

void Print_ESP_Rx_Buf(void);

#endif

