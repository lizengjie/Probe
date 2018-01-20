
#include "gpio.h"

uint8_t EINT_Flag=0;

/*
*��    �ܣ����ô�������gpio
*���������	1�������� GPIO_INTPUT (gpio����)/GPIO_OUTPUT	(gpio���)
						2���˿ںţ�0,1,2...
						3�����źţ�0~32������ͼֽ���ã�
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
*��    �ܣ�����gpio�����ƽ
*���������
		   1���˿ںţ�0,1,2...
		   2�����źţ�0~32������ͼֽ���ã�
		   3����ƽֵ:0/1
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
*��    �ܣ���ȡ����˿���������ֵ
*���������
		   1���˿ںţ�0,1,2...
*�� �� ֵ��32bit����ֵ����ʾ�˿���������״̬
*/
int32_t GetAllPinValue(uint8_t GrpNum)
{
	int32_t PinValue = 0;

	PinValue = FIO_ReadValue(GrpNum);

	return PinValue;
}

/*
*��    �ܣ���ȡ�������ŵ�ƽֵ
*���������
		   1���˿ںţ�0,1,2...
		   2�����źţ�0~32
*�� �� ֵ��32bit����ֵ��0�ͣ� 1�ߣ� -1����
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
*��    �ܣ���ʼ���õ�GPIO
*/
void GPIOInit(void)
{
	//=========�������=========
	SetGpioDirect(GPIO_OUTPUT, 1, 0);  //EN_MCU  
	SetPinValue(1,0,0);	 //�ߵ�ƽʹ�ܣ�Ĭ�ϵ͵�ƽ
	
	SetGpioDirect(GPIO_OUTPUT, 1, 18); //CW_MCU
	SetPinValue(1,18,1);	
	
	SetGpioDirect(GPIO_OUTPUT, 1, 20); //OUT_EN
	SetPinValue(1,20,1);  //�ߵ�ƽʹ�������Ĭ�ϸߵ�ƽ	
	
	SetGpioDirect(GPIO_OUTPUT, 1, 28); //M1 
	SetPinValue(1,28,1);	
	
 	SetGpioDirect(GPIO_OUTPUT, 1, 29); //M2  
 	SetPinValue(1,29,1);	
 	 
	SetGpioDirect(GPIO_OUTPUT, 1, 30); //M3
	SetPinValue(1,30,1);
		
	SetGpioDirect(GPIO_OUTPUT, 2, 2); //ANG
	SetPinValue(2,2,1);  //Ĭ�ϸߵ�ƽ

	SetGpioDirect(GPIO_OUTPUT, 2, 3); //STA
	SetPinValue(2,3,1);	 //Ĭ�ϵ͵�ƽ��������
		
	//=========��������=========
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
	GPIO_ClearInt(2,13);//����ж�
	EINT_Flag=1;//�ⲿ�жϱ�־��λ

	return;
 }

/*
*���ܣ�ʹ���ⲿ�ж�
*������0��������
	   1���½���
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
*���ܣ������ⲿ�ж�
*/
void SenseIntDisable(void)
{
	NVIC_DisableIRQ(EINT3_IRQn);

	return;
}
