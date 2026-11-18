#include "stm32f10x.h"                  // Device header

#include "sys.h"



uint8_t KeyNum;					//用于存储键码值
uint8_t D=2;					//用于存储目前按键是上还是下,默认下一项
 int menu2;
	uint8_t ID;								//定义用于存放ID号的变量
float AX, AY, AZ, GX, GY, GZ;			//定义用于存放各个数据的变量
/***************************************************************************************/
/*把一级菜单的flag放到全局变量,静态存储,不会丢失数据,所以从二级菜单回来以后
仍能回到那一行,而不是每次都回到第一行*/
uint8_t flag=1;
/***************************************************************************************/

/*-----------------------------------------一级菜单----------------------------------------------*/
/**
  * @brief 一级菜单函数,用于显示一级菜单八行选项
  * @param  无
  * @retval 返回当前行是第几行
  */
int menu1(void)
{
	
	/*初始显示*/
	OLED_Update();
	while(1)
	{
		KeyNum = Key_GetNumber();
		if(KeyNum == 1)//上一项
		{
			flag--;
			if(flag == 0){flag = 8;}
			D = 1;//上一项标志
			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
		}
		if(KeyNum == 2)//下一项
		{
			flag++;
			if(flag == 9){flag = 1;}
			D = 2;//下一项标志
			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
		}
		if(KeyNum == 3)//确认
		{
			OLED_Clear();
			OLED_Update();
			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
			return flag;
		}
		switch(flag)
		{
			case 1:
			{
				//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
				OLED_ShowString(0,0,"循迹模式             ",OLED_8X16);
				OLED_ShowString(0,16,"云台控制            ",OLED_8X16);
				OLED_ShowString(0,32,"摇杆数据            ",OLED_8X16);
				OLED_ShowString(0,48,"MPU6050             ",OLED_8X16);
				if(D==2){OLED_Animation(0,0,0,0,0,0,64,16);}//下
				if(D==1){OLED_Animation(0,16,64,16,0,0,64,16);}//上
				break;
			}
			case 2:
			{
				//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
				OLED_ShowString(0,0,"循迹模式             ",OLED_8X16);
				OLED_ShowString(0,16,"云台控制            ",OLED_8X16);
				OLED_ShowString(0,32,"摇杆数据            ",OLED_8X16);
				OLED_ShowString(0,48,"MPU6050             ",OLED_8X16);
				if(D==2){OLED_Animation(0,0,64,16,0,16,64,16);}
				if(D==1){OLED_Animation(0,32,64,16,0,16,64,16);}	
				break;
				
			}
			case 3:
			{
				//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
				OLED_ShowString(0,0,"循迹模式             ",OLED_8X16);
				OLED_ShowString(0,16,"云台控制            ",OLED_8X16);
				OLED_ShowString(0,32,"摇杆数据            ",OLED_8X16);
				OLED_ShowString(0,48,"MPU6050             ",OLED_8X16);
				if(D==2){OLED_Animation(0,16,64,16,0,32,64,16);}
				if(D==1){OLED_Animation(0,48,56,16,0,32,64,16);}			
				break;
			}
			case 4:
			{
				//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
				OLED_ShowString(0,0,"循迹模式             ",OLED_8X16);
				OLED_ShowString(0,16,"云台控制            ",OLED_8X16);
				OLED_ShowString(0,32,"摇杆数据            ",OLED_8X16);
				OLED_ShowString(0,48,"MPU6050             ",OLED_8X16);
				if(D==2){OLED_Animation(0,32,64,16,0,48,56,16);}
				if(D==1){OLED_Animation(0,64,56,16,0,48,56,16);}
				break;
			}
			case 5:
			{
				//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
				OLED_ShowString(0,0,"PID                    ",OLED_8X16);
				OLED_ShowString(0,16,"时钟                  ",OLED_8X16);
				OLED_ShowString(0,32,"串口                  ",OLED_8X16);
				OLED_ShowString(0,48,"设置                  ",OLED_8X16);
				if(D==2){OLED_Animation(0,0,0,0,0,0,24,16);}
				if(D==1){OLED_Animation(0,16,32,16,0,0,24,16);}
				break;
			}
			case 6:
			{
				//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
				OLED_ShowString(0,0,"PID                    ",OLED_8X16);
				OLED_ShowString(0,16,"时钟                  ",OLED_8X16);
				OLED_ShowString(0,32,"串口                  ",OLED_8X16);
				OLED_ShowString(0,48,"设置                  ",OLED_8X16);
				if(D==2){OLED_Animation(0,0,24,16,0,16,32,16);}
				if(D==1){OLED_Animation(0,32,32,16,0,16,32,16);}
				break;
			}
			case 7:
			{
				//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
				OLED_ShowString(0,0,"PID                    ",OLED_8X16);
				OLED_ShowString(0,16,"时钟                  ",OLED_8X16);
				OLED_ShowString(0,32,"串口                  ",OLED_8X16);
				OLED_ShowString(0,48,"设置                  ",OLED_8X16);
				if(D==2){OLED_Animation(0,16,32,16,0,32,32,16);}
				if(D==1){OLED_Animation(0,48,32,16,0,32,32,16);}
				break;
			}
			case 8:
			{
				//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
				OLED_ShowString(0,0,"PID                    ",OLED_8X16);
				OLED_ShowString(0,16,"时钟                  ",OLED_8X16);
				OLED_ShowString(0,32,"串口                  ",OLED_8X16);
				OLED_ShowString(0,48,"设置                  ",OLED_8X16);
				if(D==2){OLED_Animation(0,32,32,16,0,48,32,16);}
				if(D==1){OLED_Animation(0,64,0,0,0,48,32,16);}
				break;
			}
		}
	}
}


/*--------------------------------------------------------------------------------------*/
/*-----------------------------------二级循迹菜单-----------------------------------------*/


/**
  * @brief 循迹二级菜单函数
  * @param  无
  * @retval 
  */
int menu2_Trace(void)
{
	int menu3_T;
	uint8_t tflag = 1;//默认在第一行
	OLED_ShowString(0,0,"<-               ",OLED_8X16);
	OLED_ShowString(0,16,"摇杆控制        ",OLED_8X16);
	OLED_ShowString(0,32,"MPU6050控制     ",OLED_8X16);
	OLED_ShowString(0,48,"按键控制        ",OLED_8X16);
	OLED_Update();
	while(1)
	{
		KeyNum = Key_GetNumber();
		if(KeyNum == 1)//上一项
		{
			tflag--;
			if(tflag == 0){tflag = 4;}//共四行
			D = 1;//上一项标志
			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
		}
		if(KeyNum == 2)//下一项
		{
			tflag++;
			if(tflag == 5){tflag = 1;}
			D = 2;//下一项标志
			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
		}
		if(KeyNum == 3)//确认
		{
			OLED_Clear();
			OLED_Update();
			menu3_T = tflag;
			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
		}
		if(menu3_T == 1){return 0;}//返回0,退到第一级菜单
		if(menu3_T == 2){return 0;}//返回0,退到第一级菜单(因为还没写三级菜单函数)
		if(menu3_T == 3){return 0;}//返回0,退到第一级菜单(因为还没写三级菜单函数)
		if(menu3_T == 4){return 0;}//返回0,退到第一级菜单(因为还没写三级菜单函数)
		
		switch(tflag)
		{
			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
			case 1:
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_ShowString(0,16,"摇杆控制        ",OLED_8X16);
				OLED_ShowString(0,32,"MPU6050控制     ",OLED_8X16);
				OLED_ShowString(0,48,"按键控制        ",OLED_8X16);
				if(D==2){OLED_Animation(0,0,0,0,0,0,16,16);}//下
				if(D==1){OLED_Animation(0,16,64,16,0,0,16,16);}//上
				break;
			}
			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
			case 2:
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_ShowString(0,16,"摇杆控制        ",OLED_8X16);
				OLED_ShowString(0,32,"MPU6050控制     ",OLED_8X16);
				OLED_ShowString(0,48,"按键控制        ",OLED_8X16);
				if(D==2){OLED_Animation(0,0,16,16,0,16,64,16);}//下
				if(D==1){OLED_Animation(0,32,88,16,0,16,64,16);}//上
				break;
			}
			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
			case 3:
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_ShowString(0,16,"摇杆控制        ",OLED_8X16);
				OLED_ShowString(0,32,"MPU6050控制     ",OLED_8X16);
				OLED_ShowString(0,48,"按键控制        ",OLED_8X16);
				if(D==2){OLED_Animation(0,16,64,16,0,32,88,16);}//下
				if(D==1){OLED_Animation(0,48,64,16,0,32,88,16);}//上
				break;
			}
			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
			case 4:
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_ShowString(0,16,"摇杆控制        ",OLED_8X16);
				OLED_ShowString(0,32,"MPU6050控制     ",OLED_8X16);
				OLED_ShowString(0,48,"按键控制        ",OLED_8X16);
				if(D==2){OLED_Animation(0,32,64,16,0,48,64,16);}//下
				if(D==1){OLED_Animation(0,64,64,16,0,48,64,16);}//上
				break;
			}
		}
	}
}

/*--------------------------------------------------------------------------------------*/
/*-----------------------------------二级云台菜单-----------------------------------------*/
/**
  * @brief 三级MPU6050控制菜单函数
  * @param  无
  * @retval 
  */
int menu3_YTMPU(void)
{
	OLED_Clear();//先清屏,防止覆盖显示
	OLED_ShowString(0,24,"Press Key3 to back",OLED_6X8);
	OLED_Update();
	Servo_SetAngle(90);//初始位置
	while(1)
	{
		KeyNum = Key_GetNumber();
		if(KeyNum == 3)
		{
			return 0;
		}
		
	}
}


/**
  * @brief 云台二级菜单函数
  * @param  无
  * @retval 
  */
int menu2_YunTai(void)
{
	int menu3_YT;
	uint8_t ytflag = 1;//默认在第一行
	OLED_ShowString(0,0,"<-               ",OLED_8X16);
	OLED_ShowString(0,16,"摇杆控制        ",OLED_8X16);
	OLED_ShowString(0,32,"MPU6050控制     ",OLED_8X16);
	OLED_ShowString(0,48,"按键控制        ",OLED_8X16);
	OLED_Update();
	while(1)
	{
		KeyNum = Key_GetNumber();
		if(KeyNum == 1)//上一项
		{
			ytflag--;
			if(ytflag == 0){ytflag = 4;}//共四行
			D = 1;//上一项标志
			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
		}
		if(KeyNum == 2)//下一项
		{
			ytflag++;
			if(ytflag == 5){ytflag = 1;}
			D = 2;//下一项标志
			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
		}
		if(KeyNum == 3)//确认
		{
			OLED_Clear();
			OLED_Update();
			menu3_YT = ytflag;
			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
		}
		if(menu3_YT == 1){return 0;}//返回0,退到第一级菜单
		if(menu3_YT == 2){return 0;}//返回0,退到第一级菜单(因为还没写三级菜单函数)
		if(menu3_YT == 3){menu3_YT = menu3_YTMPU();}//返回0,退到第一级菜单(因为还没写三级菜单函数)
		if(menu3_YT == 4){return 0;}//返回0,退到第一级菜单(因为还没写三级菜单函数)
		
		switch(ytflag)
		{
			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
			case 1:
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_ShowString(0,16,"摇杆控制        ",OLED_8X16);
				OLED_ShowString(0,32,"MPU6050控制     ",OLED_8X16);
				OLED_ShowString(0,48,"按键控制        ",OLED_8X16);
				if(D==2){OLED_Animation(0,0,0,0,0,0,16,16);}//下
				if(D==1){OLED_Animation(0,16,64,16,0,0,16,16);}//上
				break;
			}
			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
			case 2:
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_ShowString(0,16,"摇杆控制        ",OLED_8X16);
				OLED_ShowString(0,32,"MPU6050控制     ",OLED_8X16);
				OLED_ShowString(0,48,"按键控制        ",OLED_8X16);
				if(D==2){OLED_Animation(0,0,16,16,0,16,64,16);}//下
				if(D==1){OLED_Animation(0,32,88,16,0,16,64,16);}//上
				break;
			}
			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
			case 3:
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_ShowString(0,16,"摇杆控制        ",OLED_8X16);
				OLED_ShowString(0,32,"MPU6050控制     ",OLED_8X16);
				OLED_ShowString(0,48,"按键控制        ",OLED_8X16);
				if(D==2){OLED_Animation(0,16,64,16,0,32,88,16);}//下
				if(D==1){OLED_Animation(0,48,64,16,0,32,88,16);}//上
				break;
			}
			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
			case 4:
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_ShowString(0,16,"摇杆控制        ",OLED_8X16);
				OLED_ShowString(0,32,"MPU6050控制     ",OLED_8X16);
				OLED_ShowString(0,48,"按键控制        ",OLED_8X16);
				if(D==2){OLED_Animation(0,32,64,16,0,48,64,16);}//下
				if(D==1){OLED_Animation(0,64,64,16,0,48,64,16);}//上
				break;
			}
		}
	}
}

/*--------------------------------------------------------------------------------------*/
/*-----------------------------二级摇杆数据菜单-----------------------------------------*/
/**
  * @brief 摇杆数据二级菜单函数
  * @param  无
  * @retval 
  */
int menu2_YaoGan(void)
{
	int menu3_YYG;
	uint8_t ygflag = 1;//默认在第一行
	OLED_ShowString(0,0,"<-               ",OLED_8X16);
	OLED_ShowString(0,16,"VRX:            ",OLED_8X16);
	OLED_ShowString(0,32,"VRY:            ",OLED_8X16);
	OLED_ShowString(0,48,"SW:             ",OLED_8X16);
	OLED_Update();
	while(1)
	{
		KeyNum = Key_GetNumber();
		if(KeyNum == 1)//上一项
		{
			ygflag--;
			if(ygflag == 0){ygflag = 4;}//共四行
			D = 1;//上一项标志
			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
		}
		if(KeyNum == 2)//下一项
		{
			ygflag++;
			if(ygflag == 5){ygflag = 1;}
			D = 2;//下一项标志
			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
		}
		if(KeyNum == 3)//确认
		{
			OLED_Clear();
			OLED_Update();
			menu3_YYG = ygflag;
			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
		}
		if(menu3_YYG == 1){return 0;}//返回0,退到第一级菜单
		if(menu3_YYG == 2){return 0;}//返回0,退到第一级菜单(因为还没写三级菜单函数)
		if(menu3_YYG == 3){return 0;}//返回0,退到第一级菜单(因为还没写三级菜单函数)
		if(menu3_YYG == 4){return 0;}//返回0,退到第一级菜单(因为还没写三级菜单函数)
		
		switch(ygflag)
		{
			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
			case 1:
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_ShowString(0,16,"VRX:            ",OLED_8X16);
				OLED_ShowString(0,32,"VRY:            ",OLED_8X16);
				OLED_ShowString(0,48,"SW:             ",OLED_8X16);
				if(D==2){OLED_Animation(0,0,0,0,0,0,16,16);}//下
				if(D==1){OLED_Animation(0,16,32,16,0,0,16,16);}//上
				break;
			}
			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
			case 2:
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_ShowString(0,16,"VRX:            ",OLED_8X16);
				OLED_ShowString(0,32,"VRY:            ",OLED_8X16);
				OLED_ShowString(0,48,"SW:             ",OLED_8X16);
				if(D==2){OLED_Animation(0,0,16,16,0,16,32,16);}//下
				if(D==1){OLED_Animation(0,32,32,16,0,16,32,16);}//上
				break;
			}
			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
			case 3:
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_ShowString(0,16,"VRX:            ",OLED_8X16);
				OLED_ShowString(0,32,"VRY:            ",OLED_8X16);
				OLED_ShowString(0,48,"SW:             ",OLED_8X16);
				if(D==2){OLED_Animation(0,16,32,16,0,32,32,16);}//下
				if(D==1){OLED_Animation(0,48,24,16,0,32,32,16);}//上
				break;
			}
			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
			case 4:
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_ShowString(0,16,"VRX:            ",OLED_8X16);
				OLED_ShowString(0,32,"VRY:            ",OLED_8X16);
				OLED_ShowString(0,48,"SW:             ",OLED_8X16);
				if(D==2){OLED_Animation(0,32,32,16,0,48,24,16);}//下
				if(D==1){OLED_Animation(0,64,24,16,0,48,24,16);}//上
				break;
			}
		}
	}
}
///*--------------------------------------------------------------------------------------*/
///*-----------------------------二级MPU6050菜单------------------------------------------*/
///**
//  * @brief MPU6050二级菜单函数
//  * @param  无
//  * @retval 
//  */
//int menu2_MPU6050(void)
//{	

//	int menu3_MMPU;
//	uint8_t mpuflag = 1;//默认在第一行
//	OLED_ShowString(0,0,"<-               ",OLED_8X16);
//	OLED_ShowString(64, 0, "ID:",OLED_8X16);		//显示静态字符串
////	ID = MPU6050_GetID();				//获取MPU6050的ID号
//	OLED_ShowHexNum(90, 0, ID, 2,OLED_8X16);		//OLED显示ID号
////	OLED_Printf(0,16,OLED_8X16,"俯仰角:%.2f     ",Pitch);
////	OLED_Printf(0,32,OLED_8X16,"横滚角:%.2f     ",Roll);
////	OLED_Printf(0,48,OLED_8X16,"偏航角:%.2f     ",Yaw);
////	OLED_Update();
//		
//	
//	while(1)
//	{	MPU6050_GetPhysicalData(&AX, &AY, &AZ, &GX, &GY, &GZ);		//获取MPU6050的数据
////		OLED_ShowSignedNum(0, 16, AX, 5,OLED_8X16);					//OLED显示数据
////		OLED_ShowSignedNum(0, 32, AY, 5,OLED_8X16);
////		OLED_ShowSignedNum(0, 48, AZ, 5,OLED_8X16);
////		OLED_ShowSignedNum(64,16, GX, 5,OLED_8X16);
////		OLED_ShowSignedNum(64,32, GY, 5,OLED_8X16);
////		OLED_ShowSignedNum(64, 48, GZ, 5,OLED_8X16);
//		OLED_Printf(0,16,OLED_8X16,"%+.3f    %+.3f",AX,GX);
//		OLED_Printf(0,32,OLED_8X16,"%+.3f    %+.3f",AY,GY);
//		OLED_Printf(0,48,OLED_8X16,"%+.3f    %+.3f",AZ,GZ);
//		OLED_Update();
//		KeyNum = Key_GetNumber();
//		if(KeyNum == 1)//上一项
//		{
//			mpuflag--;
//			if(mpuflag == 0){mpuflag = 4;}//共四行
//			D = 1;//上一项标志
//			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
//		}
//		if(KeyNum == 2)//下一项
//		{
//			mpuflag++;
//			if(mpuflag == 5){mpuflag = 1;}
//			D = 2;//下一项标志
//			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
//		}
//		if(KeyNum == 3)//确认
//		{
//			OLED_Clear();
//			OLED_Update();
//			menu3_MMPU = mpuflag;
//			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
//			
//		}
//		if(menu3_MMPU == 1){return 0;}//返回0,退到第一级菜单
//		if(menu3_MMPU == 2){return 0;}//返回0,退到第一级菜单(因为还没写三级菜单函数)
//		if(menu3_MMPU == 3){return 0;}//返回0,退到第一级菜单(因为还没写三级菜单函数)
//		if(menu3_MMPU == 4){return 0;}//返回0,退到第一级菜单(因为还没写三级菜单函数)
//		
//		switch(mpuflag)
//		{
//			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
//			case 1:
//			{
//				OLED_ShowString(0,0,"<-               ",OLED_8X16);
//				OLED_ShowString(64, 0, "ID:",OLED_8X16);		//显示静态字符串
//				OLED_ShowHexNum(90, 0, ID, 2,OLED_8X16);		//OLED显示ID号
//				OLED_Printf(0,16,OLED_8X16,"%+.3f    %+.3f",AX,GX);
//				OLED_Printf(0,32,OLED_8X16,"%+.3f    %+.3f",AY,GY);
//				OLED_Printf(0,48,OLED_8X16,"%+.3f    %+.3f",AZ,GZ);
//				if(D==2){OLED_Animation(0,0,0,0,0,0,128,16);}//下
//				if(D==1){OLED_Animation(0,16,128,16,0,0,16,16);}//上
//				break;
//			}
//			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
//			case 2:
//			{
//				OLED_ShowString(0,0,"<-               ",OLED_8X16);
//				OLED_ShowString(64, 0, "ID:",OLED_8X16);		//显示静态字符串
//				OLED_ShowHexNum(90, 0, ID, 2,OLED_8X16);		//OLED显示ID号
//				//OLED_ShowSignedNum(0, 16, AX, 5,OLED_8X16);					//OLED显示数据
//				OLED_Printf(0,16,OLED_8X16,"%+.3f    %+.3f",AX,GX);
//				OLED_Printf(0,32,OLED_8X16,"%+.3f    %+.3f",AY,GY);
//				OLED_Printf(0,48,OLED_8X16,"%+.3f    %+.3f",AZ,GZ);
//				if(D==2){OLED_Animation(0,0,16,16,0,16,128,16);}//下
//				if(D==1){OLED_Animation(0,32,128,16,0,16,128,16);}//上
//				break;
//			}
//			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
//			case 3:
//			{
//				OLED_ShowString(0,0,"<-               ",OLED_8X16);
//				OLED_ShowString(64, 0, "ID:",OLED_8X16);		//显示静态字符串
//				//OLED_ShowSignedNum(0, 16, AX, 5,OLED_8X16);					//OLED显示数据
//				OLED_Printf(0,16,OLED_8X16,"%+.3f    %+.3f",AX,GX);
//				OLED_Printf(0,32,OLED_8X16,"%+.3f    %+.3f",AY,GY);
//				OLED_Printf(0,48,OLED_8X16,"%+.3f    %+.3f",AZ,GZ);
//				if(D==2){OLED_Animation(0,16,128,16,0,32,128,16);}//下
//				if(D==1){OLED_Animation(0,48,128,16,0,32,128,16);}//上
//				break;
//			}
//			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
//			case 4:
//			{
//				OLED_ShowString(0,0,"<-               ",OLED_8X16);
//				OLED_ShowString(64, 0, "ID:",OLED_8X16);		//显示静态字符串
//				//OLED_ShowSignedNum(0, 16, AX, 5,OLED_8X16);					//OLED显示数据
//				OLED_Printf(0,16,OLED_8X16,"%+.3f    %+.3f",AX,GX);
//				OLED_Printf(0,16,OLED_8X16,"%+.3f    %+.3f",AX,GX);
//				OLED_Printf(0,32,OLED_8X16,"%+.3f    %+.3f",AY,GY);
//				OLED_Printf(0,48,OLED_8X16,"%+.3f    %+.3f",AZ,GZ);
//				if(D==2){OLED_Animation(0,32,128,16,0,48,128,16);}//下
//				if(D==1){OLED_Animation(0,64,128,16,0,48,128,16);}//上
//				break;
//			}
//		}
//	}
//}
/*--------------------------------------------------------------------------------------*/
/*-----------------------------二级MPU6050菜单------------------------------------------*/
/**
  * @brief MPU6050二级菜单函数
  * @param  无
  * @retval 
  */
int menu2_MPU6050(void)
{
	int menu3_MMPU;
	uint8_t mpuflag = 1;//默认在第一行
	OLED_ShowString(0,0,"<-               ",OLED_8X16);
	OLED_Printf(0,16,OLED_8X16,"俯仰角:%.2f     ",Pitch);
	OLED_Printf(0,32,OLED_8X16,"横滚角:%.2f     ",Roll);
	OLED_Printf(0,48,OLED_8X16,"偏航角:%.2f     ",Yaw);
	OLED_Update();
	while(1)
	{
		OLED_ShowString(0,0,"<-               ",OLED_8X16);
		OLED_Printf(0,16,OLED_8X16,"俯仰角:%.2f     ",Pitch);
		OLED_Printf(0,32,OLED_8X16,"横滚角:%.2f     ",Roll);
		OLED_Printf(0,48,OLED_8X16,"偏航角:%.2f     ",Yaw);
		KeyNum = Key_GetNumber();
		if(KeyNum == 1)//上一项
		{
			mpuflag--;
			if(mpuflag == 0){mpuflag = 4;}//共四行
			D = 1;//上一项标志
			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
		}
		if(KeyNum == 2)//下一项
		{
			mpuflag++;
			if(mpuflag == 5){mpuflag = 1;}
			D = 2;//下一项标志
			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
		}
		if(KeyNum == 3)//确认
		{
			OLED_Clear();
			OLED_Update();
			menu3_MMPU = mpuflag;
			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
		}
		if(menu3_MMPU == 1){return 0;}//返回0,退到第一级菜单
		if(menu3_MMPU == 2){return 0;}//返回0,退到第一级菜单(因为还没写三级菜单函数)
		if(menu3_MMPU == 3){return 0;}//返回0,退到第一级菜单(因为还没写三级菜单函数)
		if(menu3_MMPU == 4){return 0;}//返回0,退到第一级菜单(因为还没写三级菜单函数)
		
		switch(mpuflag)
		{
			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
			case 1:
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_Printf(0,16,OLED_8X16,"俯仰角:%.2f     ",Pitch);
				OLED_Printf(0,32,OLED_8X16,"横滚角:%.2f     ",Roll);
				OLED_Printf(0,48,OLED_8X16,"偏航角:%.2f     ",Yaw);
				if(D==2){OLED_Animation(0,0,0,0,0,0,16,16);}//下
				if(D==1){OLED_Animation(0,16,56,16,0,0,16,16);}//上
				break;
			}
			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
			case 2:
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_Printf(0,16,OLED_8X16,"俯仰角:%.2f     ",Pitch);
				OLED_Printf(0,32,OLED_8X16,"横滚角:%.2f     ",Roll);
				OLED_Printf(0,48,OLED_8X16,"偏航角:%.2f     ",Yaw);
				if(D==2){OLED_Animation(0,0,16,16,0,16,56,16);}//下
				if(D==1){OLED_Animation(0,32,56,16,0,16,56,16);}//上
				break;
			}
			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
			case 3:
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_Printf(0,16,OLED_8X16,"俯仰角:%.2f     ",Pitch);
				OLED_Printf(0,32,OLED_8X16,"横滚角:%.2f     ",Roll);
				OLED_Printf(0,48,OLED_8X16,"偏航角:%.2f     ",Yaw);
				if(D==2){OLED_Animation(0,16,56,16,0,32,56,16);}//下
				if(D==1){OLED_Animation(0,48,56,16,0,32,56,16);}//上
				break;
			}
			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
			case 4:
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_Printf(0,16,OLED_8X16,"俯仰角:%.2f     ",Pitch);
				OLED_Printf(0,32,OLED_8X16,"横滚角:%.2f     ",Roll);
				OLED_Printf(0,48,OLED_8X16,"偏航角:%.2f     ",Yaw);
				if(D==2){OLED_Animation(0,32,56,16,0,48,56,16);}//下
				if(D==1){OLED_Animation(0,64,56,16,0,48,56,16);}//上
				break;
			}
		}
	}
}



///*--------------------------------------------------------------------------------------*/
///*---------------------------------三级PID菜单------------------------------------------*/
///**
//  * @brief PID三级菜单函数
//  * @param  无
//  * @retval 
//  */
//int menu2_PID(void);

//int menu3_PID_turn(void)
//{	
//	uint8_t edit_mode = 0; // 0:浏览模式，1:编辑模式
//	int menu4_MPID;
//	uint8_t pidflag = 1;//默认在第一行
//	OLED_ShowString(0,0,"<-               ",OLED_8X16);
//	OLED_Printf(0,16,OLED_8X16,"Kp:    %.2f     ",s_turn_pid.Kp);
//	OLED_Printf(0,32,OLED_8X16,"Ki:    %.2f     ",s_turn_pid.Ki);
//	OLED_Printf(0,48,OLED_8X16,"Kd:    %.2f     ",s_turn_pid.Kd);
//	OLED_Update();
//	while(1)
//	{
//		KeyNum = Key_GetNumber();
//		if(edit_mode == 0)
//		{
//			if(KeyNum == 1)//上一项
//		{
//			pidflag--;
//			if(pidflag == 0){pidflag = 4;}//共四行
//			D = 1;//上一项标志
//			OLED_AnimUpdate();
//		}
//		if(KeyNum == 2)//下一项
//		{
//			pidflag++;
//			if(pidflag == 5){pidflag = 1;}
//			D = 2;//下一项标志
//			OLED_AnimUpdate();
//		}
//		if (KeyNum == 3) // 确认键
//			{	
//				if (pidflag == 1) // 第一行（返回）
//				{	
//					OLED_Clear();
//					OLED_Update();
//					return 0;
//				}
//				else // 其他行进入编辑模式
//				{	
//					edit_mode = 1;
//					menu4_MPID = pidflag;
//				}
//				OLED_AnimUpdate();
//			}
//		}
//		
//		
//		else
//		{	
//			if(menu4_MPID == 1)
//			{	
//				edit_mode = 0;
//				OLED_Clear();
//				OLED_Update();
//				return 0;
//			}
//			if(menu4_MPID == 2)
//			{	
//				if(KeyNum == 1)
//				{
//					s_turn_pid.Kp--;
//					OLED_AnimUpdate();
//				}
//				if(KeyNum == 2)
//				{
//					s_turn_pid.Kp++;
//					OLED_AnimUpdate();
//				}
//				if(KeyNum == 3)
//				{
//					OLED_Clear();
//					Save_PID_Params();
//					OLED_Update();
//					OLED_AnimUpdate();
//					edit_mode = 0;
//				}
//				if(KeyNum == 4)
//				{	
//					edit_mode = 0;
//					OLED_Clear();
//					return 0;
//				}
//			}
//		if(menu4_MPID == 3)
//			{	
//				{edit_mode = 1;}
//				if(KeyNum == 1)
//				{
//					s_turn_pid.Ki--;
//					OLED_AnimUpdate();
//				}
//				if(KeyNum == 2)
//				{
//					s_turn_pid.Ki++;
//					OLED_AnimUpdate();
//				}
//				if(KeyNum == 3)
//				{
//					OLED_Clear();
//					Save_PID_Params();
//					OLED_Update();
//					OLED_AnimUpdate();
//					edit_mode = 0;
//				}
//				if(KeyNum == 4)
//				{	
//					edit_mode = 0;
//					OLED_Clear();
//					return 0;
//				}
//			}
//		if(menu4_MPID == 4)
//			{	
//				{edit_mode = 1;}
//				if(KeyNum == 1)
//				{
//					s_turn_pid.Kd--;
//					OLED_AnimUpdate();
//				}
//				if(KeyNum == 2)
//				{
//					s_turn_pid.Kd++;
//					OLED_AnimUpdate();
//				}
//				if(KeyNum == 3)
//				{
//					OLED_Clear();
//					Save_PID_Params();
//					OLED_Update();
//					OLED_AnimUpdate();
//					edit_mode = 0;
//				}
//				if(KeyNum == 4)
//				{	
//					edit_mode = 0;
//					OLED_Clear();
//					return 0;
//				}
//			}
//		}			
//		switch(pidflag)
//		{
//			case 1:
//			{
//				OLED_ShowString(0,0,"<-               ",OLED_8X16);
//				OLED_Printf(0,16,OLED_8X16,"Kp:    %.2f     ",s_turn_pid.Kp);
//				OLED_Printf(0,32,OLED_8X16,"Ki:    %.2f     ",s_turn_pid.Ki);
//				OLED_Printf(0,48,OLED_8X16,"Kd:    %.2f     ",s_turn_pid.Kd);
//				if(D==2){OLED_Animation(0,0,0,0,0,0,16,16);}
//				if(D==1){OLED_Animation(0,16,128,16,0,0,16,16);}
//				break;
//			}
//			case 2:
//			{
//				OLED_ShowString(0,0,"<-               ",OLED_8X16);
//				OLED_Printf(0,16,OLED_8X16,"Kp:    %.2f     ",s_turn_pid.Kp);
//				OLED_Printf(0,32,OLED_8X16,"Ki:    %.2f     ",s_turn_pid.Ki);
//				OLED_Printf(0,48,OLED_8X16,"Kd:    %.2f     ",s_turn_pid.Kd);
//				if(D==2){OLED_Animation(0,0,128,16,0,16,128,16);}
//				if(D==1){OLED_Animation(0,32,128,16,0,16,128,16);}
//				break;
//			}
//			case 3:
//			{
//				OLED_ShowString(0,0,"<-               ",OLED_8X16);
//				OLED_Printf(0,16,OLED_8X16,"Kp:    %.2f     ",s_turn_pid.Kp);
//				OLED_Printf(0,32,OLED_8X16,"Ki:    %.2f     ",s_turn_pid.Ki);
//				OLED_Printf(0,48,OLED_8X16,"Kd:    %.2f     ",s_turn_pid.Kd);
//				if(D==2){OLED_Animation(0,16,128,16,0,32,128,16);}
//				if(D==1){OLED_Animation(0,48,128,16,0,32,128,16);}
//				break;
//			}
//			case 4:
//			{
//				OLED_ShowString(0,0,"<-               ",OLED_8X16);
//				OLED_Printf(0,16,OLED_8X16,"Kp:    %.2f     ",s_turn_pid.Kp);
//				OLED_Printf(0,32,OLED_8X16,"Ki:    %.2f     ",s_turn_pid.Ki);
//				OLED_Printf(0,48,OLED_8X16,"Kd:    %.2f     ",s_turn_pid.Kd);
//				if(D==2){OLED_Animation(0,32,128,16,0,48,128,16);}
//				if(D==1){OLED_Animation(0,64,128,16,0,48,128,16);}
//				break;
//			}
//		}
//	}
//}

//int menu3_PID_balance(void)
//{	
//	uint8_t edit_mode = 0; // 0:浏览模式，1:编辑模式
//	int menu4_MPID;
//	uint8_t pidflag = 1;//默认在第一行
//	OLED_ShowString(0,0,"<-               ",OLED_8X16);
//	OLED_Printf(0,16,OLED_8X16,"Kp:    %.2f     ",s_balance_pid.Kp);
//	OLED_Printf(0,32,OLED_8X16,"Ki:    %.2f     ",s_balance_pid.Ki);
//	OLED_Printf(0,48,OLED_8X16,"Kd:    %.2f     ",s_balance_pid.Kd);
//	OLED_Update();
//	while(1)
//	{
//		KeyNum = Key_GetNumber();
//		if(edit_mode == 0)
//		{
//			if(KeyNum == 1)//上一项
//		{
//			pidflag--;
//			if(pidflag == 0){pidflag = 4;}
//			D = 1;
//			OLED_AnimUpdate();
//		}
//		if(KeyNum == 2)//下一项
//		{
//			pidflag++;
//			if(pidflag == 5){pidflag = 1;}
//			D = 2;
//			OLED_AnimUpdate();
//		}
//		if (KeyNum == 3)
//			{	
//				if (pidflag == 1)
//				{	
//					OLED_Clear();
//					OLED_Update();
//					return 0;
//				}
//				else 
//				{	
//					edit_mode = 1;
//					menu4_MPID = pidflag;
//				}
//				OLED_AnimUpdate();
//			}
//		}
//		
//		
//		else
//		{	
//			if(menu4_MPID == 1)
//			{	
//				edit_mode = 0;
//				OLED_Clear();
//				OLED_Update();
//				return 0;
//			}
//			if(menu4_MPID == 2)
//			{	
//				if(KeyNum == 1)
//				{
//					s_balance_pid.Kp--;
//					OLED_AnimUpdate();
//				}
//				if(KeyNum == 2)
//				{
//					s_balance_pid.Kp++;
//					OLED_AnimUpdate();
//				}
//				if(KeyNum == 3)
//				{
//					OLED_Clear();
//					Save_PID_Params();
//					OLED_Update();
//					OLED_AnimUpdate();
//					edit_mode = 0;
//				}
//				if(KeyNum == 4)
//				{	
//					edit_mode = 0;
//					OLED_Clear();
//					return 0;
//				}
//			}
//		if(menu4_MPID == 3)
//			{	
//				{edit_mode = 1;}
//				if(KeyNum == 1)
//				{
//					s_balance_pid.Ki--;
//					OLED_AnimUpdate();
//				}
//				if(KeyNum == 2)
//				{
//					s_balance_pid.Ki++;
//					OLED_AnimUpdate();
//				}
//				if(KeyNum == 3)
//				{
//					OLED_Clear();
//					Save_PID_Params();
//					OLED_Update();
//					OLED_AnimUpdate();
//					edit_mode = 0;
//				}
//				if(KeyNum == 4)
//				{	
//					edit_mode = 0;
//					OLED_Clear();
//					return 0;
//				}
//			}
//		if(menu4_MPID == 4)
//			{	
//				{edit_mode = 1;}
//				if(KeyNum == 1)
//				{
//					s_balance_pid.Kd--;
//					OLED_AnimUpdate();
//				}
//				if(KeyNum == 2)
//				{
//					s_balance_pid.Kd++;
//					OLED_AnimUpdate();
//				}
//				if(KeyNum == 3)
//				{
//					OLED_Clear();
//					Save_PID_Params();
//					OLED_Update();
//					OLED_AnimUpdate();
//					edit_mode = 0;
//				}
//				if(KeyNum == 4)
//				{	
//					edit_mode = 0;
//					OLED_Clear();
//					return 0;
//				}
//			}
//		}			
//		switch(pidflag)
//		{
//			case 1:
//			{
//				OLED_ShowString(0,0,"<-               ",OLED_8X16);
//				OLED_Printf(0,16,OLED_8X16,"Kp:    %.2f     ",s_balance_pid.Kp);
//				OLED_Printf(0,32,OLED_8X16,"Ki:    %.2f     ",s_balance_pid.Ki);
//				OLED_Printf(0,48,OLED_8X16,"Kd:    %.2f     ",s_balance_pid.Kd);
//				if(D==2){OLED_Animation(0,0,0,0,0,0,16,16);}
//				if(D==1){OLED_Animation(0,16,128,16,0,0,16,16);}
//				break;
//			}
//			case 2:
//			{
//				OLED_ShowString(0,0,"<-               ",OLED_8X16);
//				OLED_Printf(0,16,OLED_8X16,"Kp:    %.2f     ",s_balance_pid.Kp);
//				OLED_Printf(0,32,OLED_8X16,"Ki:    %.2f     ",s_balance_pid.Ki);
//				OLED_Printf(0,48,OLED_8X16,"Kd:    %.2f     ",s_balance_pid.Kd);
//				if(D==2){OLED_Animation(0,0,128,16,0,16,128,16);}
//				if(D==1){OLED_Animation(0,32,128,16,0,16,128,16);}
//				break;
//			}
//			case 3:
//			{
//				OLED_ShowString(0,0,"<-               ",OLED_8X16);
//				OLED_Printf(0,16,OLED_8X16,"Kp:    %.2f     ",s_balance_pid.Kp);
//				OLED_Printf(0,32,OLED_8X16,"Ki:    %.2f     ",s_balance_pid.Ki);
//				OLED_Printf(0,48,OLED_8X16,"Kd:    %.2f     ",s_balance_pid.Kd);
//				if(D==2){OLED_Animation(0,16,128,16,0,32,128,16);}
//				if(D==1){OLED_Animation(0,48,128,16,0,32,128,16);}
//				break;
//			}
//			case 4:
//			{
//				OLED_ShowString(0,0,"<-               ",OLED_8X16);
//				OLED_Printf(0,16,OLED_8X16,"Kp:    %.2f     ",s_balance_pid.Kp);
//				OLED_Printf(0,32,OLED_8X16,"Ki:    %.2f     ",s_balance_pid.Ki);
//				OLED_Printf(0,48,OLED_8X16,"Kd:    %.2f     ",s_balance_pid.Kd);
//				if(D==2){OLED_Animation(0,32,128,16,0,48,128,16);}
//				if(D==1){OLED_Animation(0,64,128,16,0,48,128,16);}
//				break;
//			}
//		}
//	}
//}
//int menu3_PID_speed(void)
//{	
//	uint8_t edit_mode = 0; // 0:浏览模式，1:编辑模式
//	int menu4_MPID;
//	uint8_t pidflag = 1;//默认在第一行
//	OLED_ShowString(0,0,"<-               ",OLED_8X16);
//	OLED_Printf(0,16,OLED_8X16,"Kp:    %.2f     ",s_speed_pid.Kp);
//	OLED_Printf(0,32,OLED_8X16,"Ki:    %.2f     ",s_speed_pid.Ki);
//	OLED_Printf(0,48,OLED_8X16,"Kd:    %.2f     ",s_speed_pid.Kd);
//	OLED_Update();
//	while(1)
//	{
//		KeyNum = Key_GetNumber();
//		if(edit_mode == 0)
//		{
//			if(KeyNum == 1)//上一项
//		{
//			pidflag--;
//			if(pidflag == 0){pidflag = 4;}//共四行
//			D = 1;//上一项标志
//			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
//		}
//		if(KeyNum == 2)//下一项
//		{
//			pidflag++;
//			if(pidflag == 5){pidflag = 1;}
//			D = 2;//下一项标志
//			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
//		}
//		if (KeyNum == 3) // 确认键
//			{	
//				if (pidflag == 1) // 第一行（返回）
//				{	
//					OLED_Clear();
//					OLED_Update();
//					return 0; // 直接返回上级菜单
//				}
//				else // 其他行进入编辑模式
//				{	
//					edit_mode = 1;
//					menu4_MPID = pidflag;
//				}
//				OLED_AnimUpdate();
//			}
//		}
//		
//		
//		else
//		{	
//			if(menu4_MPID == 1)
//			{	
//				edit_mode = 0;
//				OLED_Clear();
//				OLED_Update();
//				
//				return 0;
//			}//退到第二级菜单
//			if(menu4_MPID == 2)
//			{	
//				
//				if(KeyNum == 1)//上一项
//				{
//					s_speed_pid.Kp--;
//					OLED_AnimUpdate();//按下按钮,刷新一次动画效果
//				}
//				if(KeyNum == 2)//下一项
//				{
//					s_speed_pid.Kp++;
//					OLED_AnimUpdate();//按下按钮,刷新一次动画效果
//				}
//				if(KeyNum == 3)//确认
//				{
//					OLED_Clear();
//					Save_PID_Params();
//					OLED_Update();
//					OLED_AnimUpdate();//按下按钮,刷新一次动画效果
//					edit_mode = 0;
//				}
//				if(KeyNum == 4)//返回
//				{	
//					edit_mode = 0;
//					OLED_Clear();
//					return 0;
//					
//				}
//			}
//		if(menu4_MPID == 3)
//			{	
//				{edit_mode = 1;}
//				if(KeyNum == 1)//上一项
//				{
//					s_speed_pid.Ki--;
//					OLED_AnimUpdate();//按下按钮,刷新一次动画效果
//				}
//				if(KeyNum == 2)//下一项
//				{
//					s_speed_pid.Ki++;
//					OLED_AnimUpdate();//按下按钮,刷新一次动画效果
//				}
//				if(KeyNum == 3)//确认
//				{
//					OLED_Clear();
//					Save_PID_Params();
//					OLED_Update();
//					OLED_AnimUpdate();//按下按钮,刷新一次动画效果
//					edit_mode = 0;
//				}
//				if(KeyNum == 4)//返回
//				{	
//					edit_mode = 0;
//					OLED_Clear();
//					return 0;
//					
//				}
//			}
//		if(menu4_MPID == 4)
//			{	
//				{edit_mode = 1;}
//				if(KeyNum == 1)//上一项
//				{
//					s_speed_pid.Kd--;
//					OLED_AnimUpdate();//按下按钮,刷新一次动画效果
//				}
//				if(KeyNum == 2)//下一项
//				{
//					s_speed_pid.Kd++;
//					OLED_AnimUpdate();//按下按钮,刷新一次动画效果
//				}
//				if(KeyNum == 3)//确认
//				{
//					OLED_Clear();
//					Save_PID_Params();//数据存入Flash
//					OLED_Update();
//					OLED_AnimUpdate();//按下按钮,刷新一次动画效果
//					edit_mode = 0;
//				}
//				if(KeyNum == 4)//返回
//				{	
//					edit_mode = 0;
//					OLED_Clear();
//					return 0;
//				}
//			}
//		}			
//		switch(pidflag)
//		{
//			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
//			case 1:
//			{
//				OLED_ShowString(0,0,"<-               ",OLED_8X16);
//				OLED_Printf(0,16,OLED_8X16,"Kp:    %.2f     ",s_speed_pid.Kp);
//				OLED_Printf(0,32,OLED_8X16,"Ki:    %.2f     ",s_speed_pid.Ki);
//				OLED_Printf(0,48,OLED_8X16,"Kd:    %.2f     ",s_speed_pid.Kd);
//				if(D==2){OLED_Animation(0,0,0,0,0,0,16,16);}//下
//				if(D==1){OLED_Animation(0,16,128,16,0,0,16,16);}//上
//				break;
//			}
//			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
//			case 2:
//			{
//				OLED_ShowString(0,0,"<-               ",OLED_8X16);
//				OLED_Printf(0,16,OLED_8X16,"Kp:    %.2f     ",s_speed_pid.Kp);
//				OLED_Printf(0,32,OLED_8X16,"Ki:    %.2f     ",s_speed_pid.Ki);
//				OLED_Printf(0,48,OLED_8X16,"Kd:    %.2f     ",s_speed_pid.Kd);
//				if(D==2){OLED_Animation(0,0,128,16,0,16,128,16);}//下
//				if(D==1){OLED_Animation(0,32,128,16,0,16,128,16);}//上
//				break;
//			}
//			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
//			case 3:
//			{
//				OLED_ShowString(0,0,"<-               ",OLED_8X16);
//				OLED_Printf(0,16,OLED_8X16,"Kp:    %.2f     ",s_speed_pid.Kp);
//				OLED_Printf(0,32,OLED_8X16,"Ki:    %.2f     ",s_speed_pid.Ki);
//				OLED_Printf(0,48,OLED_8X16,"Kd:    %.2f     ",s_speed_pid.Kd);
//				if(D==2){OLED_Animation(0,16,128,16,0,32,128,16);}//下
//				if(D==1){OLED_Animation(0,48,128,16,0,32,128,16);}//上
//				break;
//			}
//			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
//			case 4:
//			{
//				OLED_ShowString(0,0,"<-               ",OLED_8X16);
//				OLED_Printf(0,16,OLED_8X16,"Kp:    %.2f     ",s_speed_pid.Kp);
//				OLED_Printf(0,32,OLED_8X16,"Ki:    %.2f     ",s_speed_pid.Ki);
//				OLED_Printf(0,48,OLED_8X16,"Kd:    %.2f     ",s_speed_pid.Kd);
//				if(D==2){OLED_Animation(0,32,128,16,0,48,128,16);}//下
//				if(D==1){OLED_Animation(0,64,128,16,0,48,128,16);}//上
//				break;
//			}
//		}
//	}
//}

/*--------------------------------------------------------------------------------------*/
/*---------------------------------二级PID菜单------------------------------------------*/
/**
  * @brief PID二级菜单函数
  * @param  无
  * @retval 
  */
//int menu2_PID(void)
//{
//	int menu3_PPID;
//	uint8_t pidflag = 1;//默认在第一行
//	OLED_ShowString(0,0,"<-               ",OLED_8X16);
//	OLED_ShowString(0,16,"直立环            ",OLED_8X16);
//	OLED_ShowString(0,32,"速度环            ",OLED_8X16);
//	OLED_ShowString(0,48,"转向环             ",OLED_8X16);
//	OLED_Update();
//	while(1)
//	{
//		KeyNum = Key_GetNumber();
//		if(KeyNum == 1)//上一项
//		{
//			pidflag--;
//			if(pidflag == 0){pidflag = 4;}//共四行
//			D = 1;//上一项标志
//			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
//		}
//		if(KeyNum == 2)//下一项
//		{
//			pidflag++;
//			if(pidflag == 5){pidflag = 1;}
//			D = 2;//下一项标志
//			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
//		}
//		if(KeyNum == 3)//确认
//		{
//			OLED_Clear();
//			OLED_Update();
//			menu3_PPID = pidflag;
//			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
//		}
//		if(menu3_PPID == 1){return 0;}//返回0,退到第一级菜单
//		if(menu3_PPID == 2){return menu3_PID_balance();}//进入三级菜单
//		if(menu3_PPID == 3){return menu3_PID_speed();}//进入三级菜单
//		if(menu3_PPID == 4){return menu3_PID_turn();}//进入三级菜单
//		
//		switch(pidflag)
//		{
//			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
//			case 1:
//			{
//				OLED_ShowString(0,0,"<-               ",OLED_8X16);
//				OLED_ShowString(0,16,"直立环            ",OLED_8X16);
//				OLED_ShowString(0,32,"速度环            ",OLED_8X16);
//				OLED_ShowString(0,48,"转向环             ",OLED_8X16);
//				if(D==2){OLED_Animation(0,0,0,0,0,0,16,16);}//下
//				if(D==1){OLED_Animation(0,16,48,16,0,0,16,16);}//上
//				break;
//			}
//			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
//			case 2:
//			{
//				OLED_ShowString(0,0,"<-               ",OLED_8X16);
//				OLED_ShowString(0,16,"直立环            ",OLED_8X16);
//				OLED_ShowString(0,32,"速度环            ",OLED_8X16);
//				OLED_ShowString(0,48,"转向环             ",OLED_8X16);
//				if(D==2){OLED_Animation(0,0,16,16,0,16,48,16);}//下
//				if(D==1){OLED_Animation(0,32,48,16,0,16,48,16);}//上
//				break;
//			}
//			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
//			case 3:
//			{
//				OLED_ShowString(0,0,"<-               ",OLED_8X16);
//				OLED_ShowString(0,16,"直立环            ",OLED_8X16);
//				OLED_ShowString(0,32,"速度环            ",OLED_8X16);
//				OLED_ShowString(0,48,"转向环             ",OLED_8X16);
//				if(D==2){OLED_Animation(0,16,48,16,0,32,48,16);}//下
//				if(D==1){OLED_Animation(0,48,48,16,0,32,48,16);}//上
//				break;
//			}
//			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
//			case 4:
//			{
//				OLED_ShowString(0,0,"<-               ",OLED_8X16);
//				OLED_ShowString(0,16,"直立环            ",OLED_8X16);
//				OLED_ShowString(0,32,"速度环            ",OLED_8X16);
//				OLED_ShowString(0,48,"转向环             ",OLED_8X16);
//				if(D==2){OLED_Animation(0,32,48,16,0,48,48,16);}//下
//				if(D==1){OLED_Animation(0,64,48,16,0,48,48,16);}//上
//				break;
//			}
//		}
//	}
//}

/*--------------------------------------------------------------------------------------*/
/*------------------------------- 日期有效性验证函数 -----------------------------------*/
/**
  * @brief 检查日期是否合法
  * @param  year  年份
  * @param  month 月份
  * @param  day   日期
  * @retval 0-非法 1-合法
  */
uint8_t ValidateDate(uint16_t year, uint8_t month, uint8_t day)
{
    uint8_t daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    
    /* 闰年判断 */
    if((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        daysInMonth[1] = 29;
    }

    if(month < 1 || month > 12) return 0;
    if(day < 1 || day > daysInMonth[month-1]) return 0;
    
    return 1;
}

/*--------------------------------------------------------------------------------------*/
/*------------------------------- 时钟设置三级菜单-----------------------------*/
/**
  * @brief 时钟设置三级菜单函数
  * @param  无
  * @retval 0-正常设置完成 1-中途返回
  */
int menu3_SetClock(void)
{
    uint8_t current_param = 0;
    const char *param_names[] = {"Year", "Month", "Day", "Hour", "Minute", "Second"};
    uint8_t KeyNum = 0;
    uint8_t exit_flag = 0;

    /* 进入动画：从底部向上展开 */
    OLED_Animation(0, 63, 128, 1, 0, 0, 128, 64);
    
    while(current_param < 6 && !exit_flag)
    {
        OLED_Clear();
        /* 显示当前设置项（带反色效果） */
        OLED_Printf(0, 0, OLED_8X16, "Set %s:", param_names[current_param]);
        OLED_ReverseArea(0, 0, 128, 16); // 当前项反色显示
        OLED_Printf(0, 16, OLED_8X16, "%d", MyRTC_Time[current_param]);
        OLED_ShowString(0, 32, "1:Down 2:Up", OLED_8X16);
        OLED_ShowString(0, 48, "3:Next 4:Back", OLED_8X16);
        OLED_Update();

        KeyNum = Key_GetNumber();
        switch(KeyNum)
        {
            case 1: // 数值减
                switch(current_param)
                {
                    case 0: if(MyRTC_Time[0] > 2000) MyRTC_Time[0]--; break;
                    case 1: MyRTC_Time[1] = (MyRTC_Time[1] > 1) ? MyRTC_Time[1]-1 : 12; break;
                    case 2: MyRTC_Time[2] = (MyRTC_Time[2] > 1) ? MyRTC_Time[2]-1 : 31; 
                            if(!ValidateDate(MyRTC_Time[0], MyRTC_Time[1], MyRTC_Time[2])) 
                                MyRTC_Time[2] = 31;
                            break;
                    case 3: MyRTC_Time[3] = (MyRTC_Time[3] > 0) ? MyRTC_Time[3]-1 : 23; break;
                    case 4: MyRTC_Time[4] = (MyRTC_Time[4] > 0) ? MyRTC_Time[4]-1 : 59; break;
                    case 5: MyRTC_Time[5] = (MyRTC_Time[5] > 0) ? MyRTC_Time[5]-1 : 59; break;
                }
                OLED_AnimUpdate(); // 数值变化时刷新动画
                break;
                
            case 2: // 数值加
                switch(current_param)
                {
                    case 0: if(MyRTC_Time[0] < 2099) MyRTC_Time[0]++; break;
                    case 1: MyRTC_Time[1] = (MyRTC_Time[1]%12)+1; break;
                    case 2: MyRTC_Time[2] = (MyRTC_Time[2]%31)+1; 
                            if(!ValidateDate(MyRTC_Time[0], MyRTC_Time[1], MyRTC_Time[2])) 
                                MyRTC_Time[2] = 1;
                            break;
                    case 3: MyRTC_Time[3] = (MyRTC_Time[3]+1)%24; break;
                    case 4: MyRTC_Time[4] = (MyRTC_Time[4]+1)%60; break;
                    case 5: MyRTC_Time[5] = (MyRTC_Time[5]+1)%60; break;
                }
                OLED_AnimUpdate();
                break;
                
            case 3: // 下一项
                /* 日期特殊验证 */
                if(current_param == 1){ // 月份变化后验证日期
                    if(!ValidateDate(MyRTC_Time[0], MyRTC_Time[1], MyRTC_Time[2])){
                        MyRTC_Time[2] = 1;
                    }
                }
                current_param++;
                OLED_AnimUpdate();
                break;
                
            case 4: // 返回
                exit_flag = 1;
				OLED_Clear();
				OLED_Update();
                break;
        }
    }

    if(!exit_flag){ // 正常设置完成
        MyRTC_SetTime();
        /* 退出动画：向上收起 */
        OLED_Animation(0, 0, 128, 64, 0, 63, 128, 1);
        return 0;
    }
	else{
		return 0;
	}
    
}

/*--------------------------------------------------------------------------------------*/
/*---------------------------------二级时钟菜单------------------------------------------*/
/**
  * @brief 时钟二级菜单函数
  * @param  无
  * @retval 
  */
int menu2_Clock(void)
{
	int menu3_CClock;
	uint8_t clkflag = 1;//默认在第一行
	OLED_ShowString(0,0,"<-               ",OLED_8X16);
	OLED_ShowString(0,48,"            设置",OLED_8X16);
	OLED_Update();
	while(1)
	{	OLED_Printf(0,16,OLED_8X16,"Data:%d-%d-%d",MyRTC_Time[0],MyRTC_Time[1],MyRTC_Time[2]);
		OLED_Printf(0,32,OLED_8X16,"Time:%d:%d:%d",MyRTC_Time[3],MyRTC_Time[4],MyRTC_Time[5]);
		OLED_UpdateArea(0,16,128,32);
		MyRTC_ReadTime();//将RTC时间写入数组中
		KeyNum = Key_GetNumber();
		if(KeyNum == 1)//上一项
		{
			clkflag--;
			if(clkflag == 0){clkflag = 2;}//共两行,退出和设置
			D = 1;//上一项标志
			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
		}
		if(KeyNum == 2)//下一项
		{
			clkflag++;
			if(clkflag == 3){clkflag = 1;}
			D = 2;//下一项标志
			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
		}
		if(KeyNum == 3)//确认
			{
				OLED_Clear();
				OLED_Update();
				menu3_CClock = clkflag;
				OLED_AnimUpdate();//按下按钮,刷新一次动画效果
			}
		if(menu3_CClock == 1){return 0;}//返回0,退到第一级菜单
		if (menu3_CClock == 2){return menu3_SetClock();}
		
		
		switch(clkflag)
		{
			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
			case 1:
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_Printf(0,16,OLED_8X16,"Data:%d-%d-%d",MyRTC_Time[0],MyRTC_Time[1],MyRTC_Time[2]);
				OLED_Printf(0,32,OLED_8X16,"Time:%d:%d:%d",MyRTC_Time[3],MyRTC_Time[4],MyRTC_Time[5]);
				OLED_ShowString(0,48,"            设置",OLED_8X16);
				if(D==2){OLED_Animation(0,0,0,0,0,0,16,16);}//下
				if(D==1){OLED_Animation(96,48,32,16,0,0,16,16);}//上
				break;
			}
			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
			case 2:
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_Printf(0,16,OLED_8X16,"Data:%d-%d-%d",MyRTC_Time[0],MyRTC_Time[1],MyRTC_Time[2]);
				OLED_Printf(0,32,OLED_8X16,"Time:%d:%d:%d",MyRTC_Time[3],MyRTC_Time[4],MyRTC_Time[5]);
				OLED_ShowString(0,48,"            设置",OLED_8X16);
				if(D==2){OLED_Animation(0,0,16,16,96,48,32,16);}//下
				if(D==1){OLED_Animation(0,64,64,16,96,48,32,16);}//上
				break;
			}
		}
	}
}
/*--------------------------------------------------------------------------------------*/
/*---------------------------------二级串口菜单-----------------------------------------*/

/**
  * @brief 设置控制二级菜单函数
  * @param  无
  * @retval 
  */
int menu2_USART(void)
{
	int menu3_uSet;
	uint8_t usetflag = 1;//默认在第一行
	OLED_ShowString(0,0,"<-               ",OLED_8X16);
	OLED_ShowString(0,16,"蓝牙            ",OLED_8X16);
	OLED_ShowString(0,32,"WiFi            ",OLED_8X16);
	OLED_ShowString(0,48,"其他            ",OLED_8X16);
	OLED_Update();
	while(1)
	{
		KeyNum = Key_GetNumber();
		if(KeyNum == 1)//上一项
		{
			usetflag--;
			if(usetflag == 0){usetflag = 4;}
			D = 1;//上一项标志
			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
		}
		if(KeyNum == 2)//下一项
		{
			usetflag++;
			if(usetflag == 5){usetflag = 1;}
			D = 2;//下一项标志
			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
		}
		if(KeyNum == 3)//确认
		{
			OLED_Clear();
			OLED_Update();
			menu3_uSet = usetflag;
			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
		}
		if(menu3_uSet == 1){return 0;}//返回0,退到第一级菜单
		if(menu3_uSet == 2){return 0;}//返回0,退到第一级菜单(因为还没写三级菜单函数)
		if(menu3_uSet == 3){return 0;}//返回0,退到第一级菜单(因为还没写三级菜单函数)
		if(menu3_uSet == 4){return 0;}//返回0,退到第一级菜单(因为还没写三级菜单函数)
	
		
		switch(usetflag)
		{
			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
			case 1:
			{
					OLED_ShowString(0,0,"<-               ",OLED_8X16);
					OLED_ShowString(0,16,"蓝牙            ",OLED_8X16);
					OLED_ShowString(0,32,"WiFi            ",OLED_8X16);
					OLED_ShowString(0,48,"其他            ",OLED_8X16);
				if(D==2){OLED_Animation(0,0,0,0,0,0,16,16);}//下
				if(D==1){OLED_Animation(0,16,32,16,0,0,16,16);}//上
				break;
			}
			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
			case 2:
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_ShowString(0,16,"蓝牙            ",OLED_8X16);
				OLED_ShowString(0,32,"WiFi            ",OLED_8X16);
				OLED_ShowString(0,48,"其他            ",OLED_8X16);
				if(D==2){OLED_Animation(0,0,16,16,0,16,32,16);}//下
				if(D==1){OLED_Animation(0,32,64,16,0,16,32,16);}//上
				break;
			}
			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
			case 3:
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_ShowString(0,16,"蓝牙            ",OLED_8X16);
				OLED_ShowString(0,32,"WiFi            ",OLED_8X16);
				OLED_ShowString(0,48,"其他            ",OLED_8X16);		
				if(D==2){OLED_Animation(0,16,32,16,0,32,64,16);}//下
				if(D==1){OLED_Animation(0,48,32,16,0,32,64,16);}//上
				break;
			}
			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
			case 4:
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_ShowString(0,16,"蓝牙            ",OLED_8X16);
				OLED_ShowString(0,32,"WiFi            ",OLED_8X16);
				OLED_ShowString(0,48,"其他            ",OLED_8X16);
				if(D==2){OLED_Animation(0,32,64,16,0,48,32,16);}//下
				if(D==1){OLED_Animation(0,64,32,16,0,48,32,16);}//上
				break;
			}
		}
	}
}
/*--------------------------------------------------------------------------------------*/
/*---------------------------------二级设置菜单-----------------------------------------*/

/**
  * @brief 设置控制二级菜单函数
  * @param  无
  * @retval 
  */
int menu2_Set(void)
{
	int menu3_SSet;
	uint8_t setflag = 1;//默认在第一行
	OLED_ShowString(0,0,"<-               ",OLED_8X16);
	OLED_ShowString(0,16,"模式            ",OLED_8X16);
	OLED_ShowString(0,32,"动画设置        ",OLED_8X16);
	OLED_ShowString(0,48,"语言            ",OLED_8X16);
	OLED_Update();
	while(1)
	{
		KeyNum = Key_GetNumber();
		if(KeyNum == 1)//上一项
		{
			setflag--;
			if(setflag == 0){setflag = 5;}
			D = 1;//上一项标志
			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
		}
		if(KeyNum == 2)//下一项
		{
			setflag++;
			if(setflag == 6){setflag = 1;}
			D = 2;//下一项标志
			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
		}
		if(KeyNum == 3)//确认
		{
			OLED_Clear();
			OLED_Update();
			menu3_SSet = setflag;
			OLED_AnimUpdate();//按下按钮,刷新一次动画效果
		}
		if(menu3_SSet == 1){return 0;}//返回0,退到第一级菜单
		if(menu3_SSet == 2){return 0;}//返回0,退到第一级菜单(因为还没写三级菜单函数)
		if(menu3_SSet == 3){return 0;}//返回0,退到第一级菜单(因为还没写三级菜单函数)
		if(menu3_SSet == 4){return 0;}//返回0,退到第一级菜单(因为还没写三级菜单函数)
		if(menu3_SSet == 5){return 0;}//返回0,退到第一级菜单(因为还没写三级菜单函数)
		
		switch(setflag)
		{
			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
			case 1:
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_ShowString(0,16,"模式            ",OLED_8X16);
				OLED_ShowString(0,32,"动画设置        ",OLED_8X16);
				OLED_ShowString(0,48,"语言            ",OLED_8X16);
				if(D==2){OLED_Animation(0,0,0,0,0,0,16,16);}//下
				if(D==1){OLED_Animation(0,16,32,16,0,0,16,16);}//上
				break;
			}
			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
			case 2:
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_ShowString(0,16,"模式            ",OLED_8X16);
				OLED_ShowString(0,32,"动画设置        ",OLED_8X16);
				OLED_ShowString(0,48,"语言            ",OLED_8X16);
				if(D==2){OLED_Animation(0,0,16,16,0,16,32,16);}//下
				if(D==1){OLED_Animation(0,32,64,16,0,16,32,16);}//上
				break;
			}
			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
			case 3:
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_ShowString(0,16,"模式            ",OLED_8X16);
				OLED_ShowString(0,32,"动画设置        ",OLED_8X16);
				OLED_ShowString(0,48,"语言            ",OLED_8X16);
				if(D==2){OLED_Animation(0,16,32,16,0,32,64,16);}//下
				if(D==1){OLED_Animation(0,48,32,16,0,32,64,16);}//上
				break;
			}
			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
			case 4:
			{
				OLED_ShowString(0,0,"<-               ",OLED_8X16);
				OLED_ShowString(0,16,"模式            ",OLED_8X16);
				OLED_ShowString(0,32,"动画设置        ",OLED_8X16);
				OLED_ShowString(0,48,"语言            ",OLED_8X16);
				if(D==2){OLED_Animation(0,32,64,16,0,48,32,16);}//下
				if(D==1){OLED_Animation(0,64,32,16,0,48,32,16);}//上
				break;
			}
			//再次显示,解决乱码和闪烁问题(确保反相前是亮的状态)
			case 5:
			{
				OLED_ShowString(0,0,"模式              ",OLED_8X16);
				OLED_ShowString(0,16,"动画设置         ",OLED_8X16);
				OLED_ShowString(0,32,"语言             ",OLED_8X16);
				OLED_ShowString(0,48,"版本             ",OLED_8X16);
				if(D==2){OLED_Animation(0,32,32,16,0,48,32,16);}//下
				if(D==1){OLED_Animation(0,64,32,16,0,48,32,16);}//上
				break;
			}
		}
	}
}

void menu_do(void)
{		
		menu2 = menu1();
		if(menu2 == 1){menu2_Trace();}
		if(menu2 == 2){menu2_YunTai();}
		if(menu2 == 3){menu2_YaoGan();}
		if(menu2 == 4){menu2_MPU6050();}
//		if(menu2 == 5){return 0;}
		if(menu2 == 6){menu2_Clock();}
		if(menu2 == 7){menu2_USART();}
		if(menu2 == 8){menu2_Set();}



}
