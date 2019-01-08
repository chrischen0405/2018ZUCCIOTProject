/**
  ******************************************************************************
  * @file    Hal_OLED.c
  * @author  jason
	* @Tel     18938045680
	* @QQ      570526723 
  * @version V2.0
  * @date    12/05/2014
  * @brief
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, jason SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2014 jason</center></h2>
  */ 
#include "include.h"
#include "hal_rgb_led.h"
#include "stm32f4xx_rng.h"

void LED_delay(unsigned int  ms)
{
	volatile unsigned  int i=0;
     for(i=0;i<ms;i++);
  
}


/************ generation clock *********************/
void ClkProduce(void)
{  
  SCL_LOW;    // SCL=0
	LED_delay(400);
  SCL_HIGH;     // SCL=1
  LED_delay(400);  
}
 

/**********  send 32 zero ********************/
void Send32Zero(void)
{
  unsigned char i; 
	SDA_LOW;   // SDA=0
	for (i=0; i<32; i++)
		ClkProduce();
}


 /********* invert the grey value of the first two bits ***************/
uint8_t TakeAntiCode(uint8_t dat)
{
    uint8_t tmp = 0;

	tmp=((~dat) & 0xC0)>>6;		 
	return tmp;
}
 

/****** send gray data *********/
void DatSend(uint32_t dx)
{
    uint8_t i;
 
	for (i=0; i<32; i++)
	{
	    if ((dx & 0x80000000) != 0)
		{
	        
	        SDA_HIGH;     //  SDA=1; 
		}
		else
		{
             SDA_LOW;    //  SDA=0; 
		}
 
		dx <<= 1;
    ClkProduce();
	}
}
 
/******* data processing  ********************/
void DataDealWithAndSend(uint8_t r, uint8_t g, uint8_t b)
{
    uint32_t dx = 0;
 
    dx |= (uint32_t)0x03 << 30;             // The front of the two bits 1 is flag bits
    dx |= (uint32_t)TakeAntiCode(b) << 28;
    dx |= (uint32_t)TakeAntiCode(g) << 26;	
    dx |= (uint32_t)TakeAntiCode(r) << 24;
 
    dx |= (uint32_t)b << 16;
    dx |= (uint32_t)g << 8;
    dx |= r;
 
    DatSend(dx);
}

void RGB_LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
//	RCC_APB2PeriphClockCmd(GPIO_SCK_CLK | GPIO_SDA_CLK, ENABLE);
	
//	GPIO_WriteBit(GPIOB, GPIO_SCK_PIN, Bit_SET);
//	GPIO_WriteBit(GPIOB, GPIO_SDA_PIN, Bit_SET);
//	GPIO_WriteBit(GPIOB, GPIO_SCK_PIN, Bit_RESET);
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//GPIO_PuPd_DOWN;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_SCK_PIN | GPIO_SDA_PIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

 	Send32Zero();	
	DataDealWithAndSend(0,0,0);	  // display red
	DataDealWithAndSend(0,0,0);	

//	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE); //开启RNG时钟  
//	RNG_Cmd(ENABLE);  //使能RNG

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//GPIO_PuPd_DOWN;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	GPIO_SetBits(GPIOA,GPIO_Pin_0);	//打开RGB电源开关。PA0
}

void LED_RGB_Control(uint8_t R, uint8_t G, uint8_t B)
{
	Send32Zero();
	DataDealWithAndSend(R, G, B);	  // display red
	DataDealWithAndSend(R, G, B);	  // display red
//	printf("LED_R = %d LED_G = %d LED_B = %d",R,G,B);
}
