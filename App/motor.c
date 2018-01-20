#include "pwm.h"
#include "timer.h"
#include "gpio.h"
#include "motor.h"

static uint16_t distance[16]={50,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75};	//15��λ��0ΪĬ������50��  ��λ�Ƕ�
static uint16_t speed[21]={100,5,10,25,50,75,100,125,150,175,200,225,250,275,300,325,350,375,400,425,450};	//20��λ��0ΪĬ������100��ÿs  ��λ�Ƕ�ÿ��
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

	SetPMWFreq(g_MotorParam.speed); //���ݲ�ͬ��λ���ò�ͬƵ��
	time=1000*(distance[g_MotorParam.distance]/speed[g_MotorParam.speed]/2); //���㻻��ʱ��	��λ����

	while(g_MotorParam.location != 0)
	{
		switch(g_MotorParam.location)
		{
			case 0:
				break;
	
			case 1:
			{
				 MotorDirection(0);//�����򸺷���ڶ�
				 SenseIntEnable(0);//�������ж�
				 ResetTimer();
				 StartPWM();
				 StartTimer();
				 while(timer_value<(time*2))
				 {
				 	if(EINT_Flag==1)
					{
						EINT_Flag=0;
					   	StopPWM();//����PWM���
				 		StopTimer();
				 		ResetTimer();//��ʱ����λ
	
				 		g_MotorParam.location=0;//����ԭ��
						SenseIntDisable();

						break;
					}
				 }
				 StopPWM();//����PWM���
				 StopTimer();
				 ResetTimer();//��ʱ����λ
	
				 g_MotorParam.location=2;//���ﷴ�����λ��
	
	 			 StartTimer();//��ʼ��ʱ����ʱ500ms
				 while(timer_value<500);
				 StopTimer();
				 ResetTimer();				
			}
			break;
	
			case 2:
			{
				 MotorDirection(1);//������������ڶ�
				 SenseIntEnable(1);//�½������ж�
				 ResetTimer();
				 StartPWM();
				 StartTimer();
				 while(timer_value<(time*2))
				 {
				 	if(EINT_Flag==1)
					{
						EINT_Flag=0;
					   	StopPWM();//����PWM���
				 		StopTimer();
				 		ResetTimer();//��ʱ����λ
	
				 		g_MotorParam.location=0;//����ԭ��
						SenseIntDisable();

						break;
					}
				 }
				 StopPWM();//����PWM���
				 StopTimer();
				 ResetTimer();//��ʱ����λ
	
				 g_MotorParam.location=1;//�����������λ��
	
	 			 StartTimer();//��ʼ��ʱ����ʱ500ms
				 while(timer_value<500);
				 StopTimer();
				 ResetTimer();		
			}
			break;
	
			default:
				break;
		} 

		//ȫ�̰ڶ�10�Σ�δ�ҵ�ԭ�㣬�����Ѵ���ԭ�㣬��Ҫ���������Ϣ
		count++;
		if(count>10)
		{
			   	StopPWM();//����PWM���
		 		StopTimer();
		 		ResetTimer();//��ʱ����λ

		 		g_MotorParam.location=0;//���赽��ԭ��
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
			 MotorDirection(0);//�����˶�����Ϊ����
			 StartPWM();//pwm���
			 ResetTimer();//��ʱ����λ
			 StartTimer();//��ʼ��ʱ
			 while(timer_value<time);
			 StopPWM();//����PWM���
			 StopTimer();
			 ResetTimer();//��ʱ��
			 g_MotorParam.location=2; //���︺�����λ��	

			 StartTimer();//��ʼ��ʱ����ʱ500ms
			 while(timer_value<500);
			 StopTimer();
			 ResetTimer();
		}
		break;

		case 1:
			break;	//�����������λ�ã���������

		case 2:
		{
			 MotorDirection(1);//�ڸ������λ�û�������ڶ�
			 StartPWM();//pwm���
			 ResetTimer();//��ʱ����λ
			 StartTimer();//��ʼ��ʱ
			 while(timer_value<(time*2));
			 StopPWM();//����PWM���
			 StopTimer();
			 ResetTimer();//��ʱ����λ

			 g_MotorParam.location=1; //�����������λ��

 			 StartTimer();//��ʼ��ʱ����ʱ500ms
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
			 MotorDirection(1);//�����˶�����Ϊ������
			 StartPWM();//pwm���
			 ResetTimer();//��ʱ����λ
			 StartTimer();//��ʼ��ʱ
			 while(timer_value<time);
			 StopPWM();//����PWM���
			 StopTimer();
			 ResetTimer();//��ʱ���		

			 g_MotorParam.location=1;//�����������λ��
			 
 			 StartTimer();//��ʼ��ʱ����ʱ500ms
			 while(timer_value<500);
			 StopTimer();
			 ResetTimer();		 			 	
		}
		break;

		case 1:
		{
			 MotorDirection(0);//���򣬷���ڶ�
			 StartPWM();//pwm���
			 ResetTimer();//��ʱ����λ
			 StartTimer();//��ʼ��ʱ
			 while(timer_value<(time*2));
			 StopPWM();//����PWM���
			 StopTimer();
			 ResetTimer();//��ʱ����λ

			 g_MotorParam.location=2;//���ﷴ�����λ��

 			 StartTimer();//��ʼ��ʱ����ʱ500ms
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
			 MotorDirection(0);//�����˶�����Ϊ����
			 StartPWM();//pwm���
			 ResetTimer();//��ʱ����λ
			 StartTimer();//��ʼ��ʱ
			 while(timer_value<time);
			 StopPWM();//����PWM���
			 StopTimer();
			 ResetTimer();//��ʱ��
			 g_MotorParam.location=2; //���︺�����λ��	

			 StartTimer();//��ʼ��ʱ����ʱ500ms
			 while(timer_value<500);
			 StopTimer();
			 ResetTimer();			 	
		}
		break;

		case 1:
		{
			 MotorDirection(0);//���򣬷���ڶ�
			 StartPWM();//pwm���
			 ResetTimer();//��ʱ����λ
			 StartTimer();//��ʼ��ʱ
			 while(timer_value<(time*2));
			 StopPWM();//����PWM���
			 StopTimer();
			 ResetTimer();//��ʱ����λ

			 g_MotorParam.location=2;//���ﷴ�����λ��

 			 StartTimer();//��ʼ��ʱ����ʱ500ms
			 while(timer_value<500);
			 StopTimer();
			 ResetTimer();				
		}
		break;

		case 2:
		{
			 MotorDirection(1);//�ڸ������λ�û�������ڶ�
			 StartPWM();//pwm���
			 ResetTimer();//��ʱ����λ
			 StartTimer();//��ʼ��ʱ
			 while(timer_value<(time*2));
			 StopPWM();//����PWM���
			 StopTimer();
			 ResetTimer();//��ʱ����λ

			 g_MotorParam.location=1; //�����������λ��

 			 StartTimer();//��ʼ��ʱ����ʱ500ms
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
		SetPMWFreq(g_MotorParam.speed); //���ݲ�ͬ��λ���ò�ͬƵ��
		time=1000*(distance[g_MotorParam.distance]/speed[g_MotorParam.speed]/2); //���㻻��ʱ��	��λ����

		switch(g_MotorParam.mode)
		{
			case 1: //����ڶ�һ��
			{
				P_Swing(time);	
			}
			break;
			
			case 2: //�����ڶ�
 			{
				N_Swing(time);
			}
			break;
			
			case 3: //����ڶ�һ��
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

	down=GetPinValue(1,26);//�����������
	emo=GetPinValue(1,21);//��Ƶ�������
	
	if((down==0)||(emo==0))
	{
		SetPinValue(1,0,0);//��������оƬ
	}
	
	return;		
}

void MonitorDistance(void)
{
		
}

