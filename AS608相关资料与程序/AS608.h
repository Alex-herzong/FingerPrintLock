/*
	���ߣ�����NK
	������51����̳
	ʱ�䣺2018��8��3��	

	ps���������������Ǹ����ֲ������ĺ���������С��ѧϰ�����˴��ִ򣬾���û���κγ�Ϯ����ȫ���ɲ飩
*/


#ifndef _AS608_H_


#include "stc12c5a60s2.h"
#include "intrins.h"
#include "stdio.h"

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint  unsigned int
#endif
	
	
#define MAX 64      //���������ն����ֽ�����
/*������*/
#define MI_OK                          0
#define MI_NOTAGERR                    (-1)
#define MI_ERR                         (-2)


/*ȷ����*/	 /*
1. 00h����ʾָ��ִ����ϻ� OK��
2. 01h����ʾ���ݰ����մ���
3. 02h����ʾ��������û����ָ��
4. 03h����ʾ¼��ָ��ͼ��ʧ�ܣ�
5. 04h����ʾָ��ͼ��̫�ɡ�̫����������������
6. 05h����ʾָ��ͼ��̫ʪ��̫����������������
7. 06h����ʾָ��ͼ��̫�Ҷ�������������
8. 07h����ʾָ��ͼ����������������̫�٣������̫С����������������
9. 08h����ʾָ�Ʋ�ƥ�䣻
10. 09h����ʾû������ָ�ƣ�
11. 0ah����ʾ�����ϲ�ʧ�ܣ�
12. 0bh����ʾ����ָ�ƿ�ʱ��ַ��ų���ָ�ƿⷶΧ��
13. 0ch����ʾ��ָ�ƿ��ģ��������Ч��
14. 0dh����ʾ�ϴ�����ʧ�ܣ�
15. 0eh����ʾģ�鲻�ܽ��ܺ������ݰ���
16. 0fh����ʾ�ϴ�ͼ��ʧ�ܣ�
17. 10h����ʾɾ��ģ��ʧ�ܣ�
18. 11h����ʾ���ָ�ƿ�ʧ�ܣ�
19. 12h����ʾ���ܽ���͹���״̬��
20. 13h����ʾ�����ȷ��
21. 14h����ʾϵͳ��λʧ�ܣ�
22. 15H����ʾ��������û����Чԭʼͼ��������ͼ��
23. 16H����ʾ��������ʧ�ܣ�
24. 17H����ʾ����ָ�ƻ����βɼ�֮����ָû���ƶ�����
25. 18H����ʾ��д FLASH ����
26. 0xf0���к������ݰ���ָ���ȷ���պ��� 0xf0 Ӧ��
27. 0xf1���к������ݰ���ָ�������� 0xf1 Ӧ��
28. 0xf2����ʾ��д�ڲ� FLASH ʱ��У��ʹ���
29. 0xf3����ʾ��д�ڲ� FLASH ʱ������ʶ����
30. 0xf4����ʾ��д�ڲ� FLASH ʱ�������ȴ���
31. 0xf5����ʾ��д�ڲ� FLASH ʱ�����볤��̫����
32. 0xf6����ʾ��д�ڲ� FLASH ʱ����д FLASH ʧ�ܣ�
33. 0x19��δ�������
34. 0x1a����Ч�Ĵ����ţ�
35. 0x1b���Ĵ����趨���ݴ���ţ�
36. 0x1c�����±�ҳ��ָ������
37. 0x1d���˿ڲ���ʧ�ܣ�
38. 0x1e���Զ�ע�ᣨenroll��ʧ�ܣ�
39. 0x1f��ָ�ƿ���
40. 0x20��0xefh��Reserved��
*/


/**/
void SendOneByte(unsigned char c);
void Send_Bytes(uchar *c,uchar len);
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


char PS_StoreChar(uchar BufferID,uint PageID);

#endif
