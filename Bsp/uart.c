
#include "uart.h"                 

volatile int8_t  FifoHead = 0;
volatile int8_t  FifoTail = 0;
volatile uint8_t FifoBuff[FIFO_SIZE];

/*
*功  能：配置uart模块,打开串口通信功能
*/
void UartInit(void)
{
    /*uart3*/	
	PINSEL_CFG_Type 	 PinCfg_GPIO;		
  	UART_CFG_Type 	UARTConfigStruct;		 
	
	PinCfg_GPIO.Portnum 	= 0;
	PinCfg_GPIO.Funcnum		= 2;
	PinCfg_GPIO.Pinmode		= PINSEL_PINMODE_PULLUP;
	PinCfg_GPIO.OpenDrain	= PINSEL_PINMODE_NORMAL;
	PinCfg_GPIO.Pinnum		= 0; //TXD3
	PINSEL_ConfigPin(&PinCfg_GPIO);
	PinCfg_GPIO.Pinnum		= 1; //RXD3
	PINSEL_ConfigPin(&PinCfg_GPIO);

	UARTConfigStruct.Baud_rate = 115200;						    /* Baud rate = 115200  */
	UARTConfigStruct.Databits = UART_DATABIT_8;						/* 8 bit */
	UARTConfigStruct.Parity = UART_PARITY_NONE;						/* EVEN PARITY */
	UARTConfigStruct.Stopbits = UART_STOPBIT_1;						/* 1 stop bit */
	UART_Init(LPC_UART3, &UARTConfigStruct);
	UART_TxCmd(LPC_UART3, ENABLE);									/* Enable transmission on UART TxD pin */
	
	UART_IntConfig(LPC_UART3, UART_INTCFG_RBR, ENABLE);
	NVIC_EnableIRQ(UART3_IRQn);
	NVIC_SetPriority(UART3_IRQn,1);
	
    /*uart1*/
	PinCfg_GPIO.Portnum 	= 2;
	PinCfg_GPIO.Funcnum		= 2;
	PinCfg_GPIO.Pinmode		= PINSEL_PINMODE_PULLUP;
	PinCfg_GPIO.OpenDrain	= PINSEL_PINMODE_NORMAL;
	PinCfg_GPIO.Pinnum		= 0; //TXD1
	PINSEL_ConfigPin(&PinCfg_GPIO);
	PinCfg_GPIO.Pinnum		= 1; //RXD1
	PINSEL_ConfigPin(&PinCfg_GPIO);

	UARTConfigStruct.Baud_rate = 115200;						    /* Baud rate = 115200  */
	UARTConfigStruct.Databits = UART_DATABIT_8;						/* 8 bit */
	UARTConfigStruct.Parity = UART_PARITY_NONE;						/* EVEN PARITY */
	UARTConfigStruct.Stopbits = UART_STOPBIT_1;						/* 1 stop bit */
  	UART_Init((LPC_UART_TypeDef*)LPC_UART1, &UARTConfigStruct);
	UART_TxCmd((LPC_UART_TypeDef*)LPC_UART1, ENABLE);			    /* Enable transmission on UART TxD pin */
	
//	UART_IntConfig((LPC_UART_TypeDef*)LPC_UART1, UART_INTCFG_RBR, ENABLE);	/*uart1接收功能不用*/
//	NVIC_EnableIRQ(UART1_IRQn);
//	NVIC_SetPriority(UART1_IRQn,3);

	return;
}

/*
功  能：串口3接收中断处理
*/
void UART3_IRQHandler(void)
{
	uint8_t IIRValue;
	uint8_t Err = Err;
	uint8_t RecByte;

	IIRValue = LPC_UART3->IIR;
 	IIRValue >>= 1;	  //跳过中断标志IIR中断挂起位
  	IIRValue &= 0x07; //与上0x07，得到中断标志

	switch(IIRValue)
 	{
		case	IIR_RLS://线状态中断
				break;

		case	IIR_RDA://接收数据有效中断标志位
				RecByte = LPC_UART3->RBR;//读取一个有效数据到接收缓冲区
		 		FifoWriteOneByte(RecByte);//将收到字节写入fifobuf
				break;

		case	IIR_CTI://字符超时指示中断标志位	
				break;
				
		case	IIR_THRE://发送保持寄存器空标志位
				break;

		default: 
				break;
  	}

	return;
}

/*
功  能：串口3输出字节
*/
void UART3SendData(uint8_t byte) 
{
    LPC_UART3->THR = byte;	
		while((LPC_UART3->LSR & 0x40)==0); 

	return;
}

/*
功  能：串口1输出字节
*/
void UART1SendData(uint8_t byte) 
{
    LPC_UART1->THR = byte; 	
		while((LPC_UART1->LSR & 0x40)==0); 
}

/*
功  能：将接收的字节数据排队，存入缓冲区
*/
int32_t FifoWriteOneByte(uint8_t InputChar)
{
	if((FifoHead + 1)%FIFO_SIZE == FifoTail)
	{
		return -1;
	}
	FifoBuff[FifoHead] = InputChar;
	FifoHead = (FifoHead + 1)%FIFO_SIZE;
	
	return 0;
}

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

