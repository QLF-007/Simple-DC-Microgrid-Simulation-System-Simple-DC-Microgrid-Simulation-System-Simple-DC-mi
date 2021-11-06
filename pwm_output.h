//****************************************************
// pwm_output.h
// Created on: 2020年11月20日
// Author: QLF
//****************************************************
#ifndef PWM_OUTPUT_H_
#define PWM_OUTPUT_H_

#define FREQUENCY 30000//PWM频率
#define DEADBAND 2500000//死区时间的倒数

extern float D;//PWM占空比,0~1
extern int flag_Io_over;//过电流标志位

extern void PWMInit(void);//互补带死区PWM初始化
extern void PWM_Output(void);//PWM输出
extern void Check(void);//过电流检查

#endif /* PWM_OUTPUT_H_ */
