
#include "gpio.h"

uint8_t EINT_Flag=0;

/*
*功    能：设置带上拉的gpio
*输入参数：	1、可以是 GPIO_INTPUT (gpio输入)/GPIO_OUTPUT	(gpio输出)
						2、端口号：0,1,2...
						3、引脚号：0~32（根据图纸设置）
*/
void SetGpioDirect(enum GPIO_DIR Direct, uint8_t GrpNum, uint8_t PinNum)
{
	if(GPIO_INTPUT == Direct)
	{
		FIO_SetDir(GrpNum, 1<<PinNum, 0);
	}
	else if(GPIO_OUTPUT == Direct)
	{
		FIO_SetDir(GrpNum, 1<<PinNum, 1); 
	}
	
	return;	
}


/*
*功    能：设置gpio输出电平
*输入参数：
		   1、端口号：0,1,2...
		   2、引脚号：0~32（根据图纸设置）
		   3、电平值:0/1
*/
void SetPinValue(uint8_t GrpNum, uint8_t PinNum, uint8_t Value)
{
	if(1 == Value)
	{
		FIO_SetValue(GrpNum,1<<PinNum);
	}
	else if(0 == Value)
	{
		FIO_ClearValue(GrpNum,1<<PinNum);
	}
	
	return;
}

/*
*功    能：获取输入端口所有引脚值
*输入参数：
		   1、端口号：0,1,2...
*返 回 值：32bit长度值，表示端口所有引脚状态
*/
int32_t GetAllPinValue(uint8_t GrpNum)
{
	int32_t PinValue = 0;

	PinValue = FIO_ReadValue(GrpNum);

	return PinValue;
}

/*
*功    能：获取输入引脚电平值
*输入参数：
		   1、端口号：0,1,2...
		   2、引脚号：0~32
*返 回 值：32bit长度值，0低， 1高， -1错误
*/
int32_t GetPinValue(uint8_t GrpNum, uint8_t PinNum)
{
	uint32_t PinMask;

	PinMask = 1<<PinNum;

	switch(GrpNum)
	{
		case 0:
			if((LPC_GPIO0->FIOPIN & PinMask) == PinMask)
			{
				return 1;
			}
			else
			{
				return 0;
			}

		case 1:
			if((LPC_GPIO1->FIOPIN & PinMask) == PinMask)
			{
				return 1;
			}
			else
			{
				return 0;
			}
			
		case 2:
			if((LPC_GPIO2->FIOPIN & PinMask) == PinMask)
			{
				return 1;
			}
			else
			{
				return 0;
			}

		case 3:
			if((LPC_GPIO3->FIOPIN & PinMask) == PinMask)
			{
				return 1;
			}
			else
			{
				return 0;
			}

		case 4:
			if((LPC_GPIO4->FIOPIN & PinMask) == PinMask)
			{
				return 1;
			}
			else
			{
				return 0;
			}

		default:
			break;
	}

	return -1;	
}

/*
*功    能：初始化用到GPIO
*/
void GPIOInit(void)
{
	//=========输出引脚=========
	SetGpioDirect(GPIO_OUTPUT, 1, 0);  //EN_MCU  
	SetPinValue(1,0,0);	 //高电平使能，默认低电平
	
	SetGpioDirect(GPIO_OUTPUT, 1, 18); //CW_MCU
	SetPinValue(1,18,1);	
	
	SetGpioDirect(GPIO_OUTPUT, 1, 20); //OUT_EN
	SetPinValue(1,20,1);  //高电平使能输出，默认高电平	
	
	SetGpioDirect(GPIO_OUTPUT, 1, 28); //M1 
	SetPinValue(1,28,1);	
	
 	SetGpioDirect(GPIO_OUTPUT, 1, 29); //M2  
 	SetPinValue(1,29,1);	
 	 
	SetGpioDirect(GPIO_OUTPUT, 1, 30); //M3
	SetPinValue(1,30,1);
		
	SetGpioDirect(GPIO_OUTPUT, 2, 2); //ANG
	SetPinValue(2,2,1);  //默认高电平

	SetGpioDirect(GPIO_OUTPUT, 2, 3); //STA
	SetPinValue(2,3,1);	 //默认低电平，非匀速
		
	//=========输入引脚=========
	SetGpioDirect(GPIO_INTPUT, 1, 21);//EMO         
	SetGpioDirect(GPIO_INTPUT, 1, 24);//SENSE_MCU   
	SetGpioDirect(GPIO_INTPUT, 1, 26);//DOWN    
	SetGpioDirect(GPIO_INTPUT, 2, 4); //MO
	
	SetGpioDirect(GPIO_INTPUT, 0, 2);//HWID 
	SetGpioDirect(GPIO_INTPUT, 0, 3);  
	SetGpioDirect(GPIO_INTPUT, 0, 4);  
	SetGpioDirect(GPIO_INTPUT, 0, 5);  
	SetGpioDirect(GPIO_INTPUT, 0, 6);  
	SetGpioDirect(GPIO_INTPUT, 0, 7);  
	SetGpioDirect(GPIO_INTPUT, 0, 8);  
	SetGpioDirect(GPIO_INTPUT, 0, 9);   

	return;
}

 void EINT3_IRQHandler(void)
 {
	GPIO_ClearInt(2,13);//清除中断
	EINT_Flag=1;//外部中断标志置位

	return;
 }

/*
*功能：使能外部中断
*参数：0：上升沿
	   1：下降沿
*/
void SenseIntEnable(uint8_t edgeState)
{
	uint32_t bitvalue=0;
	bitvalue |= (1<<13); 
	GPIO_IntCmd(2,bitvalue,edgeState);
	NVIC_EnableIRQ(EINT3_IRQn);
	NVIC_SetPriority(EINT3_IRQn,2);

	return;
}

/*
*功能：禁能外部中断
*/
void SenseIntDisable(void)
{
	NVIC_DisableIRQ(EINT3_IRQn);

	return;
}
