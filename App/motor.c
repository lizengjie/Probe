#include "pwm.h"
#include "timer.h"
#include "gpio.h"
#include "motor.h"

static uint16_t distance[16]={50,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75};	//15µµŒª£¨0Œ™ƒ¨»œ…Ë÷√50∂»  µ•ŒªΩ«∂»
static uint16_t speed[21]={100,5,10,25,50,75,100,125,150,175,200,225,250,275,300,325,350,375,400,425,450};	//20µµŒª£¨0Œ™ƒ¨»œ…Ë÷√100∂»√øs  µ•ŒªΩ«∂»√ø√Î
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

	SetPMWFreq(g_MotorParam.speed); //∏˘æ›≤ªÕ¨µµŒª…Ë÷√≤ªÕ¨∆µ¬ 
	time=1000*(distance[g_MotorParam.distance]/speed[g_MotorParam.speed]/2); //º∆À„ªªœÚ ±º‰	µ•Œª∫¡√Î

	while(g_MotorParam.location != 0)
	{
		switch(g_MotorParam.location)
		{
			case 0:
				break;
	
			case 1:
			{
				 MotorDirection(0);//…Ë÷√œÚ∏∫∑ΩœÚ∞⁄∂Ø
				 SenseIntEnable(0);//…œ…˝—ÿ÷–∂œ
				 ResetTimer();
				 StartPWM();
				 StartTimer();
				 while(timer_value<(time*2))
				 {
				 	if(EINT_Flag==1)
					{
						EINT_Flag=0;
					   	StopPWM();//Ω˚ƒ‹PWM ‰≥ˆ
				 		StopTimer();
				 		ResetTimer();//º∆ ±∆˜∏¥Œª
	
				 		g_MotorParam.location=0;//µΩ¥Ô‘≠µ„
						SenseIntDisable();

						break;
					}
				 }
				 StopPWM();//Ω˚ƒ‹PWM ‰≥ˆ
				 StopTimer();
				 ResetTimer();//º∆ ±∆˜∏¥Œª
	
				 g_MotorParam.location=2;//µΩ¥Ô∑¥œÚ◊Ó¥ÛŒª÷√
	
	 			 StartTimer();//ø™ ºº∆ ±£¨—” ±500ms
				 while(timer_value<500);
				 StopTimer();
				 ResetTimer();				
			}
			break;
	
			case 2:
			{
				 MotorDirection(1);//…Ë÷√œÚ’˝∑ΩœÚ∞⁄∂Ø
				 SenseIntEnable(1);//œ¬Ωµ—ÿ—ÿ÷–∂œ
				 ResetTimer();
				 StartPWM();
				 StartTimer();
				 while(timer_value<(time*2))
				 {
				 	if(EINT_Flag==1)
					{
						EINT_Flag=0;
					   	StopPWM();//Ω˚ƒ‹PWM ‰≥ˆ
				 		StopTimer();
				 		ResetTimer();//º∆ ±∆˜∏¥Œª
	
				 		g_MotorParam.location=0;//µΩ¥Ô‘≠µ„
						SenseIntDisable();

						break;
					}
				 }
				 StopPWM();//Ω˚ƒ‹PWM ‰≥ˆ
				 StopTimer();
				 ResetTimer();//º∆ ±∆˜∏¥Œª
	
				 g_MotorParam.location=1;//µΩ¥Ô’˝œÚ◊Ó¥ÛŒª÷√
	
	 			 StartTimer();//ø™ ºº∆ ±£¨—” ±500ms
				 while(timer_value<500);
				 StopTimer();
				 ResetTimer();		
			}
			break;
	
			default:
				break;
		} 

		//»´≥Ã∞⁄∂Ø10¥Œ£¨Œ¥’“µΩ‘≠µ„£¨ºŸ…Ë“—¥¶”⁄‘≠µ„£¨–Ë“™±®∏Ê¥ÌŒÛ–≈œ¢
		count++;
		if(count>10)
		{
			   	StopPWM();//Ω˚ƒ‹PWM ‰≥ˆ
		 		StopTimer();
		 		ResetTimer();//º∆ ±∆˜∏¥Œª

		 		g_MotorParam.location=0;//ºŸ…ËµΩ¥Ô‘≠µ„
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
			 MotorDirection(0);//…Ë÷√‘À∂Ø∑ΩœÚŒ™∏∫œÚ
			 StartPWM();//pwm ‰≥ˆ
			 ResetTimer();//º∆ ±∆˜∏¥Œª
			 StartTimer();//ø™ ºº∆ ±
			 while(timer_value<time);
			 StopPWM();//Ω˚ƒ‹PWM ‰≥ˆ
			 StopTimer();
			 ResetTimer();//º∆ ±∆˜
			 g_MotorParam.location=2; //µΩ¥Ô∏∫œÚ◊Ó¥ÛŒª÷√	

			 StartTimer();//ø™ ºº∆ ±£¨—” ±500ms
			 while(timer_value<500);
			 StopTimer();
			 ResetTimer();
		}
		break;

		case 1:
			break;	//µΩ¥Ô’˝œÚ◊Ó¥ÛŒª÷√£¨≤ª◊ˆ¥¶¿Ì

		case 2:
		{
			 MotorDirection(1);//‘⁄∏∫œÚ◊Ó¥ÛŒª÷√ªªœÚ£¨’˝œÚ∞⁄∂Ø
			 StartPWM();//pwm ‰≥ˆ
			 ResetTimer();//º∆ ±∆˜∏¥Œª
			 StartTimer();//ø™ ºº∆ ±
			 while(timer_value<(time*2));
			 StopPWM();//Ω˚ƒ‹PWM ‰≥ˆ
			 StopTimer();
			 ResetTimer();//º∆ ±∆˜∏¥Œª

			 g_MotorParam.location=1; //µΩ¥Ô’˝œÚ◊Ó¥ÛŒª÷√

 			 StartTimer();//ø™ ºº∆ ±£¨—” ±500ms
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
			 MotorDirection(1);//…Ë÷√‘À∂Ø∑ΩœÚŒ™’˝∑ΩœÚ
			 StartPWM();//pwm ‰≥ˆ
			 ResetTimer();//º∆ ±∆˜∏¥Œª
			 StartTimer();//ø™ ºº∆ ±
			 while(timer_value<time);
			 StopPWM();//Ω˚ƒ‹PWM ‰≥ˆ
			 StopTimer();
			 ResetTimer();//º∆ ±∆˜ª		

			 g_MotorParam.location=1;//µΩ¥Ô’˝œÚ◊Ó¥ÛŒª÷√
			 
 			 StartTimer();//ø™ ºº∆ ±£¨—” ±500ms
			 while(timer_value<500);
			 StopTimer();
			 ResetTimer();		 			 	
		}
		break;

		case 1:
		{
			 MotorDirection(0);//ªªœÚ£¨∑¥œÚ∞⁄∂Ø
			 StartPWM();//pwm ‰≥ˆ
			 ResetTimer();//º∆ ±∆˜∏¥Œª
			 StartTimer();//ø™ ºº∆ ±
			 while(timer_value<(time*2));
			 StopPWM();//Ω˚ƒ‹PWM ‰≥ˆ
			 StopTimer();
			 ResetTimer();//º∆ ±∆˜∏¥Œª

			 g_MotorParam.location=2;//µΩ¥Ô∑¥œÚ◊Ó¥ÛŒª÷√

 			 StartTimer();//ø™ ºº∆ ±£¨—” ±500ms
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
			 MotorDirection(0);//…Ë÷√‘À∂Ø∑ΩœÚŒ™∏∫œÚ
			 StartPWM();//pwm ‰≥ˆ
			 ResetTimer();//º∆ ±∆˜∏¥Œª
			 StartTimer();//ø™ ºº∆ ±
			 while(timer_value<time);
			 StopPWM();//Ω˚ƒ‹PWM ‰≥ˆ
			 StopTimer();
			 ResetTimer();//º∆ ±∆˜
			 g_MotorParam.location=2; //µΩ¥Ô∏∫œÚ◊Ó¥ÛŒª÷√	

			 StartTimer();//ø™ ºº∆ ±£¨—” ±500ms
			 while(timer_value<500);
			 StopTimer();
			 ResetTimer();			 	
		}
		break;

		case 1:
		{
			 MotorDirection(0);//ªªœÚ£¨∑¥œÚ∞⁄∂Ø
			 StartPWM();//pwm ‰≥ˆ
			 ResetTimer();//º∆ ±∆˜∏¥Œª
			 StartTimer();//ø™ ºº∆ ±
			 while(timer_value<(time*2));
			 StopPWM();//Ω˚ƒ‹PWM ‰≥ˆ
			 StopTimer();
			 ResetTimer();//º∆ ±∆˜∏¥Œª

			 g_MotorParam.location=2;//µΩ¥Ô∑¥œÚ◊Ó¥ÛŒª÷√

 			 StartTimer();//ø™ ºº∆ ±£¨—” ±500ms
			 while(timer_value<500);
			 StopTimer();
			 ResetTimer();				
		}
		break;

		case 2:
		{
			 MotorDirection(1);//‘⁄∏∫œÚ◊Ó¥ÛŒª÷√ªªœÚ£¨’˝œÚ∞⁄∂Ø
			 StartPWM();//pwm ‰≥ˆ
			 ResetTimer();//º∆ ±∆˜∏¥Œª
			 StartTimer();//ø™ ºº∆ ±
			 while(timer_value<(time*2));
			 StopPWM();//Ω˚ƒ‹PWM ‰≥ˆ
			 StopTimer();
			 ResetTimer();//º∆ ±∆˜∏¥Œª

			 g_MotorParam.location=1; //µΩ¥Ô’˝œÚ◊Ó¥ÛŒª÷√

 			 StartTimer();//ø™ ºº∆ ±£¨—” ±500ms
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
		SetPMWFreq(g_MotorParam.speed); //∏˘æ›≤ªÕ¨µµŒª…Ë÷√≤ªÕ¨∆µ¬ 
		time=1000*(distance[g_MotorParam.distance]/speed[g_MotorParam.speed]/2); //º∆À„ªªœÚ ±º‰	µ•Œª∫¡√Î

		switch(g_MotorParam.mode)
		{
			case 1: //’˝œÚ∞⁄∂Ø“ª¥Œ
			{
				P_Swing(time);	
			}
			break;
			
			case 2: //Õ˘∑µ∞⁄∂Ø
 			{
				N_Swing(time);
			}
			break;
			
			case 3: //∏∫œÚ∞⁄∂Ø“ª¥Œ
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

	down=GetPinValue(1,26);//π˝¡˜±£ª§º‡≤‚
	emo=GetPinValue(1,21);//µÕ∆µ±£ª§º‡≤‚
	
	if((down==0)||(emo==0))
	{
		SetPinValue(1,0,0);//Ω˚ƒ‹«˝∂Ø–æ∆¨
	}
	
	return;		
}

void MonitorDistance(void)
{
		
}

