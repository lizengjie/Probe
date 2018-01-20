#ifndef _MOTOR_H_
#define _MOTOR_H_

typedef struct {
	
		volatile uint8_t speed;
		volatile uint8_t distance;
		volatile uint8_t mode;
		volatile uint8_t action_flag;
		volatile uint8_t location;	
}MotorParam;

extern MotorParam g_MotorParam;

void MotorParamReset(void);
void MotorReset(void);
void MotorAction(void);
void P_Swing(uint32_t time);
void N_Swing(uint32_t time);
void PN_Swing(uint32_t time);
void MonitorFault(void);

#endif

