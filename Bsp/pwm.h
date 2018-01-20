#ifndef _PWM_H_
#define _PWM_H_

#include "lpc17xx_pinsel.h"
#include "lpc17xx_pwm.h"

extern float Frequence[21];
extern uint8_t pwm_flag;

void PWMInit(void);
void SetPMWFreq(uint8_t level);
void StopPWM(void);
void StartPWM(void);


#endif

