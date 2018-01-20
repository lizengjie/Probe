#include "timer.h"
#include "uart.h"

/*计时值*/
volatile uint32_t timer_value=0;

/*
*功  能：配置使用定时器
*/
void TimerInit(void)
{
		TIM_TIMERCFG_Type TimerCfgStruct;
		TIM_MATCHCFG_Type TimerMatchCfgStruct;
	
		TimerCfgStruct.PrescaleOption=TIM_PRESCALE_TICKVAL;
		TimerCfgStruct.PrescaleValue=1;
		TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &TimerCfgStruct);
	
		TimerMatchCfgStruct.MatchChannel=0;
		TimerMatchCfgStruct.MatchValue=25000; //1ms
		TimerMatchCfgStruct.IntOnMatch=ENABLE;
		TimerMatchCfgStruct.ResetOnMatch=ENABLE;
		TimerMatchCfgStruct.StopOnMatch=DISABLE;
		TIM_ConfigMatch(LPC_TIM0,&TimerMatchCfgStruct);

 		NVIC_SetPriority(TIMER0_IRQn,2);
		NVIC_EnableIRQ(TIMER0_IRQn);

		TIM_ResetCounter(LPC_TIM0);
		TIM_Cmd(LPC_TIM0,DISABLE);

//		LPC_TIM0->PR  = 0x63;		/* set prescaler to zero */
//		LPC_TIM0->MR0 = 25000;
//		LPC_TIM0->MCR = 3;			/* Interrupt and Reset on MR0 */
//	
//		NVIC_EnableIRQ(TIMER0_IRQn);
//
//		LPC_TIM0->TCR = 1;
	
		return;
}

void TIMER0_IRQHandler()
{
		TIM_ClearIntPending(LPC_TIM0,TIM_MR0_INT); //一定要记得清除中断标志
		timer_value++;
		UART3SendData('0');
		if(timer_value>600000)//超过10min
		{
			 timer_value=0;
		}
		return;
}

/*
*功  能：启动开始计时
*/
void StartTimer(void)
{
		TIM_ResetCounter(LPC_TIM0);
		TIM_Cmd(LPC_TIM0,ENABLE);
	
		return;
}

/*
*功  能：停止计时
*/
void StopTimer(void)
{
		TIM_Cmd(LPC_TIM0,DISABLE);
	  
	    return;
}

/*
*功  能：复位计时器
*/
void ResetTimer(void)
{
		timer_value=0;
	 
		return;
}






