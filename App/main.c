
#include "LPC17xx.h"
#include "lpc17xx_libcfg.h"
#include "gpio.h"
#include "pwm.h"
#include "uart.h"
#include "dispose.h"
#include "motor.h"
#include "timer.h"


int main(void)
{
   SystemInit();      //cpu时钟100Mhz，需要的外设可用
   GPIOInit();		  //初始化IO口
   PWMInit();         //初始化pwm
   UartInit();        //初始化串口
   TimerInit();		  //初始化定时器
   MotorParamReset(); //恢复马达设置
   MotorReset();      //复位马达，归原点

   while(1)
   {
		Dispose();
		MotorAction();
   }
}































/**以下部分请勿删**/

#ifdef  DEBUG
/*******************************************************************************
* @brief		Reports the name of the source file and the source line number
* 				where the CHECK_PARAM error has occurred.
* @param[in]	file Pointer to the source file name
* @param[in]    line assert_param error line source number
* @return		None
*******************************************************************************/
void check_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while(1);
}
#endif

/*
 * @}
 */


