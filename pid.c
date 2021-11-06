//****************************************************
// pid.c
// Created on: 2020年11月19日
// Author: QLF
//****************************************************
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_timer.h"
#include "inc/hw_sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/fpu.h"

#include "uartreceive.h"
#include "pwm_output.h"
#include "pid.h"

PID_IncTypeDef pid;
float d=0;
int flag_pid_check=0;//PID和过电流检查标志位

/************************************************
函数名称 ：PID_Inc
功    能 ： PID增量(Increment)计算
参    数 ： SetValue ------ 设置值(期望值)
         ActualValue --- 实际值(反馈值)
         PID ----------- PID数据结构
返 回 值 ：PIDInc -------- 本次PID增量(+/-)
*************************************************/
float PID_Inc(float SetValue, float ActualValue, PID_IncTypeDef *PID)//PID增量计算
{
  float PIDInc;                                  //增量
  PID->Ek = SetValue - ActualValue;              //当前误差
  PIDInc = (PID->Kp * PID->Ek) - (PID->Ki * PID->Ek1) + (PID->Kd * PID->Ek2);
  PID->Ek2 = PID->Ek1;
  PID->Ek1 = PID->Ek;
  return PIDInc;
}
void PIDInit(void)//PID参数初始化
{
	pid.Kp=0.004;           //Kp
	pid.Ki=0.000000;          //Ki
	pid.Kd=0;                //Kd
	pid.Ek=0;                //当前误差
	pid.Ek1=0;               //上一次的误差
	pid.Ek2=0;               //上上次误差
	Timer0PeriodInit();//定时器0初始化
}
void PID(void)
{
	d=D;
	d+=PID_Inc(Us,Uo,&pid)/10;//调用PID增量计算函数，PID增量叠加到输入D
	if(d<1&&d>0)//占空比0~1范围内
	{
		D=d;
	}
}
void Timer0PeriodInit(void)//定时器0初始化，周期模式20ms
{
	uint32_t ui32Period;
	ui32Period=SysCtlClockGet()/50;//周期20ms
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);//全字节（32位）周期定时模式,全字节只能用TimerA
	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period-1);//设定重装载值
	TimerEnable(TIMER0_BASE, TIMER_A);//使能定时器
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);//使能超时中断（改参数可以设置为匹配中断）
	TimerIntRegister(TIMER0_BASE,TIMER_A,&Timer0IntHandler);//注册中断处理程序
}
void Timer0IntHandler(void)//周期模式中断函数
{
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	flag_pid_check=1;
}

