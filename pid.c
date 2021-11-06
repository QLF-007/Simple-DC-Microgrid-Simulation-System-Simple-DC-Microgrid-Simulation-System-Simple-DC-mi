//****************************************************
// pid.c
// Created on: 2020��11��19��
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
int flag_pid_check=0;//PID�͹���������־λ

/************************************************
�������� ��PID_Inc
��    �� �� PID����(Increment)����
��    �� �� SetValue ------ ����ֵ(����ֵ)
         ActualValue --- ʵ��ֵ(����ֵ)
         PID ----------- PID���ݽṹ
�� �� ֵ ��PIDInc -------- ����PID����(+/-)
*************************************************/
float PID_Inc(float SetValue, float ActualValue, PID_IncTypeDef *PID)//PID��������
{
  float PIDInc;                                  //����
  PID->Ek = SetValue - ActualValue;              //��ǰ���
  PIDInc = (PID->Kp * PID->Ek) - (PID->Ki * PID->Ek1) + (PID->Kd * PID->Ek2);
  PID->Ek2 = PID->Ek1;
  PID->Ek1 = PID->Ek;
  return PIDInc;
}
void PIDInit(void)//PID������ʼ��
{
	pid.Kp=0.004;           //Kp
	pid.Ki=0.000000;          //Ki
	pid.Kd=0;                //Kd
	pid.Ek=0;                //��ǰ���
	pid.Ek1=0;               //��һ�ε����
	pid.Ek2=0;               //���ϴ����
	Timer0PeriodInit();//��ʱ��0��ʼ��
}
void PID(void)
{
	d=D;
	d+=PID_Inc(Us,Uo,&pid)/10;//����PID�������㺯����PID�������ӵ�����D
	if(d<1&&d>0)//ռ�ձ�0~1��Χ��
	{
		D=d;
	}
}
void Timer0PeriodInit(void)//��ʱ��0��ʼ��������ģʽ20ms
{
	uint32_t ui32Period;
	ui32Period=SysCtlClockGet()/50;//����20ms
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);//ȫ�ֽڣ�32λ�����ڶ�ʱģʽ,ȫ�ֽ�ֻ����TimerA
	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period-1);//�趨��װ��ֵ
	TimerEnable(TIMER0_BASE, TIMER_A);//ʹ�ܶ�ʱ��
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);//ʹ�ܳ�ʱ�жϣ��Ĳ�����������Ϊƥ���жϣ�
	TimerIntRegister(TIMER0_BASE,TIMER_A,&Timer0IntHandler);//ע���жϴ������
}
void Timer0IntHandler(void)//����ģʽ�жϺ���
{
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	flag_pid_check=1;
}

