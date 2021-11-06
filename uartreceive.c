//****************************************************
// uartreceive.c
// Created on: 2020年11月19日
// Author: QLF
//****************************************************
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "inc/hw_nvic.h"
#include "inc/hw_timer.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/ssi.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/uart.h"

#include "uartreceive.h"
#include "pwm_output.h"
#include "pid.h"

float Uo=0;//电压
float Io=0;//电流
float Us=0;//电压
long Uo_receive=0;//接收的10倍电压，整型
long Us_receive=0;//接收的10倍电压，整型
long Io_receive=0;//接收的10倍电流，整型
char ThisChar;
int flag_receive_erro=0;//接受错误标志位

void UART3IntHandler(void)//UART3中断函数
{
    uint32_t ui32Status;
    ui32Status=UARTIntStatus(UART3_BASE, true); //获取中断状态
    UARTIntClear(UART3_BASE,ui32Status); //清除中断
    if(flag_Io_over==0)
    {
        Receive();//接收电压电流
    }
}

void Receive(void)//接收电压电流
{
	int i;
    Uo_receive=0;//接收的10倍电压，整型
    Us_receive=0;//接收的10倍电压，整型
    Io_receive=0;//接收的10倍电流，整型
    if(UARTCharGet(UART3_BASE)=='a')
    {
    	flag_receive_erro=0;
    	for(i=0;i<4;i++)
    	{
    		ThisChar=UARTCharGet(UART3_BASE);
    		Us_receive=Us_receive<<8;
    		Us_receive|=ThisChar;
    	}
    }
    else
    {
    	flag_receive_erro=1;
    }
    if(UARTCharGet(UART3_BASE)=='b')
    {
    	flag_receive_erro=0;
    	for(i=0;i<4;i++)
    	{
    		ThisChar=UARTCharGet(UART3_BASE);
    		Uo_receive=Uo_receive<<8;
        	Uo_receive|=ThisChar;
    	}
    }
    else
    {
    	flag_receive_erro=1;
    }
    if(UARTCharGet(UART3_BASE)=='c')
    {
    	flag_receive_erro=0;
    	for(i=0;i<4;i++)
    	{
    		ThisChar=UARTCharGet(UART3_BASE);
    		Io_receive=Io_receive<<8;
    		Io_receive|=ThisChar;
    	}
    }
    else
    {
    	flag_receive_erro=1;
    }
    if(Us_receive<1100&&Us_receive>0&&Uo_receive<1100&&Uo_receive>0&&flag_receive_erro==0)
    {
        Us=Us_receive/100.0;//将整型数转换回浮点数，实现了保留一位小数
        Uo=Uo_receive/100.0;//将整型数转换回浮点数，实现了保留一位小数
        Io=Io_receive/100.0;//将整型数转换回浮点数，实现了保留一位小数
    }
}

void UART3Init(void)//UART3配置
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3); //使能UART3外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);//使能GPIOC
	GPIOPinConfigure(GPIO_PC6_U3RX); //GPIO引脚配置
	GPIOPinConfigure(GPIO_PC7_U3TX); //GPIO引脚配置
	GPIOPinTypeUART(GPIO_PORTC_BASE,GPIO_PIN_6|GPIO_PIN_7);//引脚复用为UART外设
	UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(),9600,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE)); //配置 UART参数（时钟、波特率、数据位、停止位、奇偶校验位）
    IntEnable(INT_UART3);
    UARTIntEnable(UART3_BASE, UART_INT_RX);
    UARTIntRegister(UART3_BASE,&UART3IntHandler);
}






