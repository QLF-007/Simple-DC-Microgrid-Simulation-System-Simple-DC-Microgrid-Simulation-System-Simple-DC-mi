//****************************************************
// pid.h
// Created on: 2020��11��19��
// Author: QLF
//****************************************************
#ifndef PID_H_
#define PID_H_

extern float d;
extern int flag_pid_check;//PID�͹���������־λ

typedef struct
{
  float Kp;                       //����ϵ��Proportional
  float Ki;                       //����ϵ��Integral
  float Kd;                       //΢��ϵ��Derivative

  float Ek;                       //��ǰ���
  float Ek1;                      //ǰһ����� e(k-1)
  float Ek2;                      //��ǰһ����� e(k-2)
}PID_IncTypeDef;

extern float PID_Inc(float SetValue, float ActualValue, PID_IncTypeDef *PID);
extern void PID(void);
extern void PIDInit(void);
extern void Timer0PeriodInit(void);
extern void Timer0IntHandler(void);//����ģʽ�жϺ���
#endif /* PID_H_ */
