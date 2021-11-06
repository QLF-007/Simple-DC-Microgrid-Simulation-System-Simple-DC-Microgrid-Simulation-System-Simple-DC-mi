//****************************************************
// pid.h
// Created on: 2020年11月19日
// Author: QLF
//****************************************************
#ifndef PID_H_
#define PID_H_

extern float d;
extern int flag_pid_check;//PID和过电流检查标志位

typedef struct
{
  float Kp;                       //比例系数Proportional
  float Ki;                       //积分系数Integral
  float Kd;                       //微分系数Derivative

  float Ek;                       //当前误差
  float Ek1;                      //前一次误差 e(k-1)
  float Ek2;                      //再前一次误差 e(k-2)
}PID_IncTypeDef;

extern float PID_Inc(float SetValue, float ActualValue, PID_IncTypeDef *PID);
extern void PID(void);
extern void PIDInit(void);
extern void Timer0PeriodInit(void);
extern void Timer0IntHandler(void);//周期模式中断函数
#endif /* PID_H_ */
