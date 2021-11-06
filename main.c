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
//接收端
//从发送端接收8个字符，还原成电压、电流
//引脚映射：
//UART3,TX->PC7,RX->PC6
//PWM引脚->PC4,PC5
//**********************************************************
int main(void)
{
	//浮点运算单元
    FPULazyStackingEnable();
    FPUEnable();

    //设置系统时钟
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);//40MHz
	UART3Init();//蓝牙串口初始化
    PWMInit();//PWM输出初始化
    PIDInit();//PID参数初始化

    while(1)
    {
    	if(flag_pid_check==1)//20ms
    	{
    		flag_pid_check=0;//标志位清除
    		Check();//过电流检查
    		PID();//PID调节
    		PWM_Output();//PWM输出
    	}
    }
}

