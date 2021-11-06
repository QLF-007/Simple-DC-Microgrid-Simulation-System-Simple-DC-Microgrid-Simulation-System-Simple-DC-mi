//****************************************************
// pwm_output.c
// Created on: 2020��11��20��
// Author: QLF
//****************************************************
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/fpu.h"
#include "driverlib/timer.h"


#include "uartreceive.h"
#include "pwm_output.h"
#include "pid.h"

float D=0;//PWMռ�ձ�,0~1
int flag_Io_over=0;//��������־λ

//****************************
//PWM����->PC4,PC5
//****************************
void PWM_Output(void)//PWM���
{
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, SysCtlClockGet()/FREQUENCY*(1-D));//ռ�ձ�D
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, SysCtlClockGet()/FREQUENCY*(1-D));
	PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);
	PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);
}
void PWMInit(void)//PWM��ʼ��
{
	SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	// M0PWM7���Ÿ�������
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	GPIOPinConfigure(GPIO_PC5_M0PWM7);
	GPIOPinConfigure(GPIO_PC4_M0PWM6);
	GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_5);
	GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_4);

	PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN |PWM_GEN_MODE_NO_SYNC);
	// ����PWM���ںͿ�ȣ�ʹ�����
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, SysCtlClockGet()/FREQUENCY);//PWMƵ��400Hz

	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, SysCtlClockGet()/FREQUENCY*(1-D));//ռ�ձ�D
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, SysCtlClockGet()/FREQUENCY*(1-D));
	PWMDeadBandEnable(PWM0_BASE, PWM_GEN_3, SysCtlClockGet()/DEADBAND,SysCtlClockGet()/DEADBAND);//����ʱ��100us

	PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);
	PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);

	// ʹ��PWM0������3
	PWMGenEnable(PWM0_BASE, PWM_GEN_3);
}

void Check(void)//���������
{
	if(Io>2)//�ж��Ƿ������
	{
		D=0;
		Io=0;
		Uo=0;
		PWM_Output();//����ռ�ձ�Ϊ�㣬�ض�PWM���
		flag_Io_over=1;
		SysCtlDelay(SysCtlClockGet()/3);//�ȴ�1s
		flag_Io_over=0;
	}
}

