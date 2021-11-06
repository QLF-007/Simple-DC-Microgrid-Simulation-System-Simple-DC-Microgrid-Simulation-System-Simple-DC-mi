//****************************************************
// pwm_output.c
// Created on: 2020年11月20日
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

float D=0;//PWM占空比,0~1
int flag_Io_over=0;//过电流标志位

//****************************
//PWM引脚->PC4,PC5
//****************************
void PWM_Output(void)//PWM输出
{
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, SysCtlClockGet()/FREQUENCY*(1-D));//占空比D
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, SysCtlClockGet()/FREQUENCY*(1-D));
	PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);
	PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);
}
void PWMInit(void)//PWM初始化
{
	SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	// M0PWM7引脚复用设置
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	GPIOPinConfigure(GPIO_PC5_M0PWM7);
	GPIOPinConfigure(GPIO_PC4_M0PWM6);
	GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_5);
	GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_4);

	PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN |PWM_GEN_MODE_NO_SYNC);
	// 配置PWM周期和宽度，使能输出
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, SysCtlClockGet()/FREQUENCY);//PWM频率400Hz

	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, SysCtlClockGet()/FREQUENCY*(1-D));//占空比D
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, SysCtlClockGet()/FREQUENCY*(1-D));
	PWMDeadBandEnable(PWM0_BASE, PWM_GEN_3, SysCtlClockGet()/DEADBAND,SysCtlClockGet()/DEADBAND);//死区时间100us

	PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);
	PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);

	// 使能PWM0发生器3
	PWMGenEnable(PWM0_BASE, PWM_GEN_3);
}

void Check(void)//过电流检查
{
	if(Io>2)//判断是否过电流
	{
		D=0;
		Io=0;
		Uo=0;
		PWM_Output();//调节占空比为零，关断PWM输出
		flag_Io_over=1;
		SysCtlDelay(SysCtlClockGet()/3);//等待1s
		flag_Io_over=0;
	}
}

