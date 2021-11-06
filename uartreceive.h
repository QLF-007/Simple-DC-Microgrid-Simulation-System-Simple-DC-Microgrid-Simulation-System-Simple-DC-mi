//****************************************************
// uartreceive.h
// Created on: 2020年11月19日
// Author: QLF
//****************************************************
#ifndef UARTRECEIVE_H_
#define UARTRECEIVE_H_

extern float Uo;//实测电压
extern float Io;//实测电流
extern float Us;//设定电压
extern long Uo_receive;//接收的10倍实测电压，整型
extern long Us_receive;//接收的10倍设定电压，整型
extern long Io_receive;//接收的10倍实测电流，整型
extern char ThisChar;
extern int flag_receive_erro;//接收错误标志位

extern void UART3Init(void);//UART3初始化函数
extern void UART3IntHandler(void);//UART3中断函数
extern void Receive(void);//接收电压电流

#endif /* UARTRECEIVE_H_ */
