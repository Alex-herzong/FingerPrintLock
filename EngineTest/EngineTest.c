#include <reg52.h>
#include <stdio.h> 
unsigned char count;              //0.5ms次数标识
sbit pwm =P1^0;                  //PWM信号输出,连舵机黄线
unsigned char jd;                 //角度标识

void delayms(unsigned int i)      //延时
{
    unsigned int j,k;
    for(j=i; j>0; j--)
        for(k=110; k>0; k--);
}

void Time0_Init()                 //定时器0初始化
{  
    TH0  = (65536-440)/256;		  //1.0851us一个计数周期
    TL0  = (65536-440)%256;		  //初始化中断器
    TMOD = 0x01;                  //定时器0工作在方式1
    EA = 1;
	ET0 = 1;
	TR0 = 1;
	PT0=1;
}

void Time0_Int() interrupt 1      //中断程序
{
    TH0  = (65536-440)/256;		  //1.0851us一个计数周期
    TL0  = (65536-440)%256;		  //重新初始化中断器
    if(count< jd)
    {
        pwm=0;                    //高电平次数小于目标角度，PWM输出高电平（一次高电平输出0.5ms）
	    count=(count+1);          //总次数加1
        count=count%40;           //确保总次数始终保持低于40 即保持周期为20ms
    } else {
        pwm=1;                    //高电平次数大于等于目标角度，PWM输出低电平
    	count=(count+1);          //总次数加1
        count=count%40;           //确保总次数始终保持低于40 即保持周期为20ms
    }

}

void main(){
  
  count=0;
  jd = 3;
  Time0_Init();
  while(1);
  
  /*
  while(1){
  pwm = 1;
  delayms(10);
  pwm = 0;
  delayms(10);
  }
  */
  }
