#ifndef __DH11_H__
#define __DH11_H__
 
#include "common.h"
#include "reg52.h"
 
sbit DHT11_DATA_BIT	 = P3^7 ;/*????????*/
 
typedef struct {
	UB8 Humidity_H ;
	UB8 Humidity_L ;
	UB8 Temperature_H ;
	UB8 Temperature_L ;
	UB8 DHT11_CRC;
}DHT11_info ;
 
#define DHT11_EXISTENCT		0 	/*DHT11??*/
#define DHT11_NOT_EXISTENCT -1	/*DHT11???*/
 
extern SB8  DHT11Init(void) ;
extern void DHT11ReadInfo(DHT11_info *temp) ;

 
 
#endif 