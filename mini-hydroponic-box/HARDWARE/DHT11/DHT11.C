 
#include "DHT11.h"
#include <intrins.h>
 
static void delay20msForDHT11(void) ;
static void delay10usForDHT11(void) ;
static UB8  DHT11ReadByte(void) ;	

static void delay20msForDHT11(void)	//@11.0592MHZ
{
	unsigned char i, j, k;
 
	i = 1;
	j = 216;
	k = 35;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

static void delay10usForDHT11(void)
{
	unsigned char i;
 
	_nop_();
	i = 25;
	while (--i);
}
 
SB8  DHT11Init(void)
{
	SB8 existenceFlag ; 
	UB8 i=0 ;
	
	DHT11_DATA_BIT = HIGH_LEVEL ;
	_nop_() ;
	_nop_() ;
	
	DHT11_DATA_BIT = LOW_LEVEL ;
	delay20msForDHT11() ;
	
	DHT11_DATA_BIT = HIGH_LEVEL ;
	delay10usForDHT11() ;
	delay10usForDHT11() ;
	delay10usForDHT11() ;
	delay10usForDHT11() ;
	delay10usForDHT11() ;
 
	if( !DHT11_DATA_BIT)
		existenceFlag = DHT11_EXISTENCT ;
	else
		existenceFlag = DHT11_NOT_EXISTENCT ;
	
	while((!DHT11_DATA_BIT) && (i<250)) 
		i++ ; 

	return existenceFlag ;
}
 
static UB8  DHT11ReadByte(void)
{
	UB8 j=0 ;
	UB8 i, datCode;
 
	for(i=0;i<8;i++)	 	
	{	
	    datCode <<= 1;
			j=0 ;
  		while( (! DHT11_DATA_BIT) && (j<250))			
				j++ ;   		
		delay10usForDHT11() ;
		delay10usForDHT11() ;
		delay10usForDHT11() ;
		delay10usForDHT11() ;
		if (DHT11_DATA_BIT)	
		{	
			datCode |= 0x01;
			j=0 ;
			while((DHT11_DATA_BIT) &&(j<250))
				j++ ;
		}	
	}
	return datCode;	
}
 
void DHT11ReadInfo(DHT11_info *temp)
{
	temp->Humidity_H 	= DHT11ReadByte() ; //Humidity_H
	temp->Humidity_L 	= DHT11ReadByte() ; //Humidity_L
	temp->Temperature_H = DHT11ReadByte() ; //Temperature_H
	temp->Temperature_L = DHT11ReadByte() ; //Temperature_L
	temp->DHT11_CRC 	= DHT11ReadByte() ; //DHT11_CRC
 
	delay20msForDHT11() ;
	delay20msForDHT11() ;
	delay20msForDHT11() ;
	delay20msForDHT11() ;
	delay20msForDHT11() ;
	delay20msForDHT11() ;
	delay20msForDHT11() ;
	delay20msForDHT11() ;
	delay20msForDHT11() ;
	delay20msForDHT11() ;
}
