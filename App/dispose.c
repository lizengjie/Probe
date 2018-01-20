
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
功  能：从缓冲区读取数据
参  数： 1、数据存放地址
		 2、要读多少字节0~128

说  明：此处无校验，请自行注意数据存放区域大小
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
功  能：校验读出的数据
参  数：待解析字节
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
			 cmd_flag=1; //数据开始解析
			 state=0;
		}
		break;

		default:
			break;
	}

	return;

}

/*
*功  能: 对接收的数据进行解析

*说  明：此处请按照通信协议，重写
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
			case 0xE7://调节速度
			{
				g_MotorParam.speed=g_ComData.data;
				UART3SendData(g_ComData.cmd);
				UART3SendData(g_MotorParam.speed);
			}
			break;
			
			case 0xE8://调节角度
			{
			    g_MotorParam.distance=g_ComData.data;
			   	UART3SendData(g_ComData.cmd);
				UART3SendData(g_MotorParam.distance);
			}
			break;
			
			case 0xEA://调节模式
			{
			    g_MotorParam.mode=g_ComData.data;
				UART3SendData(g_ComData.cmd);
				UART3SendData(g_MotorParam.mode);
			}
			break;
			
			case 0xEC://恢复默认设置
			{
				  MotorParamReset();
 				  UART3SendData(g_ComData.cmd);
			}
			break;						
			
			case 0xED://换能器归原点
			{
				  MotorReset();	
 				  UART3SendData(g_ComData.cmd);
			}
			break;						
			
			case 0xEE://启动电机
			{
			    g_MotorParam.action_flag=1;
				UART3SendData(g_ComData.cmd);
				UART3SendData(g_MotorParam.action_flag);
			}
			break;
			
			case 0xEF://停止电机
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






