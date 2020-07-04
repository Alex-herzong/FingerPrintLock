
#include <reg52.h>
#include <stdio.h> 
#include <string.h>
#include "AS608.h"
unsigned char count;              //0.5ms次数标识
sbit pwm =P1^0 ;                  //PWM信号输出,连舵机黄线
sbit Relay =P1^1 ;
sbit check = P2^0;
unsigned char jd;                 //角度标识
uchar xdata Data_buff[MAX];		  //初始化结构体
struct as608 As_608_data={0};
int judge=1;
char locked = 0;

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
    IE = 0x82;	                  //IE=0x82=1000 0010 等价于 EA=1 开总中断  ET0=1 开定时器0中断
    TR0=1;                        //开定时器0
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

/*
void SendOneByte(unsigned char c) //通过串口发送一个字节
{
    SBUF = c;
    while(!TI);
    TI = 0;
}
*/

/*
char putchar(char c)			  //putchar 函数重写
{
	SendOneByte(c);
	return c;
}
*/
void UART_Init(void)              //波特率已最高为57600，12T  晶振11.0592MHz
{
	SCON = 0x50;
	TMOD &= 0x0f;
    TMOD = 0x20;
    PCON = 0x80;
    TH1 = 0xFF;
    TL1 = 0xFF; 
	ET1 = 0;
    TR1 = 1;
}

//*******************************************  
//函数名：Send_Bytes
//功能：发送多个字节
//参数：*c:首地址    len：长度
//指令代码：无
//返回值：  无
//******************************************* 
void Send_Bytes(uchar *c,uchar len)
{
	uchar i=0;
	for(i=0;i<len;i++)
	{
    	SBUF = *(c+i);
    	while(!TI);
    	TI = 0;
	}
}

//*******************************************  
//函数名：Receive_Bytes
//功能：接收多个字节
//参数：*c:首地址    len：长度
//指令代码：无
//返回值：  无
//******************************************* 
char Receive_Bytes(uchar *c,uchar len)
{
	uchar i=0;
	uchar time=200;
	for(i=0;i<len;i++)
	{
		while(!RI && time--)
		{
			if(time<=1)
				return MI_NOTAGERR;
		}
    	*(c+i)=SBUF;
		RI=0;
    }	
	return MI_OK;
}

//*******************************************  
//函数名：PS_GetImage
//功能：从传感器上读入图像存于图像缓冲区
//参数：无
//指令代码：01H
//返回值：有
//******************************************* 
char PS_GetImage()
{

	uchar *ps2=     "\xef\x01\xff\xff\xff\xff\x01\x00\x03\x01\x00\x05";	 //指令码

	Send_Bytes(ps2,12);		       //发送指令码

	while(RI==0);

	if(Receive_Bytes(Data_buff,12))   //接收应答包
	{
		return MI_ERR;	
	}		

	if(Data_buff[9]==0x00&&Data_buff[11]==0x0a)		   //判断确认码是否成功	  后面为校验和判断
	{							   
		return MI_OK;	
	}
	
	return MI_ERR;	
}

//*******************************************  
//函数名：PS_GenChar
//功能：将 ImageBuffer(图像缓冲区) 中的原始图像生成指纹特征文件存于 CharBuffer1 或 CharBuffer2
//参数：BufferID(特征缓冲区号)
//指令代码：02H
//返回值：有
//******************************************* 
char PS_GenChar(uchar BufferID)
{

	uchar *ps1=     "\xef\x01\xff\xff\xff\xff\x01\x00\x04\x02\x01\x00\x08";	 //存放CharBuffer1的指令码
	uchar *ps2=     "\xef\x01\xff\xff\xff\xff\x01\x00\x04\x02\x02\x00\x09";	 //存放CharBuffer1的指令码
	if(BufferID==0x01)
	{
		Send_Bytes(ps1,13);		       //发送指令码
	}
	else
	{
		Send_Bytes(ps2,13);		       //发送指令码
	}
	
	while(RI==0);

	if(Receive_Bytes(Data_buff,12))   //接收应答包
	{
		return MI_ERR;	
	}		
	
	if(Data_buff[9]==0x00&&Data_buff[11]==0x0a)		   //判断确认码是否成功	  后面为校验和判断
	{							   
		return MI_OK;	
	}
	
	return MI_ERR;	
}

//*******************************************  
//函数名：PS_Search
//功能：以 CharBuffer1 或 CharBuffer2 中的特征文件搜索整个或部分指纹库。若搜索到，则返回页码
//参数：BufferID， (StartPage(起始页)，PageNum（页数）)(默认从0到300全局搜索)
//指令代码：04H
//返回值：确认码、页码（ID）
//******************************************* 
char PS_Search(uchar BufferID)
{

	uchar *ps1=     "\xef\x01\xff\xff\xff\xff\x01\x00\x08\x04\x01\x00\x00\x01\x2b\x00\x3a";	 //存放CharBuffer1的指令码
	uchar *ps2=     "\xef\x01\xff\xff\xff\xff\x01\x00\x08\x04\x02\x00\x00\x01\x2b\x00\x3b";
	

	if(BufferID==0x01)
	{
		Send_Bytes(ps1,17);		       //发送指令码
	}
	else
	{
		Send_Bytes(ps2,17);		       //发送指令码
	}
	
	while(RI==0);

	if(Receive_Bytes(Data_buff,16))   //接收应答包
	{
		return MI_ERR;	
	}		


	if(Data_buff[9]==0x00)		   //判断确认码是否成功	 
	{							   
		return MI_OK;	
	}
	
	return MI_ERR;	
} 

void lock()
{ 
  if (locked == 0){
  Relay = 1;
  delayms(100);
  Time0_Init();
  count=0;
  jd = 2;
  delayms(500);
  locked = 1;
  Relay = 0;
  delayms(2500);
  }
}

void unlock()
{
  if (locked == 1){   
    Relay = 1;
    delayms(100);
    Time0_Init();
    count=0;
    jd = 5;
    delayms(500);
    locked = 0;
    Relay = 0;
    if (check == 1){		  //门未开时为高电平
	  delayms(3500);
	}else{
	  delayms(2500);
	}
	  
  }		 
}  								 

void main()
{
  check = 1;
  lock();
  unlock();
  lock();
  RI=0;
  while(1){
    char Re;
	UART_Init();
	Re = PS_GetImage();
    if(Re==MI_OK){ 
	  Re = PS_GenChar(0x01);
      if(Re==MI_OK){
        Re = PS_Search(0x01);
        if(Re==MI_OK){
		  unlock();
		  lock();
	    }else{
	      //lock(); 
	    }
      }else{
	    //lock();
	  }
    }else{
	  //lock();
	}
  }
}
