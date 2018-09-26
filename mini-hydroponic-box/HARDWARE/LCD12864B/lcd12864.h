#ifndef __LCD12864_H
#define __LCD12864_H

//---����ͷ�ļ�---//
#include<reg52.h>

//---�ض���ؼ���---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

//---���ʹ�û�ͼģʽ�������---//

//#define LCD12864_PICTURE

//---����ʹ�õ�IO��---//

#define LCD12864_DATAPORT P2	  //����IO��

sbit LCD12864_RS  =  P1^6;             //����������Ĵ���ѡ������ 
sbit LCD12864_RW  =  P1^5;             //Һ����/д����
sbit LCD12864_EN  =  P1^4;             //Һ��ʹ�ܿ���
sbit LCD12864_PSB =  P1^2;             //��/����ʽ����
sbit LCD12864_RST =  P1^0;			   //��λ��


//---����ȫ�ֺ���---//
void LCD12864_Delay1ms(uint c);
uchar LCD12864_Busy(void);
void LCD12864_WriteCmd(uchar cmd);
void LCD12864_WriteData(uchar dat);
void LCD12864_Init();
void LCD12864_ClearScreen(void);
void LCD12864_SetWindow(uchar x, uchar y);

#endif