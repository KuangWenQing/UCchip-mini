#include<REG52.h>
#include"lcd12864.h"
#include"temp.h"
#include"DHT11.h"
#include "common.h"

#define		on	1
#define		off	0

unsigned char tempbuf,count,flag;
DHT11_info dataCode;
unsigned char code char_WenDu[]="温度：        ℃";
unsigned char code char_ShiDu[]="湿度:         RH";
unsigned char code char_CO2[]="CO2 浓度：   ppm";

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
			if(tempbuf == 0)	//关闭所有继电器
			{heat=off;	light=off;	motor=off;}
			if(tempbuf == 'L')	//打开灯光
					light = on;
			if(tempbuf == 'H')	//加热
					heat = on;
			if(tempbuf == 'M')	//打开水泵
					motor = on;
			
			if(tempbuf == 'l')	//关闭灯光
					light = off;
			if(tempbuf == 'h')	//关闭加热
					heat = off;
			if(tempbuf == 'm')	//关闭水泵
					motor = off;	
			
			if(flag)				//2s钟刷新一次温度湿度
			{
				EA = 0;				//关闭所有中断
				TR1 = 0;			//关闭计数器
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

	TH0 = 76;			//装这个初值，只需50ms就会溢出
	TL0 = 208;
	TH1 = 0xfd;		//波特率9600
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
void display_temp(int temp) 	 //lcd显示温度
{ 
  	unsigned char i, datas[] = {0, 0, 0, 0, 0}; 
		float tp; 
				
		if(temp< 0)				    //当温度为负数时
  	{
	  	LCD12864_SetWindow(0, 3);		
	    LCD12864_WriteData('-');  	     //显示负号
		
			temp=temp-1;						//负数的补码转换
			temp=~temp;
			tp=temp;
			temp=tp*0.0625*100+0.5;			//一个等级为0.0625℃，故乘以0.0625
																	//乘以100用来保留小数点后2位，+0.5实现四舍五入
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

/****************  温度湿度部分  *********************/
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
	if(count==40)		//2s钟标志位置1
	{
		count = 0;
		flag = 1;
	}
}


