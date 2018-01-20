
#include "uart.h"
#include "pwm.h"
#include "dispose.h"
#include "motor.h"
#include "timer.h"

typedef struct
{
	uint8_t cmd;
	uint8_t data;

}ComData;

ComData g_ComData;
static uint8_t cmd_flag=0;

/*
��  �ܣ��ӻ�������ȡ����
��  ���� 1�����ݴ�ŵ�ַ
		 2��Ҫ�������ֽ�0~128

˵  �����˴���У�飬������ע�����ݴ�������С
*/
int32_t InquireUartRecvBuffer(uint8_t *pBuf, int8_t MaxBuffCount)
{
	int8_t i = 0;
	int8_t RcvCount = 0;

	while((FifoHead != FifoTail) && (MaxBuffCount > 0))
	{
		pBuf[i++] = FifoBuff[FifoTail];
		FifoBuff[FifoTail] = 0;
		FifoTail = (FifoTail + 1)%FIFO_SIZE;
		MaxBuffCount--;
		RcvCount++;
	}
	
	return RcvCount;
}

/*
��  �ܣ�У�����������
��  �����������ֽ�
*/
void CheckData(uint8_t byte)
{
	static uint8_t state=0;

	switch(state)
	{
		case 0:
		{
			 if(byte==0xFB)
			 {
			 	state=1;
			 }
			 else
			 {
			 	state=0;
			 }
		}
		break;

		case 1:
		{
			 g_ComData.data=byte;
			 state=2;
		}
		break;

		case 2:
		{
			 g_ComData.cmd=byte;
			 cmd_flag=1; //���ݿ�ʼ����
			 state=0;
		}
		break;

		default:
			break;
	}

	return;

}

/*
*��  ��: �Խ��յ����ݽ��н���

*˵  �����˴��밴��ͨ��Э�飬��д
*/
void Dispose(void)
{
	uint8_t tmp[10]={0};
	uint8_t count;
	uint8_t i;

	count=InquireUartRecvBuffer(tmp,10);
	if(count==0)
	{
		return;
	}

	for(i=0;i<count;i++)
	{
		CheckData(tmp[i]);
	}

	if(cmd_flag == 1)
	{	
		switch(g_ComData.cmd)
		{
			case 0xE7://�����ٶ�
			{
				g_MotorParam.speed=g_ComData.data;
				UART3SendData(g_ComData.cmd);
				UART3SendData(g_MotorParam.speed);
			}
			break;
			
			case 0xE8://���ڽǶ�
			{
			    g_MotorParam.distance=g_ComData.data;
			   	UART3SendData(g_ComData.cmd);
				UART3SendData(g_MotorParam.distance);
			}
			break;
			
			case 0xEA://����ģʽ
			{
			    g_MotorParam.mode=g_ComData.data;
				UART3SendData(g_ComData.cmd);
				UART3SendData(g_MotorParam.mode);
			}
			break;
			
			case 0xEC://�ָ�Ĭ������
			{
				  MotorParamReset();
 				  UART3SendData(g_ComData.cmd);
			}
			break;						
			
			case 0xED://��������ԭ��
			{
				  MotorReset();	
 				  UART3SendData(g_ComData.cmd);
			}
			break;						
			
			case 0xEE://�������
			{
			    g_MotorParam.action_flag=1;
				UART3SendData(g_ComData.cmd);
				UART3SendData(g_MotorParam.action_flag);
			}
			break;
			
			case 0xEF://ֹͣ���
			{
			    g_MotorParam.action_flag=0;
				UART3SendData(g_ComData.cmd);
				UART3SendData(g_MotorParam.action_flag);
			}
			break;	
			
			default:
				break;	
		}

		cmd_flag=0;
	}
}






