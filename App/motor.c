#include "pwm.h"
#include "timer.h"
#include "gpio.h"
#include "motor.h"

static uint16_t distance[16]={50,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75};	//15档位，0为默认设置50度  单位角度
static uint16_t speed[21]={100,5,10,25,50,75,100,125,150,175,200,225,250,275,300,325,350,375,400,425,450};	//20档位，0为默认设置100度每s  单位角度每秒
MotorParam g_MotorParam;

static void  MotorDirection(uint8_t dir)
{
	if(dir==0)
		SetPinValue(1,18,0);
	else
		SetPinValue(1,18,1);
		
	return;			
}

void MotorParamReset(void)
{
	g_MotorParam.speed=0;
	g_MotorParam.distance=0;
	g_MotorParam.mode=2;
	g_MotorParam.location=2;

	return;
}

void MotorReset(void)
{
	uint32_t time=0;
	static uint32_t count=0;

	SetPMWFreq(g_MotorParam.speed); //根据不同档位设置不同频率
	time=1000*(distance[g_MotorParam.distance]/speed[g_MotorParam.speed]/2); //计算换向时间	单位毫秒

	while(g_MotorParam.location != 0)
	{
		switch(g_MotorParam.location)
		{
			case 0:
				break;
	
			case 1:
			{
				 MotorDirection(0);//设置向负方向摆动
				 SenseIntEnable(0);//上升沿中断
				 ResetTimer();
				 StartPWM();
				 StartTimer();
				 while(timer_value<(time*2))
				 {
				 	if(EINT_Flag==1)
					{
						EINT_Flag=0;
					   	StopPWM();//禁能PWM输出
				 		StopTimer();
				 		ResetTimer();//计时器复位
	
				 		g_MotorParam.location=0;//到达原点
						SenseIntDisable();

						break;
					}
				 }
				 StopPWM();//禁能PWM输出
				 StopTimer();
				 ResetTimer();//计时器复位
	
				 g_MotorParam.location=2;//到达反向最大位置
	
	 			 StartTimer();//开始计时，延时500ms
				 while(timer_value<500);
				 StopTimer();
				 ResetTimer();				
			}
			break;
	
			case 2:
			{
				 MotorDirection(1);//设置向正方向摆动
				 SenseIntEnable(1);//下降沿沿中断
				 ResetTimer();
				 StartPWM();
				 StartTimer();
				 while(timer_value<(time*2))
				 {
				 	if(EINT_Flag==1)
					{
						EINT_Flag=0;
					   	StopPWM();//禁能PWM输出
				 		StopTimer();
				 		ResetTimer();//计时器复位
	
				 		g_MotorParam.location=0;//到达原点
						SenseIntDisable();

						break;
					}
				 }
				 StopPWM();//禁能PWM输出
				 StopTimer();
				 ResetTimer();//计时器复位
	
				 g_MotorParam.location=1;//到达正向最大位置
	
	 			 StartTimer();//开始计时，延时500ms
				 while(timer_value<500);
				 StopTimer();
				 ResetTimer();		
			}
			break;
	
			default:
				break;
		} 

		//全程摆动10次，未找到原点，假设已处于原点，需要报告错误信息
		count++;
		if(count>10)
		{
			   	StopPWM();//禁能PWM输出
		 		StopTimer();
		 		ResetTimer();//计时器复位

		 		g_MotorParam.location=0;//假设到达原点
				SenseIntDisable();
		}

	}

	return;
}

void P_Swing(uint32_t time)
{
	switch(g_MotorParam.location)
	{
		case 0:
		{
			 MotorDirection(0);//设置运动方向为负向
			 StartPWM();//pwm输出
			 ResetTimer();//计时器复位
			 StartTimer();//开始计时
			 while(timer_value<time);
			 StopPWM();//禁能PWM输出
			 StopTimer();
			 ResetTimer();//计时器
			 g_MotorParam.location=2; //到达负向最大位置	

			 StartTimer();//开始计时，延时500ms
			 while(timer_value<500);
			 StopTimer();
			 ResetTimer();
		}
		break;

		case 1:
			break;	//到达正向最大位置，不做处理

		case 2:
		{
			 MotorDirection(1);//在负向最大位置换向，正向摆动
			 StartPWM();//pwm输出
			 ResetTimer();//计时器复位
			 StartTimer();//开始计时
			 while(timer_value<(time*2));
			 StopPWM();//禁能PWM输出
			 StopTimer();
			 ResetTimer();//计时器复位

			 g_MotorParam.location=1; //到达正向最大位置

 			 StartTimer();//开始计时，延时500ms
			 while(timer_value<500);
			 StopTimer();
			 ResetTimer();
		}
		break;	
			
		default:
			break;	
	}

	return;
}
void N_Swing(uint32_t time)
{
	switch(g_MotorParam.location)
	{
		case 0:
		{
			 MotorDirection(1);//设置运动方向为正方向
			 StartPWM();//pwm输出
			 ResetTimer();//计时器复位
			 StartTimer();//开始计时
			 while(timer_value<time);
			 StopPWM();//禁能PWM输出
			 StopTimer();
			 ResetTimer();//计时器�		

			 g_MotorParam.location=1;//到达正向最大位置
			 
 			 StartTimer();//开始计时，延时500ms
			 while(timer_value<500);
			 StopTimer();
			 ResetTimer();		 			 	
		}
		break;

		case 1:
		{
			 MotorDirection(0);//换向，反向摆动
			 StartPWM();//pwm输出
			 ResetTimer();//计时器复位
			 StartTimer();//开始计时
			 while(timer_value<(time*2));
			 StopPWM();//禁能PWM输出
			 StopTimer();
			 ResetTimer();//计时器复位

			 g_MotorParam.location=2;//到达反向最大位置

 			 StartTimer();//开始计时，延时500ms
			 while(timer_value<500);
			 StopTimer();
			 ResetTimer();			 			
		}
		break;

		case 2:
			break;	
			
		default:
			break;	
	}

	return;
}

void PN_Swing(uint32_t time)
{
	switch(g_MotorParam.location)
	{
		case 0:
		{
			 MotorDirection(0);//设置运动方向为负向
			 StartPWM();//pwm输出
			 ResetTimer();//计时器复位
			 StartTimer();//开始计时
			 while(timer_value<time);
			 StopPWM();//禁能PWM输出
			 StopTimer();
			 ResetTimer();//计时器
			 g_MotorParam.location=2; //到达负向最大位置	

			 StartTimer();//开始计时，延时500ms
			 while(timer_value<500);
			 StopTimer();
			 ResetTimer();			 	
		}
		break;

		case 1:
		{
			 MotorDirection(0);//换向，反向摆动
			 StartPWM();//pwm输出
			 ResetTimer();//计时器复位
			 StartTimer();//开始计时
			 while(timer_value<(time*2));
			 StopPWM();//禁能PWM输出
			 StopTimer();
			 ResetTimer();//计时器复位

			 g_MotorParam.location=2;//到达反向最大位置

 			 StartTimer();//开始计时，延时500ms
			 while(timer_value<500);
			 StopTimer();
			 ResetTimer();				
		}
		break;

		case 2:
		{
			 MotorDirection(1);//在负向最大位置换向，正向摆动
			 StartPWM();//pwm输出
			 ResetTimer();//计时器复位
			 StartTimer();//开始计时
			 while(timer_value<(time*2));
			 StopPWM();//禁能PWM输出
			 StopTimer();
			 ResetTimer();//计时器复位

			 g_MotorParam.location=1; //到达正向最大位置

 			 StartTimer();//开始计时，延时500ms
			 while(timer_value<500);
			 StopTimer();
			 ResetTimer();		
		}
		break;
			
		default:
			break;	
	}

	return;	
}

void MotorAction(void)
{
	uint32_t time=0;

	if(g_MotorParam.action_flag == 1)
	{
		SetPMWFreq(g_MotorParam.speed); //根据不同档位设置不同频率
		time=1000*(distance[g_MotorParam.distance]/speed[g_MotorParam.speed]/2); //计算换向时间	单位毫秒

		switch(g_MotorParam.mode)
		{
			case 1: //正向摆动一次
			{
				P_Swing(time);	
			}
			break;
			
			case 2: //往返摆动
 			{
				N_Swing(time);
			}
			break;
			
			case 3: //负向摆动一次
			{
				PN_Swing(time);
			}
			break;
			
			default:
				break;
		}
	}	
	
	return;
}

void MonitorFault(void)
{
	int32_t emo,down;

	down=GetPinValue(1,26);//过流保护监测
	emo=GetPinValue(1,21);//低频保护监测
	
	if((down==0)||(emo==0))
	{
		SetPinValue(1,0,0);//禁能驱动芯片
	}
	
	return;		
}

void MonitorDistance(void)
{
		
}

