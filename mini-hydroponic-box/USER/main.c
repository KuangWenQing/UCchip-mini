#include<REG52.h>
#include"lcd12864.h"
#include"temp.h"
#include"DHT11.h"
#include "common.h"

#define		on	1
#define		off	0

unsigned char tempbuf,count,flag;
DHT11_info dataCode;
unsigned char code char_WenDu[]="�¶ȣ�        ��";
unsigned char code char_ShiDu[]="ʪ��:         RH";
unsigned char code char_CO2[]="CO2 Ũ�ȣ�   ppm";

sbit heat  = P3^6;
sbit light = P3^5;
sbit motor = P3^4;
void BlueteethInit();
void init();
void display_temp(int temp);
void display_DHT11();
void LcdDisplay_DHT();

void main()
{
		tempbuf= 0;
		count = 0;
		flag = 0;
		init();
		while(1)
		{
			if(tempbuf == 0)	//�ر����м̵���
			{heat=off;	light=off;	motor=off;}
			if(tempbuf == 'L')	//�򿪵ƹ�
					light = on;
			if(tempbuf == 'H')	//����
					heat = on;
			if(tempbuf == 'M')	//��ˮ��
					motor = on;
			
			if(tempbuf == 'l')	//�رյƹ�
					light = off;
			if(tempbuf == 'h')	//�رռ���
					heat = off;
			if(tempbuf == 'm')	//�ر�ˮ��
					motor = off;	
			
			if(flag)				//2s��ˢ��һ���¶�ʪ��
			{
				EA = 0;				//�ر������ж�
				TR1 = 0;			//�رռ�����
				TR0 = 0;
		
				display_temp(Ds18b20ReadTemp());	
				display_DHT11();
				
				flag = 0;
				BlueteethInit();
			}
		}	
}

void BlueteethInit()
{
	SCON = 0x50;	
	TMOD = 0x21;	

	TH0 = 76;			//װ�����ֵ��ֻ��50ms�ͻ����
	TL0 = 208;
	TH1 = 0xfd;		//������9600
	TL1 = 0xfd;

	TR1 = 1;
	TR0 = 1;
	ET0=1;
	ET1=1;
	ES = 1;
	EA = 1;
}

void init()
{		
		unsigned char i = 0;
		BlueteethInit();
		P1 = 0;	P3 = 0;
		LCD12864_Init();
		LCD12864_SetWindow(0, 0);
		while(char_WenDu[i]!='\0')
		{
				LCD12864_WriteData(char_WenDu[i]);
				i++;
		}
		i = 0;
		LCD12864_SetWindow(1, 0);
		while(char_ShiDu[i]!='\0')
		{
				LCD12864_WriteData(char_ShiDu[i]);
				i++;
		}
		i = 0;
		LCD12864_SetWindow(2, 0);
		while(char_CO2[i]!='\0')
		{
				LCD12864_WriteData(char_CO2[i]);
				i++;
		}
}

/************** DS18B20 *****************/
void display_temp(int temp) 	 //lcd��ʾ�¶�
{ 
  	unsigned char i, datas[] = {0, 0, 0, 0, 0}; 
		float tp; 
				
		if(temp< 0)				    //���¶�Ϊ����ʱ
  	{
	  	LCD12864_SetWindow(0, 3);		
	    LCD12864_WriteData('-');  	     //��ʾ����
		
			temp=temp-1;						//�����Ĳ���ת��
			temp=~temp;
			tp=temp;
			temp=tp*0.0625*100+0.5;			//һ���ȼ�Ϊ0.0625�棬�ʳ���0.0625
																	//����100��������С�����2λ��+0.5ʵ����������
  	}
		else
  	{			
	  	LCD12864_SetWindow(0, 3);		
			LCD12864_WriteData('+'); 			
			tp=temp;
			temp=tp*0.0625*100+0.5;	
		}
		datas[0] = temp / 10000;
		datas[1] = temp % 10000 / 1000;
		datas[2] = temp % 1000 / 100;
		datas[3] = temp % 100 / 10;
		datas[4] = temp % 10;
		for(i=0;i<5;i++)
		{	
				if(i == 3)
						LCD12864_WriteData('.');			
				LCD12864_WriteData('0'+datas[i]);
		}
}

/****************  �¶�ʪ�Ȳ���  *********************/
void display_DHT11()
{	
	unsigned char i = 0;
	if(DHT11Init()==DHT11_EXISTENCT) 
	{
			i=0;
			while((DHT11_DATA_BIT )&& (i < 250))
				i++; 
	
			DHT11ReadInfo(&dataCode) ;
	
			LCD12864_SetWindow(1, 3);				//
			LCD12864_WriteData('X'+(dataCode.Humidity_H / 10));
			LCD12864_WriteData('X'+(dataCode.Humidity_H % 10));
			LCD12864_WriteData('.');
			LCD12864_WriteData('X'+(dataCode.Humidity_L / 10));
			LCD12864_WriteData('X'+(dataCode.Humidity_L % 10));		
	}
}

void Serial(void) interrupt 4
{
	tempbuf = SBUF;
	RI = 0;					
}
void T0_time() interrupt 1
{
	TH0 = 76;
	TL0 = 208;
	count++;
	if(count==40)		//2s�ӱ�־λ��1
	{
		count = 0;
		flag = 1;
	}
}


