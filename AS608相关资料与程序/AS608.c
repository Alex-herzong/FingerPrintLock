/*
	作者：超神NK
	发处：51黑论坛
	时间：2018年8月3号	

	ps：纯个人整理，都是根据手册命名的函数，帮助小白学习，本人纯手打，绝对没有任何抄袭。（全网可查）
*/

#include "AS608.h"


uchar xdata Data_buff[MAX];


void SendOneByte(unsigned char c)
{
    SBUF = c;
    while(!TI);
    TI = 0;
}

/* putchar 函数重写 */
char putchar(char c)
{
	SendOneByte(c);
	return c;
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
//函数名：Send_Bytes
//功能：接收多个字节
//参数：*c:首地址    len：长度
//指令代码：无
//返回值：  无
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
	Receive_Bytes(Data_buff,12);   //接收应答包
	
	if(Data_buff[9]==0x00&&Data_buff[11]==0x0a)		   //判断确认码是否成功	  后面为校验和判断
	{							   
		return MI_OK;	
	}
	
	return (-1);	
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
	
	Receive_Bytes(Data_buff,12);   //接收应答包
	
	if(Data_buff[9]==0x00&&Data_buff[11]==0x0a)		   //判断确认码是否成功	  后面为校验和判断
	{							   
		return MI_OK;	
	}
	
	return (-1);	
} 



//*******************************************  
//函数名：PS_Match
//功能：精确比对 CharBuffer1 与 CharBuffer2 中的特征文件
//参数：NULL
//指令代码：03H
//返回值：有
//******************************************* 
char PS_Match()
{

	uchar *ps1=     "\xef\x01\xff\xff\xff\xff\x01\x00\x03\x03\x00\x07";	 //存放CharBuffer1的指令码

	Send_Bytes(ps1,12);		       //发送指令码

	
	Receive_Bytes(Data_buff,14);   //接收应答包
	
	if(Data_buff[9]==0x00)		   //判断确认码是否成功	 
	{							   
		return MI_OK;	
	}
	
	return (-1);	
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
	

		
	Receive_Bytes(Data_buff,16);   //接收应答包
	
	write_com(0x8a);
	wstring("应答成功...");

	if(Data_buff[9]==0x00)		   //判断确认码是否成功	 
	{							   
		return MI_OK;	
	}
	
	return (1);	
} 



//*******************************************  
//函数名：PS_RegModel
//功能：将 CharBuffer1 与 CharBuffer2 中的特征文件合并生成模板，结果存于 CharBuffer1 与 CharBuffer2
//参数：NULL
//指令代码：05H
//返回值：确认码
//******************************************* 
char PS_RegModel()
{

	uchar *ps1=     "\xef\x01\xff\xff\xff\xff\x01\x00\x03\x05\x00\x09";	 //存放CharBuffer1的指令码
	

	Send_Bytes(ps1,12);		       //发送指令码

	
	Receive_Bytes(Data_buff,12);   //接收应答包
	
	if(Data_buff[9]==0x00)		   //判断确认码是否成功	 
	{							   
		return MI_OK;	
	}
	
	return (-1);	
}



//*******************************************  
//函数名：PS_StoreChar
//功能：将 CharBuffer1 或 CharBuffer2 中的模板文件存到 PageID 号flash 数据库位置
//参数：BufferID(缓冲区号)，PageID（指纹库位置号）
//指令代码：06H
//返回值：确认码
//******************************************* 
char PS_StoreChar(uchar BufferID,uint PageID)
{

	uint sum=0x000d;
	uchar ps1[]="\xef\x01\xff\xff\xff\xff\x01\x00\x06\x06\x01\x00\x00\x00\x00";	 //存放CharBuffer1的指令码
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
		Send_Bytes(ps1,15);		       //发送指令码
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
		Send_Bytes(ps2,15);		       //发送指令码
	}

	
	Receive_Bytes(Data_buff,12);   //接收应答包
	
	if(Data_buff[9]==0x00&&Data_buff[11]==0x0a)		   //判断确认码是否成功	 
	{	
//		Send_Bytes(Data_buff,12);						   
		return MI_OK;			
	}

	return 1;	
}



//*******************************************  
//函数名：PS_LoadChar
//功能：将 flash 数据库中指定 ID 号的指纹模板读入到模板缓冲区 CharBuffer1 或 CharBuffer2
//参数：BufferID(缓冲区号)，PageID(指纹库模板号)
//指令代码：07H
//返回值：确认码
//******************************************* 
char PS_LoadChar(uchar BufferID,uint PageID)
{
	uint sum=0x000e;
	uchar ps1[]="\xef\x01\xff\xff\xff\xff\x01\x00\x06\x07\x01\x00\x00\x00\x09";	 //存放CharBuffer1的指令码
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
		
		Send_Bytes(ps1,15);		       //发送指令码
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
	
	Receive_Bytes(Data_buff,12);   //接收应答包
	
	if(Data_buff[9]==0x00)		   //判断确认码是否成功	 
	{							   
		return MI_OK;	
	}
	
	return (-1);	
}



//*******************************************  
//函数名：PS_Empty
//功能：删除 flash 数据库中所有指纹模板
//参数：NULL
//指令代码：0dH
//返回值：确认码
//******************************************* 
char PS_Empty()
{
	uchar ps1[]="\xef\x01\xff\xff\xff\xff\x01\x00\x03\x0d\x00\x11";	 //存放CharBuffer1的指令码


	Send_Bytes(ps1,12);		       //发送指令码

	
	Receive_Bytes(Data_buff,12);   //接收应答包
	
	if(Data_buff[9]==0x00)		   //判断确认码是否成功	 
	{							   
		return MI_OK;	
	}
	
	return (-1);	
}



//*******************************************  
//函数名：PS_VfyPwd
//功能： 验证模块握手口令
//参数：NULL
//指令代码：13H
//返回值：确认码
//******************************************* 
char PS_VfyPwd()
{
	uchar ps1[]="\xef\x01\xff\xff\xff\xff\x01\x00\x07\x13\x00\x00\x00\x00\x1b";	 //存放CharBuffer1的指令码


	Send_Bytes(ps1,16);		       //发送指令码

	Receive_Bytes(Data_buff,12);   //接收应答包
	
	if(Data_buff[9]==0x00)		   //判断确认码是否成功	 
	{				
				   
		return MI_OK;	
	}
	
	return (-1);	
}





//*******************************************  
//函数名：PS_Enroll
//功能： 采集一次指纹注册模板，在指纹库中搜索空位并存储，返回存储ID
//参数：NULL
//指令代码：10H
//返回值：确认码  页码（相配指纹模板）
//******************************************* 
char PS_Enroll()
{
	uchar ps1[]="\xef\x01\xff\xff\xff\xff\x01\x00\x03\x10\x00\x14";	 //存放CharBuffer1的指令码


	Send_Bytes(ps1,12);		       //发送指令码

	Receive_Bytes(Data_buff,14);   //接收应答包
	
	if(Data_buff[9]==0x00)		   //判断确认码是否成功	 
	{				
				   
		return MI_OK;	
	}
	
	return (-1);	
}



//*******************************************  
//函数名：PS_Identify
//功能： 自动采集指纹，在指纹库中搜索目标模板并返回搜索结果。
//		 如果目标模板同当前采集的指纹比对得分大于最高阀值，并且目标模板不完整特征则以采集的特征更新目标模板的空白区域。
//参数：NULL
//指令代码：11H
//返回值：确认码  页码（相配指纹模板）
//******************************************* 
char PS_Identify()
{
	uchar ps1[]="\xef\x01\xff\xff\xff\xff\x01\x00\x03\x11\x00\x15";	 //存放CharBuffer1的指令码


	Send_Bytes(ps1,12);		       //发送指令码

	Receive_Bytes(Data_buff,16);   //接收应答包
	
	if(Data_buff[9]==0x00)		   //判断确认码是否成功	 
	{				
				   
		return MI_OK;	
	}
	
	return (-1);	
}




