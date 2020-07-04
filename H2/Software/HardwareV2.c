#include <reg52.h>
#include <stdio.h>
#include <string.h>
#include "AS608.h"
unsigned char count;              //0.5ms������ʶ
sbit pwm =P1^1 ;                  //PWM�ź����,���������
sbit Relay =P1^0 ;
sbit sw = P2^0;
unsigned char jd;                 //�Ƕȱ�ʶ
uchar xdata Data_buff[MAX];		  //��ʼ���ṹ��
struct as608 As_608_data= {0};

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
    IE = 0x82;	                  //IE=0x82=1000 0010 �ȼ��� EA=1 �����ж�  ET0=1 ����ʱ��0�ж�
    TR0=1;                        //����ʱ��0
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

void UART_Init(void)              //�����������Ϊ57600��12T  ����11.0592MHz
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
//��������Send_Bytes
//���ܣ����Ͷ���ֽ�
//������*c:�׵�ַ    len������
//ָ����룺��
//����ֵ��  ��
//*******************************************
void Send_Bytes(uchar *c,uchar len)
{
    uchar i=0;
    for(i=0; i<len; i++)
    {
        SBUF = *(c+i);
        while(!TI);
        TI = 0;
    }
}

//*******************************************
//��������Receive_Bytes
//���ܣ����ն���ֽ�
//������*c:�׵�ַ    len������
//ָ����룺��
//����ֵ��  ��
//*******************************************
char Receive_Bytes(uchar *c,uchar len)
{
    uchar i=0;
    unsigned long time=33000;
    for(i=0; i<len; i++)
    {
        while(!RI && time--)
        {
            if(time==0)
                return MI_NOTAGERR;
        }
        *(c+i)=SBUF;
        RI=0;
    }
    return MI_OK;
}

//*******************************************
//��������PS_GetImage
//���ܣ��Ӵ������϶���ͼ�����ͼ�񻺳���
//��������
//ָ����룺01H
//����ֵ����
//*******************************************
char PS_GetImage()
{

    uchar *ps2=     "\xef\x01\xff\xff\xff\xff\x01\x00\x03\x01\x00\x05";	 //ָ����

    Send_Bytes(ps2,12);		       //����ָ����

    //while(RI==0);

    if(Receive_Bytes(Data_buff,12))   //����Ӧ���
    {
        return MI_ERR;
    }

    if(Data_buff[9]==0x00&&Data_buff[11]==0x0a)		   //�ж�ȷ�����Ƿ�ɹ�	  ����ΪУ����ж�
    {
        return MI_OK;
    }

    return MI_ERR;
}

//*******************************************
//��������PS_GenChar
//���ܣ��� ImageBuffer(ͼ�񻺳���) �е�ԭʼͼ������ָ�������ļ����� CharBuffer1 �� CharBuffer2
//������BufferID(������������)
//ָ����룺02H
//����ֵ����
//*******************************************
char PS_GenChar(uchar BufferID)
{

    uchar *ps1=     "\xef\x01\xff\xff\xff\xff\x01\x00\x04\x02\x01\x00\x08";	 //���CharBuffer1��ָ����
    uchar *ps2=     "\xef\x01\xff\xff\xff\xff\x01\x00\x04\x02\x02\x00\x09";	 //���CharBuffer1��ָ����
    if(BufferID==0x01)
    {
        Send_Bytes(ps1,13);		       //����ָ����
    }
    else
    {
        Send_Bytes(ps2,13);		       //����ָ����
    }

    //while(RI==0);

    if(Receive_Bytes(Data_buff,12))   //����Ӧ���
    {
        return MI_ERR;
    }

    if(Data_buff[9]==0x00&&Data_buff[11]==0x0a)		   //�ж�ȷ�����Ƿ�ɹ�	  ����ΪУ����ж�
    {
        return MI_OK;
    }

    return MI_ERR;
}

//*******************************************
//��������PS_Search
//���ܣ��� CharBuffer1 �� CharBuffer2 �е������ļ����������򲿷�ָ�ƿ⡣�����������򷵻�ҳ��
//������BufferID�� (StartPage(��ʼҳ)��PageNum��ҳ����)(Ĭ�ϴ�0��300ȫ������)
//ָ����룺04H
//����ֵ��ȷ���롢ҳ�루ID��
//*******************************************
char PS_Search(uchar BufferID)
{

    uchar *ps1=     "\xef\x01\xff\xff\xff\xff\x01\x00\x08\x04\x01\x00\x00\x01\x2b\x00\x3a";	 //���CharBuffer1��ָ����
    uchar *ps2=     "\xef\x01\xff\xff\xff\xff\x01\x00\x08\x04\x02\x00\x00\x01\x2b\x00\x3b";


    if(BufferID==0x01)
    {
        Send_Bytes(ps1,17);		       //����ָ����
    }
    else
    {
        Send_Bytes(ps2,17);		       //����ָ����
    }

    //while(RI==0);

    if(Receive_Bytes(Data_buff,16))   //����Ӧ���
    {
        return MI_ERR;
    }


    if(Data_buff[9]==0x00)		   //�ж�ȷ�����Ƿ�ɹ�
    {
        return MI_OK;
    }

    return MI_ERR;
}

void unlock()
{
    delayms(100);
    Time0_Init();
    count=0;
    jd = 5;
    delayms(3000);
    Time0_Init();
    count=0;
    jd = 2;
    delayms(1000);
    Relay = 0;
}
void main()
{
    char status;
    Relay = 1;
    sw = 1;
    status = sw;
    while (!status)
    {
        delayms(3000);
        if(sw == 0)
            Relay = 0;
    }
    RI=0;
    while(1) {
        UART_Init();
        if(PS_GetImage()==MI_OK) {
            if(PS_GenChar(0x01)==MI_OK) {
                if(PS_Search(0x01)==MI_OK) {
                    unlock();
                } else {
                    while(1)
                    {
                        Relay = 0;
                        delayms(250);
                        Relay = 1;
                        delayms(250);
                    }
                }
            } else
                Relay = 0;
        } else
            Relay = 0;
    }
}
