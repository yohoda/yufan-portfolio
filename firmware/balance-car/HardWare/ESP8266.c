/*ESP8266.C*/

#include "stm32f10x.h"
#include "esp8266.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <delay.h>



ESP_BufTypeDef ESP_RX_BUF;      //Wifi串口接收缓冲区
u8 ESP_RX_STATE = 0;          //初始化已接收数据为0
#define ESP_BUF_SIZE   680     //ESP缓冲区长度

#define ESP_RX_END    0x01     //接收数据结束
#define ESP_RX_ALLOW  0x00     //允许接收数据/正在接收数据

#define ACK_SUCCESS   1
#define ACK_DEFEAT    0


#pragma pack()


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

/*WIFI连接使用*/
char ESP_WIFI_SSID[125]  = "MN001";
char ESP_WIFI_PASSWORD[125] = "MN001789";

//wifi串口初始化
void ESP_Usart_Config(void)
{
	  GPIO_InitTypeDef  GPIO_InitStructure;
	  USART_InitTypeDef USART_InitStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;
	
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);                    //开启GPIOA时钟
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);                     //开启端口复用时钟
	
      RCC_APB1PeriphClockCmd(ESP_RCC, ENABLE);                                 //开启esp串口时钟

	  USART_DeInit(ESP_USART);																								//复位esp串口
	
//		发送端口
	  GPIO_InitStructure.GPIO_Pin = ESP_TX_PIN;																				//PA2
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;																	//复用推挽输出
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(ESP_TX_GPIO, &GPIO_InitStructure);
	
//		接收端口
	  GPIO_InitStructure.GPIO_Pin = ESP_RX_PIN;																		  	//PA3
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;														//复用浮空输入	
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(ESP_RX_GPIO, &GPIO_InitStructure);	
		
	  //配置串口模式  
		USART_DeInit(ESP_USART);
	  USART_InitStructure.USART_BaudRate = 115200;																			//设置波特率为115200bps
	  USART_InitStructure.USART_WordLength = USART_WordLength_8b;												//数据位为8位
	  USART_InitStructure.USART_StopBits = USART_StopBits_1;														//停止位1位
	  USART_InitStructure.USART_Parity = USART_Parity_No;																//无校验位
	  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;										//发送、接收模式打开
	  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件控制流控
	
	  USART_Init(ESP_USART, &USART_InitStructure);																		//传送配置参数
	
	  USART_ClearFlag(ESP_USART, USART_FLAG_CTS);																			//清除发送完成标志位
      USART_Cmd(ESP_USART, ENABLE);																										//使能串口1	  	
	
	  USART_ITConfig(ESP_USART, USART_IT_RXNE, ENABLE);																//开启接收中断 （1字节一次中断）		
		
    //设置NVIC
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	  
	  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;																	//设置串口2中断
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;													//抢占优先级
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;																//子优先级为1
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;																		//使能
	  NVIC_Init(&NVIC_InitStructure);		

    printf("## ESP_usart_config_over ##\r\n");
}

///*串口接收中断函数*/	
//void USART2_IRQHandler(void)
//{
//  	unsigned char rev_byte;
//    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)			//判断度寄存器是否为非空
//	    {
//		       rev_byte = USART_ReceiveData(USART2);     //接收数据
//				
////				   if(ESP_RX_STATE == ESP_RX_ALLOW)            //允许接收数据，即接收完的数据已经处理
//				   {     /*这里是入队函数，可以封装出来*/                                    
//				         if(ESP_RX_BUF.length <= ESP_BUF_SIZE) //如果数据没有溢出
//						 {									 
//							 ESP_RX_BUF.buf[ESP_RX_BUF.rear] = rev_byte;			//将数据存入BUFF缓冲区尾部
//	                         ESP_RX_BUF.length ++;                           //缓冲区长度增加
//						     ESP_RX_BUF.rear = (ESP_RX_BUF.rear + 1) % ESP_BUF_SIZE;  //防止溢出
//						 }		
//                 		  TIM_SetCounter(TIM3,0);                         //计数器清空
//				          TIM_Cmd(TIM3,ENABLE);                           //使能定时器	 
//					 }
//	    }				
//	  USART_ClearITPendingBit(USART2, USART_IT_RXNE);						//清除标志
//}	

//ESP接收缓冲区初始化
void ESP_Rxbuf_Init(void)
{
	  memset(ESP_RX_BUF.buf,0,sizeof(ESP_RX_BUF.buf));

      ESP_RX_BUF.fornt = 0;
	  ESP_RX_BUF.length = 0;
	  ESP_RX_BUF.rear = 0;
	
	  ESP_RX_STATE = 0;          //允许接收数据
}

//读取esp数据接收队列的数据
uint8_t ESP_Read_Quene_Data(ESP_BufTypeDef* Rx_buf)
{
    u8 read_data_temp;
	if(Rx_buf->length == 0)
	{
	    read_data_temp = 0;
	}
	else
	{
	      read_data_temp = Rx_buf->buf[Rx_buf->fornt];
		  Rx_buf->fornt = (Rx_buf->fornt + 1) % ESP_BUF_SIZE;
		  Rx_buf->length --;
	}
	return read_data_temp;
}
u8 ESP_Check_Ack(char *string)         //检查相应回复信号
{
      char *str_temp;            
	  u8 data_temp;
	  u8 rx_buff_temp[150]={0};     //临时存储接收到的数据
	  u8 pt_temp = 0;               //rx_buff_temp的下标
	  u8 length_temp = 0;
		
	  str_temp = string;
	  
	  if(ESP_RX_STATE > 0)    //如果接收数据结束
		{
			//获取收到的回复 至临时数组，便于判断		
		    do
			  {
				  if(length_temp < 150)
				  {
				      data_temp = ESP_Read_Quene_Data(&ESP_RX_BUF);
			          rx_buff_temp[pt_temp] = data_temp;
				      pt_temp ++;

			        length_temp ++;
//					printf("## data%d :%c ##\r\n",length_temp,data_temp);
			    }
				  else 
					    break;  //超出存储上限，没有收到‘\0’则会死循环，跳出当前循环
		  	}
			  while(data_temp != '\0');
			
        ESP_RX_STATE --; //读取完一个接收的字符串，计数-1									    
		}	
		//判断是否符合期望信号
		if(strstr((const char *)rx_buff_temp, (const char *)str_temp ) != NULL)
				
			 return ACK_SUCCESS;   //如果返回地址不为空，返回有效
			
		else
			 return ACK_DEFEAT;    //否则，返回失败
}

void USART_Send_String(USART_TypeDef* USARTx, u8 *Data, ...)
{//使用变长参数，具体实现原理可百度
		 va_list valist;
	   va_start(valist, Data);
	
    while(*Data != '\0')
		{
		    USART_SendData(USARTx, *Data);
			  while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET); //
		}
	}
