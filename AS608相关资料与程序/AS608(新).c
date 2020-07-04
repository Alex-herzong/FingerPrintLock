/*
	作者：超神NK
	发处：51hei论坛
	时间：2018年8月3号	


	ps：纯个人整理，都是根据手册命名的函数，帮助小白学习，本人纯手打，绝对没有任何抄袭。（全网可查）

	*ps：2018年8月5号更新，有些函数没有去添加，感觉不太常用的，剩下的也就是记事本功能有点用，这个还是留给大家去添加吧！
*/

#include "AS608.h"


uchar xdata Data_buff[MAX];

/*初始化结构体*/
struct as608 As_608_data={0};


/*通过串口发送一个字节*/
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

void UART_Init(void)  //波特率为19200，12T  晶振11.0592MHz
{

    TMOD = 0x20;
    SCON = 0x50;
    TH1 = 0xFD;
    TL1 = TH1;
    PCON = 0x80;
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
//函数名：Send_Bytes
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

	
	if(Receive_Bytes(Data_buff,14))   //接收应答包
	{
		return MI_ERR;	
	}		
	
	if(Data_buff[9]==0x00)		   //判断确认码是否成功	 
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


	if(Receive_Bytes(Data_buff,12))   //接收应答包
	{
		return MI_ERR;	
	}		

	
	if(Data_buff[9]==0x00)		   //判断确认码是否成功	 
	{							   
		return MI_OK;	
	}
	
	return MI_ERR;	
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


	if(Receive_Bytes(Data_buff,12))   //接收应答包
	{
		return MI_ERR;	
	}		
	
	if(Data_buff[9]==0x00&&Data_buff[11]==0x0a)		   //判断确认码是否成功	 
	{							   
		return MI_OK;			
	}

	return MI_ERR;	
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
	
	if(Receive_Bytes(Data_buff,12))   //接收应答包
	{
		return MI_ERR;	
	}		
	
	if(Data_buff[9]==0x00)		   //判断确认码是否成功	 
	{							   
		return MI_OK;	
	}
	
	return MI_ERR;	
}

//

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


	if(Receive_Bytes(Data_buff,12))   //接收应答包
	{
		return MI_ERR;	
	}		
	
	if(Data_buff[9]==0x00)		   //判断确认码是否成功	 
	{							   
		return MI_OK;	
	}
	
	return MI_ERR;	
}



//*******************************************  
//函数名：PS_DeletChar
//功能：删除 flash 数据库中指定 ID 号开始的 N 个指纹模板
//参数：PageID(指纹库模板号)，N 删除的模板个数
//指令代码：0cH
//返回值：确认码
//******************************************* 
char PS_DeletChar(uint PageID,uint count)
{
	uint sum=0x14;
	uchar ps1[]="\xef\x01\xff\xff\xff\xff\x01\x00\x07\x0c\x00\x00\x00\x00\x00\x00";	 //存放CharBuffer1的指令码

	ps1[10]=PageID/256;
	ps1[11]=PageID%256;

	ps1[12]=count/256;
	ps1[13]=count%256;

	sum=sum+ps1[10]+ps1[11]+ps1[12]+ps1[13];
	
	ps1[14]=sum/256;
	ps1[15]=sum%256;

	Send_Bytes(ps1,16);		       //发送指令码

	
	if(Receive_Bytes(Data_buff,12))   //接收应答包
	{
		return MI_ERR;	
	}		

	
	if(Data_buff[9]==0x00)		   //判断确认码是否成功	 
	{							   
		return MI_OK;	
	}
	
	return MI_ERR;	
}






//*******************************************  
//函数名：PS_WriteReg
//功能： 写模块寄存器
//参数：reg： 寄存器序号(04:改变波特率，必须是9600的N倍)(05:比对阀值寄存器)(06:设定包大小寄存器) 
//		cont：值		
//指令代码：0eH
//返回值：确认码


/*    PS: 建议 比对阀值寄存器和设定包大小寄存器不要去改变， 波特率设置可以更改  */

//******************************************* 
char PS_WriteReg(uchar reg,uchar cont)
{
	uint sum=0x14;
	uchar ps1[]="\xef\x01\xff\xff\xff\xff\x01\x00\x05\x0e\x00\x00\x00\x1b";	 //存放CharBuffer1的指令码

	ps1[10]=reg;
	ps1[11]=cont;
	sum+=ps1[10];
	sum+=ps1[11];
	ps1[12]=sum/256;
	ps1[13]=sum%256;



	Send_Bytes(ps1,14);		       //发送指令码


	if(Receive_Bytes(Data_buff,12))   //接收应答包
	{
		return MI_ERR;	
	}		
	
	if(Data_buff[9]==0x00)		   //判断确认码是否成功	 
	{				
				   
		return MI_OK;	
	}
	
	return MI_ERR;	
}


//*******************************************  
//函数名：PS_ReadSysPara
//功能： 读取模块的基本参数（波特率，包大小等）     参数表前 16 个字节存放了模块的基本通讯和配置信息，称为模块的基本参数
//参数： NULL		
//指令代码：0fH
//返回值：确认码+ 基本参数（16bytes）
//******************************************* 
char PS_ReadSysPara()
{
	
	uchar ps1[]="\xef\x01\xff\xff\xff\xff\x01\x00\x03\x0f\x00\x13";	 //存放CharBuffer1的指令码

	Send_Bytes(ps1,12);		       //发送指令码

	if(Receive_Bytes(Data_buff,28))   //接收应答包
	{
		return MI_ERR;	
	}		
	
	if(Data_buff[9]==0x00)		   //判断确认码是否成功  并接收数据到结构体	 
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


	if(Receive_Bytes(Data_buff,14))   //接收应答包
	{
		return MI_ERR;	
	}		
	
	if(Data_buff[9]==0x00)		   //判断确认码是否成功	 
	{				
				   
		return MI_OK;	
	}
	
	return MI_ERR;	
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



//*******************************************  
//函数名：PS_SetPwd
//功能： 设置模块握手口令
//参数：PassWord
//指令代码：12H
//返回值：确认码  
//******************************************* 
char PS_SetPwd(unsigned long PassWord)
{
	uint sum=0x1a;
	uchar ps1[]="\xef\x01\xff\xff\xff\xff\x01\x00\x07\x12\x00\x00\x00\x00\x00\x00";	 //存放CharBuffer1的指令码

	ps1[10]=PassWord/256/256/256;
	ps1[11]=PassWord/256/256%256;
	ps1[12]=PassWord/256%256;
	ps1[13]=PassWord%256;
	 
	sum=(uint)((long)sum+ps1[10]+ps1[11]+ps1[12]+ps1[13]);

	ps1[14]=sum/256;
	ps1[15]=sum%256;

	Send_Bytes(ps1,16);		       //发送指令码


	if(Receive_Bytes(Data_buff,12))   //接收应答包
	{
		return MI_ERR;	
	}		
	
	if(Data_buff[9]==0x00)		   //判断确认码是否成功	 
	{				
				   
		return MI_OK;	
	}
	
	return MI_ERR;	
}



//*******************************************  
//函数名：PS_VfyPwd
//功能： 验证模块握手口令
//参数：PassWord
//指令代码：13H
//返回值：确认码  
//******************************************* 
char PS_VfyPwd(unsigned long PassWord)
{
	uint sum=0x1b;
	uchar ps1[]="\xef\x01\xff\xff\xff\xff\x01\x00\x07\x12\x00\x00\x00\x00\x00\x00";	 //存放CharBuffer1的指令码

	ps1[10]=PassWord/256/256/256;
	ps1[11]=PassWord/256/256%256;
	ps1[12]=PassWord/256%256;
	ps1[13]=PassWord%256;
	 
	sum=(uint)((long)sum+ps1[10]+ps1[11]+ps1[12]+ps1[13]);

	ps1[14]=sum/256;
	ps1[15]=sum%256;

	Send_Bytes(ps1,16);		       //发送指令码

	if(Receive_Bytes(Data_buff,12))   //接收应答包
	{
		return MI_ERR;	
	}		
	
	if(Data_buff[9]==0x00)		   //判断确认码是否成功	 
	{				
				   
		return MI_OK;	
	}
	
	return MI_ERR;	
}



//*******************************************  
//函数名：PS_GetRandomCode
//功能： 令芯片生成一个随机数并返回给上位机
//参数：NULL
//指令代码：14H
//返回值：确认码 + 随机数 
//******************************************* 
char PS_GetRandomCode()
{
	uint sum=0x1b;
	uchar ps1[]="\xef\x01\xff\xff\xff\xff\x01\x00\x03\x14\x00\x18";	 //存放CharBuffer1的指令码


	Send_Bytes(ps1,12);		       //发送指令码

	if(Receive_Bytes(Data_buff,16))   //接收应答包
	{
		return MI_ERR;	
	}		
	
	if(Data_buff[9]==0x00)		   //判断确认码是否成功	 
	{				
		As_608_data.Random=Data_buff[10]<<48;
		As_608_data.Random+=Data_buff[11]<<32;	
		As_608_data.Random+=Data_buff[12]<<16;
		As_608_data.Random+=Data_buff[13];	   
		return MI_OK;	
	}
	
	return MI_ERR;	
}












