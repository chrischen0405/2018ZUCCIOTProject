/**
  ******************************************************************************
  * @file    Hal_led.c
  * @author  jason
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
#include "Config.h"
#include "LIB_Config.h"
#include "hal_led.h"

void LED_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_LED2_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
//	LED_OFF(LED1);
	LED_OFF(LED2);
}

void LED_ON(uint8_t LEDNUM)
{
	LEDNUM = LED2;
	switch (LEDNUM)
	{
		case LED2:GPIO_SetBits(GPIOA,GPIO_LED2_PIN);
			break;
		default:
			break;
	}
}

void LED_OFF(uint8_t LEDNUM)
{
	LEDNUM = LED2;
	switch (LEDNUM)
	{
		case LED2:GPIO_ResetBits(GPIOA,GPIO_LED2_PIN);
			break;
		default:
			break;
	}
}
