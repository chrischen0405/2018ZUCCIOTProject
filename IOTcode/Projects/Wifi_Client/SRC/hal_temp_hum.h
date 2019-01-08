#ifndef _HAL_HEMP_HUM_H
#define _HAL_HEMP_HUM_H

#include <stdio.h>
#include <stm32f4xx.h>
#include "delay.h"
//#include "sys.h"  

#define	DHT11_DQ_PIN   GPIO_Pin_3
#define	DHT11_DQ_GPIO  GPIOB

//Set GPIO Direction
#define DHT11_IO_IN()  GPIO_InitTypeDef GPIO_InitStruct;\
	                     GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;\
	                     GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;\
											 GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;\
											 GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;\
	                     GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;\
	                     GPIO_Init(GPIOB, &GPIO_InitStruct);
											 
#define DHT11_IO_OUT() GPIO_InitTypeDef GPIO_InitStruct;\
	                     GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;\
	                     GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;\
											 GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;\
											 GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;\
	                     GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;\
	                     GPIO_Init(GPIOB, &GPIO_InitStruct);
	

#define DHT11_DQ_SET() GPIO_WriteBit(DHT11_DQ_GPIO, DHT11_DQ_PIN, Bit_SET)  
#define DHT11_DQ_CLR() GPIO_WriteBit(DHT11_DQ_GPIO, DHT11_DQ_PIN, Bit_RESET)
#define DHT11_DQ_READ() GPIO_ReadInputDataBit(DHT11_DQ_GPIO, DHT11_DQ_PIN)


u8 DHT11_Init(void); //Init DHT11
u8 DHT11_Read_Data(u8 *temperature,u8 *humidity); //Read DHT11 Value
u8 DHT11_Read_Byte(void);//Read One Byte
u8 DHT11_Read_Bit(void);//Read One Bit
u8 DHT11_Check(void);//Chack DHT11
void DHT11_Rst(void);//Reset DHT11    



#endif /*_HAL_HEMP_HUM_H*/

