/*
	���ߣ�����NK
	������51hei��̳
	ʱ�䣺2018��8��3��	


	ps���������������Ǹ����ֲ������ĺ���������С��ѧϰ�����˴��ִ򣬾���û���κγ�Ϯ����ȫ���ɲ飩

	*ps��2018��8��5�Ÿ��£���Щ����û��ȥ��ӣ��о���̫���õģ�ʣ�µ�Ҳ���Ǽ��±������е��ã���������������ȥ��Ӱɣ�
*/

#include "AS608.h"


uchar xdata Data_buff[MAX];

/*��ʼ���ṹ��*/
struct as608 As_608_data={0};


/*ͨ�����ڷ���һ���ֽ�*/
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

void UART_Init(void)  //������Ϊ19200��12T  ����11.0592MHz
{

    TMOD = 0x20;
    SCON = 0x50;
    TH1 = 0xFD;
    TL1 = TH1;
    PCON = 0x80;
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

	
	if(Receive_Bytes(Data_buff,14))   //����Ӧ���
	{
		return MI_ERR;	
	}		
	
	if(Data_buff[9]==0x00)		   //�ж�ȷ�����Ƿ�ɹ�	 
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


	if(Receive_Bytes(Data_buff,12))   //����Ӧ���
	{
		return MI_ERR;	
	}		

	
	if(Data_buff[9]==0x00)		   //�ж�ȷ�����Ƿ�ɹ�	 
	{							   
		return MI_OK;	
	}
	
	return MI_ERR;	
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


	if(Receive_Bytes(Data_buff,12))   //����Ӧ���
	{
		return MI_ERR;	
	}		
	
	if(Data_buff[9]==0x00&&Data_buff[11]==0x0a)		   //�ж�ȷ�����Ƿ�ɹ�	 
	{							   
		return MI_OK;			
	}

	return MI_ERR;	
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
	
	if(Receive_Bytes(Data_buff,12))   //����Ӧ���
	{
		return MI_ERR;	
	}		
	
	if(Data_buff[9]==0x00)		   //�ж�ȷ�����Ƿ�ɹ�	 
	{							   
		return MI_OK;	
	}
	
	return MI_ERR;	
}

//

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


	if(Receive_Bytes(Data_buff,12))   //����Ӧ���
	{
		return MI_ERR;	
	}		
	
	if(Data_buff[9]==0x00)		   //�ж�ȷ�����Ƿ�ɹ�	 
	{							   
		return MI_OK;	
	}
	
	return MI_ERR;	
}



//*******************************************  
//��������PS_DeletChar
//���ܣ�ɾ�� flash ���ݿ���ָ�� ID �ſ�ʼ�� N ��ָ��ģ��
//������PageID(ָ�ƿ�ģ���)��N ɾ����ģ�����
//ָ����룺0cH
//����ֵ��ȷ����
//******************************************* 
char PS_DeletChar(uint PageID,uint count)
{
	uint sum=0x14;
	uchar ps1[]="\xef\x01\xff\xff\xff\xff\x01\x00\x07\x0c\x00\x00\x00\x00\x00\x00";	 //���CharBuffer1��ָ����

	ps1[10]=PageID/256;
	ps1[11]=PageID%256;

	ps1[12]=count/256;
	ps1[13]=count%256;

	sum=sum+ps1[10]+ps1[11]+ps1[12]+ps1[13];
	
	ps1[14]=sum/256;
	ps1[15]=sum%256;

	Send_Bytes(ps1,16);		       //����ָ����

	
	if(Receive_Bytes(Data_buff,12))   //����Ӧ���
	{
		return MI_ERR;	
	}		

	
	if(Data_buff[9]==0x00)		   //�ж�ȷ�����Ƿ�ɹ�	 
	{							   
		return MI_OK;	
	}
	
	return MI_ERR;	
}






//*******************************************  
//��������PS_WriteReg
//���ܣ� дģ��Ĵ���
//������reg�� �Ĵ������(04:�ı䲨���ʣ�������9600��N��)(05:�ȶԷ�ֵ�Ĵ���)(06:�趨����С�Ĵ���) 
//		cont��ֵ		
//ָ����룺0eH
//����ֵ��ȷ����


/*    PS: ���� �ȶԷ�ֵ�Ĵ������趨����С�Ĵ�����Ҫȥ�ı䣬 ���������ÿ��Ը���  */

//******************************************* 
char PS_WriteReg(uchar reg,uchar cont)
{
	uint sum=0x14;
	uchar ps1[]="\xef\x01\xff\xff\xff\xff\x01\x00\x05\x0e\x00\x00\x00\x1b";	 //���CharBuffer1��ָ����

	ps1[10]=reg;
	ps1[11]=cont;
	sum+=ps1[10];
	sum+=ps1[11];
	ps1[12]=sum/256;
	ps1[13]=sum%256;



	Send_Bytes(ps1,14);		       //����ָ����


	if(Receive_Bytes(Data_buff,12))   //����Ӧ���
	{
		return MI_ERR;	
	}		
	
	if(Data_buff[9]==0x00)		   //�ж�ȷ�����Ƿ�ɹ�	 
	{				
				   
		return MI_OK;	
	}
	
	return MI_ERR;	
}


//*******************************************  
//��������PS_ReadSysPara
//���ܣ� ��ȡģ��Ļ��������������ʣ�����С�ȣ�     ������ǰ 16 ���ֽڴ����ģ��Ļ���ͨѶ��������Ϣ����Ϊģ��Ļ�������
//������ NULL		
//ָ����룺0fH
//����ֵ��ȷ����+ ����������16bytes��
//******************************************* 
char PS_ReadSysPara()
{
	
	uchar ps1[]="\xef\x01\xff\xff\xff\xff\x01\x00\x03\x0f\x00\x13";	 //���CharBuffer1��ָ����

	Send_Bytes(ps1,12);		       //����ָ����

	if(Receive_Bytes(Data_buff,28))   //����Ӧ���
	{
		return MI_ERR;	
	}		
	
	if(Data_buff[9]==0x00)		   //�ж�ȷ�����Ƿ�ɹ�  ���������ݵ��ṹ��	 
	{				
		As_608_data.StateRegister=Data_buff[10]<<16|Data_buff[11];
		As_608_data.SensorType=Data_buff[12]<<16|Data_buff[13];
		As_608_data.zw_library=Data_buff[14]<<16|Data_buff[15];
		As_608_data.SecurLevel=Data_buff[16]<<16|Data_buff[17];		
		As_608_data.addr=(ulong)Data_buff[18]<<48|(ulong)Data_buff[19]<<32|(ulong)Data_buff[20]<<16|(ulong)Data_buff[21];
		As_608_data.CFG_PktSize=Data_buff[22]<<16|Data_buff[23];
		As_608_data.UARTs=9600*(Data_buff[24]<<16|Data_buff[25]);		   
		return MI_OK;	
	}
	
	return MI_ERR;	
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


	if(Receive_Bytes(Data_buff,14))   //����Ӧ���
	{
		return MI_ERR;	
	}		
	
	if(Data_buff[9]==0x00)		   //�ж�ȷ�����Ƿ�ɹ�	 
	{				
				   
		return MI_OK;	
	}
	
	return MI_ERR;	
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



//*******************************************  
//��������PS_SetPwd
//���ܣ� ����ģ�����ֿ���
//������PassWord
//ָ����룺12H
//����ֵ��ȷ����  
//******************************************* 
char PS_SetPwd(unsigned long PassWord)
{
	uint sum=0x1a;
	uchar ps1[]="\xef\x01\xff\xff\xff\xff\x01\x00\x07\x12\x00\x00\x00\x00\x00\x00";	 //���CharBuffer1��ָ����

	ps1[10]=PassWord/256/256/256;
	ps1[11]=PassWord/256/256%256;
	ps1[12]=PassWord/256%256;
	ps1[13]=PassWord%256;
	 
	sum=(uint)((long)sum+ps1[10]+ps1[11]+ps1[12]+ps1[13]);

	ps1[14]=sum/256;
	ps1[15]=sum%256;

	Send_Bytes(ps1,16);		       //����ָ����


	if(Receive_Bytes(Data_buff,12))   //����Ӧ���
	{
		return MI_ERR;	
	}		
	
	if(Data_buff[9]==0x00)		   //�ж�ȷ�����Ƿ�ɹ�	 
	{				
				   
		return MI_OK;	
	}
	
	return MI_ERR;	
}



//*******************************************  
//��������PS_VfyPwd
//���ܣ� ��֤ģ�����ֿ���
//������PassWord
//ָ����룺13H
//����ֵ��ȷ����  
//******************************************* 
char PS_VfyPwd(unsigned long PassWord)
{
	uint sum=0x1b;
	uchar ps1[]="\xef\x01\xff\xff\xff\xff\x01\x00\x07\x12\x00\x00\x00\x00\x00\x00";	 //���CharBuffer1��ָ����

	ps1[10]=PassWord/256/256/256;
	ps1[11]=PassWord/256/256%256;
	ps1[12]=PassWord/256%256;
	ps1[13]=PassWord%256;
	 
	sum=(uint)((long)sum+ps1[10]+ps1[11]+ps1[12]+ps1[13]);

	ps1[14]=sum/256;
	ps1[15]=sum%256;

	Send_Bytes(ps1,16);		       //����ָ����

	if(Receive_Bytes(Data_buff,12))   //����Ӧ���
	{
		return MI_ERR;	
	}		
	
	if(Data_buff[9]==0x00)		   //�ж�ȷ�����Ƿ�ɹ�	 
	{				
				   
		return MI_OK;	
	}
	
	return MI_ERR;	
}



//*******************************************  
//��������PS_GetRandomCode
//���ܣ� ��оƬ����һ������������ظ���λ��
//������NULL
//ָ����룺14H
//����ֵ��ȷ���� + ����� 
//******************************************* 
char PS_GetRandomCode()
{
	uint sum=0x1b;
	uchar ps1[]="\xef\x01\xff\xff\xff\xff\x01\x00\x03\x14\x00\x18";	 //���CharBuffer1��ָ����


	Send_Bytes(ps1,12);		       //����ָ����

	if(Receive_Bytes(Data_buff,16))   //����Ӧ���
	{
		return MI_ERR;	
	}		
	
	if(Data_buff[9]==0x00)		   //�ж�ȷ�����Ƿ�ɹ�	 
	{				
		As_608_data.Random=Data_buff[10]<<48;
		As_608_data.Random+=Data_buff[11]<<32;	
		As_608_data.Random+=Data_buff[12]<<16;
		As_608_data.Random+=Data_buff[13];	   
		return MI_OK;	
	}
	
	return MI_ERR;	
}












