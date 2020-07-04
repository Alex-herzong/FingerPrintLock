/*
	作者：超神NK
	发处：51hei论坛
	时间：2018年8月3号


	ps：纯个人整理，都是根据手册命名的函数，帮助小白学习，本人纯手打，绝对没有任何抄袭。（全网可查）

	*ps：2018年8月5号更新，有些函数没有去添加，感觉不太常用的，剩下的也就是记事本功能有点用，这个还是留给大家去添加吧！
*/


#ifndef _AS608_H_


#include "reg52.h"

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint  unsigned int
#endif

#ifndef ulong
#define ulong  unsigned long
#endif


#define MAX 128      //定义最大接收多少字节数据(根据数据包大小定义，默认是128)

/*错误码*/
#define MI_OK                          0
#define MI_NOTAGERR                    (-1)
#define MI_ERR                         (-2)




/*

PS:这里只是定义了一些，还有很多可以添加。
例如：芯片地址保存、记事本的用户信息等，可自行在里面定义

*/
typedef struct as608
{
    uint StateRegister;	       //保存设备的状态寄存器

    uint SensorType;	   	   //保存传感器类型

    uint zw_library;		   //保存设备的指纹库大小

    uint SecurLevel;		   //保存设备的安全等级

    unsigned long addr;   	   //保存设备地址

    uint  CFG_PktSize;		   //保存设备的数据包大小

    unsigned long UARTs;	   //保存设备的波特率大小

    unsigned long Random;	   //用来存取返回的随机数

//  ......
};



extern struct as608 As_608_data;





/*返回的确认码说明合集*/	 /*
1. 00h：表示指令执行完毕或 OK；
2. 01h：表示数据包接收错误；
3. 02h：表示传感器上没有手指；
4. 03h：表示录入指纹图像失败；
5. 04h：表示指纹图像太干、太淡而生不成特征；
6. 05h：表示指纹图像太湿、太糊而生不成特征；
7. 06h：表示指纹图像太乱而生不成特征；
8. 07h：表示指纹图像正常，但特征点太少（或面积太小）而生不成特征；
9. 08h：表示指纹不匹配；
10. 09h：表示没搜索到指纹；
11. 0ah：表示特征合并失败；
12. 0bh：表示访问指纹库时地址序号超出指纹库范围；
13. 0ch：表示从指纹库读模板出错或无效；
14. 0dh：表示上传特征失败；
15. 0eh：表示模块不能接受后续数据包；
16. 0fh：表示上传图像失败；
17. 10h：表示删除模板失败；
18. 11h：表示清空指纹库失败；
19. 12h：表示不能进入低功耗状态；
20. 13h：表示口令不正确；
21. 14h：表示系统复位失败；
22. 15H：表示缓冲区内没有有效原始图而生不成图像；
23. 16H：表示在线升级失败；
24. 17H：表示残留指纹或两次采集之间手指没有移动过；
25. 18H：表示读写 FLASH 出错；
26. 0xf0：有后续数据包的指令，正确接收后用 0xf0 应答；
27. 0xf1：有后续数据包的指令，命令包用 0xf1 应答；
28. 0xf2：表示烧写内部 FLASH 时，校验和错误；
29. 0xf3：表示烧写内部 FLASH 时，包标识错误；
30. 0xf4：表示烧写内部 FLASH 时，包长度错误；
31. 0xf5：表示烧写内部 FLASH 时，代码长度太长；
32. 0xf6：表示烧写内部 FLASH 时，烧写 FLASH 失败；
33. 0x19：未定义错误；
34. 0x1a：无效寄存器号；
35. 0x1b：寄存器设定内容错误号；
36. 0x1c：记事本页码指定错误；
37. 0x1d：端口操作失败；
38. 0x1e：自动注册（enroll）失败；
39. 0x1f：指纹库满
40. 0x20—0xefh：Reserved。
*/


/*函数声明*/
void SendOneByte(unsigned char c);
char Receive_Bytes(uchar *c,uchar len);
void UART_Init(void);
char PS_GetImage();
char PS_GenChar(uchar BufferID);
char PS_Match();
char PS_Search(uchar BufferID);
char PS_RegModel();
char PS_StoreChar(uchar BufferID,uint PageID);
char PS_LoadChar(uchar BufferID,uint PageID);
char PS_Empty();
char PS_VfyPwd();
char PS_Enroll();
char PS_Identify();
char PS_DeletChar(uint PageID,uint count);
char PS_WriteReg(uchar reg,uchar cont);
char PS_ReadSysPara();
char PS_Enroll();
char PS_Identify();
char PS_SetPwd(unsigned long PassWord);
char PS_VfyPwd(unsigned long PassWord);
char PS_GetRandomCode();
char PS_StoreChar(uchar BufferID,uint PageID);



char Record_SaveLibrary(uint ID);

#endif
