/*
	���ߣ�����NK
	������51����̳
	ʱ�䣺2018��8��3��	

	ps���������������Ǹ����ֲ������ĺ���������С��ѧϰ�����˴��ִ򣬾���û���κγ�Ϯ����ȫ���ɲ飩
*/

#include "AS608.h"


uchar xdata Data_buff[MAX];


void SendOneByte(unsigned char c)
{
    SBUF = c;
    while(!TI);
    TI = 0;
}

/* putchar ������д */
char putchar(char c)
{
	SendOneByte(c);
	return c;
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
	for(i=0;i<len;i++)
	{
    	SBUF = *(c+i);
    	while(!TI);
    	TI = 0;
	}
}

//*******************************************  
//��������Send_Bytes
//���ܣ����ն���ֽ�
//������*c:�׵�ַ    len������
//ָ����룺��
//����ֵ��  ��
//******************************************* 
void Receive_Bytes(uchar *c,uchar len)
{
	uchar i=0;
	for(i=0;i<len;i++)
	{
		while(!RI);
    	*(c+i)=SBUF;
		RI=0;
    }	
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
	Receive_Bytes(Data_buff,12);   //����Ӧ���
	
	if(Data_buff[9]==0x00&&Data_buff[11]==0x0a)		   //�ж�ȷ�����Ƿ�ɹ�	  ����ΪУ����ж�
	{							   
		return MI_OK;	
	}
	
	return (-1);	
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
	
	Receive_Bytes(Data_buff,12);   //����Ӧ���
	
	if(Data_buff[9]==0x00&&Data_buff[11]==0x0a)		   //�ж�ȷ�����Ƿ�ɹ�	  ����ΪУ����ж�
	{							   
		return MI_OK;	
	}
	
	return (-1);	
} 



//*******************************************  
//��������PS_Match
//���ܣ���ȷ�ȶ� CharBuffer1 �� CharBuffer2 �е������ļ�
//������NULL
//ָ����룺03H
//����ֵ����
//******************************************* 
char PS_Match()
{

	uchar *ps1=     "\xef\x01\xff\xff\xff\xff\x01\x00\x03\x03\x00\x07";	 //���CharBuffer1��ָ����

	Send_Bytes(ps1,12);		       //����ָ����

	
	Receive_Bytes(Data_buff,14);   //����Ӧ���
	
	if(Data_buff[9]==0x00)		   //�ж�ȷ�����Ƿ�ɹ�	 
	{							   
		return MI_OK;	
	}
	
	return (-1);	
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
	

		
	Receive_Bytes(Data_buff,16);   //����Ӧ���
	
	write_com(0x8a);
	wstring("Ӧ��ɹ�...");

	if(Data_buff[9]==0x00)		   //�ж�ȷ�����Ƿ�ɹ�	 
	{							   
		return MI_OK;	
	}
	
	return (1);	
} 



//*******************************************  
//��������PS_RegModel
//���ܣ��� CharBuffer1 �� CharBuffer2 �е������ļ��ϲ�����ģ�壬������� CharBuffer1 �� CharBuffer2
//������NULL
//ָ����룺05H
//����ֵ��ȷ����
//******************************************* 
char PS_RegModel()
{

	uchar *ps1=     "\xef\x01\xff\xff\xff\xff\x01\x00\x03\x05\x00\x09";	 //���CharBuffer1��ָ����
	

	Send_Bytes(ps1,12);		       //����ָ����

	
	Receive_Bytes(Data_buff,12);   //����Ӧ���
	
	if(Data_buff[9]==0x00)		   //�ж�ȷ�����Ƿ�ɹ�	 
	{							   
		return MI_OK;	
	}
	
	return (-1);	
}



//*******************************************  
//��������PS_StoreChar
//���ܣ��� CharBuffer1 �� CharBuffer2 �е�ģ���ļ��浽 PageID ��flash ���ݿ�λ��
//������BufferID(��������)��PageID��ָ�ƿ�λ�úţ�
//ָ����룺06H
//����ֵ��ȷ����
//******************************************* 
char PS_StoreChar(uchar BufferID,uint PageID)
{

	uint sum=0x000d;
	uchar ps1[]="\xef\x01\xff\xff\xff\xff\x01\x00\x06\x06\x01\x00\x00\x00\x00";	 //���CharBuffer1��ָ����
	uchar ps2[]="\xef\x01\xff\xff\xff\xff\x01\x00\x06\x06\x02\x00\x00\x00\x00";

	if(BufferID==0x01)
	{
		sum+=0x01;
		ps1[11]=PageID/256;
		ps1[12]=PageID%256;
		sum+=ps1[11];
		sum+=ps1[12];
		ps1[13]=sum/256;
		ps1[14]=sum%256;
		Send_Bytes(ps1,15);		       //����ָ����
	}
	else
	{
		sum+=0x02;
		ps2[11]=PageID/256;
		ps2[12]=PageID%256;
		sum+=ps2[11];
		sum+=ps2[12];
		ps2[13]=sum/256;
		ps2[14]=sum%256;
		Send_Bytes(ps2,15);		       //����ָ����
	}

	
	Receive_Bytes(Data_buff,12);   //����Ӧ���
	
	if(Data_buff[9]==0x00&&Data_buff[11]==0x0a)		   //�ж�ȷ�����Ƿ�ɹ�	 
	{	
//		Send_Bytes(Data_buff,12);						   
		return MI_OK;			
	}

	return 1;	
}



//*******************************************  
//��������PS_LoadChar
//���ܣ��� flash ���ݿ���ָ�� ID �ŵ�ָ��ģ����뵽ģ�建���� CharBuffer1 �� CharBuffer2
//������BufferID(��������)��PageID(ָ�ƿ�ģ���)
//ָ����룺07H
//����ֵ��ȷ����
//******************************************* 
char PS_LoadChar(uchar BufferID,uint PageID)
{
	uint sum=0x000e;
	uchar ps1[]="\xef\x01\xff\xff\xff\xff\x01\x00\x06\x07\x01\x00\x00\x00\x09";	 //���CharBuffer1��ָ����
	uchar ps2[]="\xef\x01\xff\xff\xff\xff\x01\x00\x06\x07\x02\x00\x00\x00\x09";

	if(BufferID==0x01)
	{
		sum+=0x01;
		ps1[11]=PageID/256;
		ps1[12]=PageID%256;
		sum+=ps1[11];
		sum+=ps1[12];
		ps1[13]=sum/256;
		ps1[14]=sum%256;
		
		Send_Bytes(ps1,15);		       //����ָ����
	}
	else
	{
		sum+=0x02;
		ps2[11]=PageID/256;
		ps2[12]=PageID%256;
		sum+=ps2[11];
		sum+=ps2[12];
		ps2[13]=sum/256;
		ps2[14]=sum%256;
		
		Send_Bytes(ps2,15);
	}
	
	Receive_Bytes(Data_buff,12);   //����Ӧ���
	
	if(Data_buff[9]==0x00)		   //�ж�ȷ�����Ƿ�ɹ�	 
	{							   
		return MI_OK;	
	}
	
	return (-1);	
}



//*******************************************  
//��������PS_Empty
//���ܣ�ɾ�� flash ���ݿ�������ָ��ģ��
//������NULL
//ָ����룺0dH
//����ֵ��ȷ����
//******************************************* 
char PS_Empty()
{
	uchar ps1[]="\xef\x01\xff\xff\xff\xff\x01\x00\x03\x0d\x00\x11";	 //���CharBuffer1��ָ����


	Send_Bytes(ps1,12);		       //����ָ����

	
	Receive_Bytes(Data_buff,12);   //����Ӧ���
	
	if(Data_buff[9]==0x00)		   //�ж�ȷ�����Ƿ�ɹ�	 
	{							   
		return MI_OK;	
	}
	
	return (-1);	
}



//*******************************************  
//��������PS_VfyPwd
//���ܣ� ��֤ģ�����ֿ���
//������NULL
//ָ����룺13H
//����ֵ��ȷ����
//******************************************* 
char PS_VfyPwd()
{
	uchar ps1[]="\xef\x01\xff\xff\xff\xff\x01\x00\x07\x13\x00\x00\x00\x00\x1b";	 //���CharBuffer1��ָ����


	Send_Bytes(ps1,16);		       //����ָ����

	Receive_Bytes(Data_buff,12);   //����Ӧ���
	
	if(Data_buff[9]==0x00)		   //�ж�ȷ�����Ƿ�ɹ�	 
	{				
				   
		return MI_OK;	
	}
	
	return (-1);	
}





//*******************************************  
//��������PS_Enroll
//���ܣ� �ɼ�һ��ָ��ע��ģ�壬��ָ�ƿ���������λ���洢�����ش洢ID
//������NULL
//ָ����룺10H
//����ֵ��ȷ����  ҳ�루����ָ��ģ�壩
//******************************************* 
char PS_Enroll()
{
	uchar ps1[]="\xef\x01\xff\xff\xff\xff\x01\x00\x03\x10\x00\x14";	 //���CharBuffer1��ָ����


	Send_Bytes(ps1,12);		       //����ָ����

	Receive_Bytes(Data_buff,14);   //����Ӧ���
	
	if(Data_buff[9]==0x00)		   //�ж�ȷ�����Ƿ�ɹ�	 
	{				
				   
		return MI_OK;	
	}
	
	return (-1);	
}



//*******************************************  
//��������PS_Identify
//���ܣ� �Զ��ɼ�ָ�ƣ���ָ�ƿ�������Ŀ��ģ�岢�������������
//		 ���Ŀ��ģ��ͬ��ǰ�ɼ���ָ�ƱȶԵ÷ִ�����߷�ֵ������Ŀ��ģ�岻�����������Բɼ�����������Ŀ��ģ��Ŀհ�����
//������NULL
//ָ����룺11H
//����ֵ��ȷ����  ҳ�루����ָ��ģ�壩
//******************************************* 
char PS_Identify()
{
	uchar ps1[]="\xef\x01\xff\xff\xff\xff\x01\x00\x03\x11\x00\x15";	 //���CharBuffer1��ָ����


	Send_Bytes(ps1,12);		       //����ָ����

	Receive_Bytes(Data_buff,16);   //����Ӧ���
	
	if(Data_buff[9]==0x00)		   //�ж�ȷ�����Ƿ�ɹ�	 
	{				
				   
		return MI_OK;	
	}
	
	return (-1);	
}




