#include "pwm.h"

float Frequence[21]= {1, 2.709, 5.419, 13.547, 27.094, 40.640, 54.187, 67.734, 81.281, 94.827, 108.347, 121.921, 135.468, 149.014, 162.561, 176.108, 189.655, 203.201, 216.748, 230.295, 243.841};
uint8_t pwm_flag=0;

void PWMInit(void)
{
	/*使用固件库api*/
	PINSEL_CFG_Type 	 PinCfg_GPIO;		
	PWM_MATCHCFG_Type	 PWMMatch_Struct;
	PWM_TIMERCFG_Type	 PWMCfg_Struct;

	PinCfg_GPIO.Portnum 	= 3;
	PinCfg_GPIO.Funcnum		= 3;
	PinCfg_GPIO.Pinmode		= PINSEL_PINMODE_PULLUP;
	PinCfg_GPIO.OpenDrain	= PINSEL_PINMODE_NORMAL;
	PinCfg_GPIO.Pinnum		= 26; //pwm1.3
	PINSEL_ConfigPin(&PinCfg_GPIO);

	PWMCfg_Struct.PrescaleOption = PWM_TIMER_PRESCALE_TICKVAL;
	PWMCfg_Struct.PrescaleValue = 2;
	PWM_Init(LPC_PWM1, PWM_MODE_TIMER, &PWMCfg_Struct);//初始化

	PWMMatch_Struct.MatchChannel = 0;
	PWMMatch_Struct.ResetOnMatch = ENABLE;
	PWMMatch_Struct.IntOnMatch = DISABLE;
	PWMMatch_Struct.StopOnMatch = DISABLE;
	PWM_ConfigMatch(LPC_PWM1,&PWMMatch_Struct);	//设置匹配寄存器0
	PWM_MatchUpdate(LPC_PWM1,0,12500,PWM_MATCH_UPDATE_NOW);

	PWM_ChannelConfig(LPC_PWM1,3,PWM_CHANNEL_SINGLE_EDGE);//通道3单边沿模式
	PWMMatch_Struct.MatchChannel = 3;
	PWMMatch_Struct.ResetOnMatch = DISABLE;
	PWMMatch_Struct.IntOnMatch = DISABLE;
	PWMMatch_Struct.StopOnMatch = DISABLE;
	PWM_ConfigMatch(LPC_PWM1,&PWMMatch_Struct);	//设置匹配寄存器3
	PWM_MatchUpdate(LPC_PWM1,3,6250,PWM_MATCH_UPDATE_NOW);

	PWM_ChannelCmd(LPC_PWM1,3,ENABLE);//使能通道3PWM功能
	PWM_ResetCounter(LPC_PWM1);

//	PWM_CounterCmd(LPC_PWM1, ENABLE);//使能定时计数器
//	PWM_Cmd(LPC_PWM1, ENABLE); //使能PWM输出
	
	pwm_flag=0;	//初始化PWM，不输出脉冲

	/*直接操作寄存器*/
//	LPC_PINCON->PINSEL7 &= ~(0x3<<20); 	
//	LPC_PINCON->PINSEL7 |= (0x3<<20); //选择引脚功能 
//
//	LPC_PWM1->TCR = (1<<1);	//计数器复位
//	LPC_PWM1->MCR =(1<<1);	//匹配复位
//	LPC_PWM1->MR0 = 12500;  //设置频率
//	LPC_PWM1->MR3 = 6250;   //设置占空比
//	LPC_PWM1->LER =(1<<0)|(1<<3); //设置锁存器
//
//	LPC_PWM1->PCR |=(1<<11); //设置pwm控制寄存器
//	LPC_PWM1->TCR = (1<<0)|(1<<3); //设置定时器控制寄存器 

	return;
}

/*
*功  能：设置电脉冲频率
*参  数：1.频率档位 1~20
*/
void SetPMWFreq(uint8_t level)
{
	float value = Frequence[level];//根据速度档位选择频率
	PWM_MatchUpdate(LPC_PWM1,0,(uint32_t)(12500/value),PWM_MATCH_UPDATE_NOW);//更新频率
	PWM_MatchUpdate(LPC_PWM1,3,(uint32_t)(6250/value),PWM_MATCH_UPDATE_NOW); //更新占空比

	return;
}

/*
*功  能：停止脉冲输出
*/
void StopPWM(void)
{
	if(pwm_flag==1)
	{
		PWM_Cmd(LPC_PWM1, DISABLE); 
		PWM_CounterCmd(LPC_PWM1, DISABLE);
	 	PWM_ResetCounter(LPC_PWM1);

		pwm_flag = 0;
	}

	return;
}

/*
*功  能：开始脉冲输出
*/
void StartPWM(void)
{
	if(pwm_flag == 0)
	{
		PWM_CounterCmd(LPC_PWM1, ENABLE);	
		PWM_Cmd(LPC_PWM1, ENABLE);

		pwm_flag = 1;
	}

	return;
}


