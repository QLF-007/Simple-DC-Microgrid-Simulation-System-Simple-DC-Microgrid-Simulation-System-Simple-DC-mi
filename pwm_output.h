//****************************************************
// pwm_output.h
// Created on: 2020��11��20��
// Author: QLF
//****************************************************
#ifndef PWM_OUTPUT_H_
#define PWM_OUTPUT_H_

#define FREQUENCY 30000//PWMƵ��
#define DEADBAND 2500000//����ʱ��ĵ���

extern float D;//PWMռ�ձ�,0~1
extern int flag_Io_over;//��������־λ

extern void PWMInit(void);//����������PWM��ʼ��
extern void PWM_Output(void);//PWM���
extern void Check(void);//���������

#endif /* PWM_OUTPUT_H_ */
