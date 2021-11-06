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
#include "driverlib/fpu.h"
#include "driverlib/uart.h"

#include "uartreceive.h"
#include "pwm_output.h"
#include "pid.h"

//**********************************************************
//���ն�
//�ӷ��Ͷ˽���8���ַ�����ԭ�ɵ�ѹ������
//����ӳ�䣺
//UART3,TX->PC7,RX->PC6
//PWM����->PC4,PC5
//**********************************************************
int main(void)
{
	//�������㵥Ԫ
    FPULazyStackingEnable();
    FPUEnable();

    //����ϵͳʱ��
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);//40MHz
	UART3Init();//�������ڳ�ʼ��
    PWMInit();//PWM�����ʼ��
    PIDInit();//PID������ʼ��

    while(1)
    {
    	if(flag_pid_check==1)//20ms
    	{
    		flag_pid_check=0;//��־λ���
    		Check();//���������
    		PID();//PID����
    		PWM_Output();//PWM���
    	}
    }
}

