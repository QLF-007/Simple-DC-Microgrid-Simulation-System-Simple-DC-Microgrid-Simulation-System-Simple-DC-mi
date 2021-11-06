//****************************************************
// uartreceive.h
// Created on: 2020��11��19��
// Author: QLF
//****************************************************
#ifndef UARTRECEIVE_H_
#define UARTRECEIVE_H_

extern float Uo;//ʵ���ѹ
extern float Io;//ʵ�����
extern float Us;//�趨��ѹ
extern long Uo_receive;//���յ�10��ʵ���ѹ������
extern long Us_receive;//���յ�10���趨��ѹ������
extern long Io_receive;//���յ�10��ʵ�����������
extern char ThisChar;
extern int flag_receive_erro;//���մ����־λ

extern void UART3Init(void);//UART3��ʼ������
extern void UART3IntHandler(void);//UART3�жϺ���
extern void Receive(void);//���յ�ѹ����

#endif /* UARTRECEIVE_H_ */
