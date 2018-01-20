#ifndef _TIMER_H_
#define _TIMER_H_

#include "lpc17xx_timer.h"

extern volatile uint32_t timer_value;

void TimerInit(void);
void StartTimer(void);
void StopTimer(void);
void ResetTimer(void);

#endif

