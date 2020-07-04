#include <reg52.h>
#include <stdio.h> 
unsigned char count;              //0.5ms������ʶ
sbit pwm =P1^0;                  //PWM�ź����,���������
unsigned char jd;                 //�Ƕȱ�ʶ

void delayms(unsigned int i)      //��ʱ
{
    unsigned int j,k;
    for(j=i; j>0; j--)
        for(k=110; k>0; k--);
}

void Time0_Init()                 //��ʱ��0��ʼ��
{  
    TH0  = (65536-440)/256;		  //1.0851usһ����������
    TL0  = (65536-440)%256;		  //��ʼ���ж���
    TMOD = 0x01;                  //��ʱ��0�����ڷ�ʽ1
    EA = 1;
	ET0 = 1;
	TR0 = 1;
	PT0=1;
}

void Time0_Int() interrupt 1      //�жϳ���
{
    TH0  = (65536-440)/256;		  //1.0851usһ����������
    TL0  = (65536-440)%256;		  //���³�ʼ���ж���
    if(count< jd)
    {
        pwm=0;                    //�ߵ�ƽ����С��Ŀ��Ƕȣ�PWM����ߵ�ƽ��һ�θߵ�ƽ���0.5ms��
	    count=(count+1);          //�ܴ�����1
        count=count%40;           //ȷ���ܴ���ʼ�ձ��ֵ���40 ����������Ϊ20ms
    } else {
        pwm=1;                    //�ߵ�ƽ�������ڵ���Ŀ��Ƕȣ�PWM����͵�ƽ
    	count=(count+1);          //�ܴ�����1
        count=count%40;           //ȷ���ܴ���ʼ�ձ��ֵ���40 ����������Ϊ20ms
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
