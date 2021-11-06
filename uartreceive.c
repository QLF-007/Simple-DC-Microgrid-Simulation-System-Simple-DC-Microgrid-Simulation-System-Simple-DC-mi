//****************************************************
// uartreceive.c
// Created on: 2020��11��19��
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

float Uo=0;//��ѹ
float Io=0;//����
float Us=0;//��ѹ
long Uo_receive=0;//���յ�10����ѹ������
long Us_receive=0;//���յ�10����ѹ������
long Io_receive=0;//���յ�10������������
char ThisChar;
int flag_receive_erro=0;//���ܴ����־λ

void UART3IntHandler(void)//UART3�жϺ���
{
    uint32_t ui32Status;
    ui32Status=UARTIntStatus(UART3_BASE, true); //��ȡ�ж�״̬
    UARTIntClear(UART3_BASE,ui32Status); //����ж�
    if(flag_Io_over==0)
    {
        Receive();//���յ�ѹ����
    }
}

void Receive(void)//���յ�ѹ����
{
	int i;
    Uo_receive=0;//���յ�10����ѹ������
    Us_receive=0;//���յ�10����ѹ������
    Io_receive=0;//���յ�10������������
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
        Us=Us_receive/100.0;//��������ת���ظ�������ʵ���˱���һλС��
        Uo=Uo_receive/100.0;//��������ת���ظ�������ʵ���˱���һλС��
        Io=Io_receive/100.0;//��������ת���ظ�������ʵ���˱���һλС��
    }
}

void UART3Init(void)//UART3����
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3); //ʹ��UART3����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);//ʹ��GPIOC
	GPIOPinConfigure(GPIO_PC6_U3RX); //GPIO��������
	GPIOPinConfigure(GPIO_PC7_U3TX); //GPIO��������
	GPIOPinTypeUART(GPIO_PORTC_BASE,GPIO_PIN_6|GPIO_PIN_7);//���Ÿ���ΪUART����
	UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(),9600,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE)); //���� UART������ʱ�ӡ������ʡ�����λ��ֹͣλ����żУ��λ��
    IntEnable(INT_UART3);
    UARTIntEnable(UART3_BASE, UART_INT_RX);
    UARTIntRegister(UART3_BASE,&UART3IntHandler);
}






