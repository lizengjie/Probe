
#ifndef __GPIO_H__
#define	__GPIO_H__

#include <stdint.h>
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"

extern uint8_t EINT_Flag;

enum GPIO_DIR
{
	GPIO_INTPUT,
	GPIO_OUTPUT
};

/*IO操作函数*/
void SetGpioDirect(enum GPIO_DIR Direct, uint8_t GrpNum, uint8_t PinNum);
void SetGpioDirectDown(enum GPIO_DIR Direct, uint8_t GrpNum, uint8_t PinNum);
void SetPinValue(uint8_t GrpNum, uint8_t PinNum, uint8_t Value);
int  GetAllPinValue(uint8_t GrpNum);
int  GetPinValue(uint8_t GrpNum, uint8_t PinNum);

/*IO应用初始化*/
void GPIOInit(void);

void SenseIntEnable(uint8_t edgeState);
void SenseIntDisable(void);

#endif /**End File**/
