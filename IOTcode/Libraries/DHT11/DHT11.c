/**
  ******************************************************************************
  * @file    xxx.c 
  * @author  Waveshare Team
  * @version 
  * @date    xx-xx-2014
  * @brief   xxxxx.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, WAVESHARE SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "LIB_Config.h"
#include "DHT11.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief reset DHT11
 * @param None
 * @retval None
 */
static void dht11_reset(void)	   
{				  
	DHT11_DQ_OUT(); 	//SET OUTPUT
	DHT11_DQ_CLR(); 	//
	delay_ms(20);		//Pull down Least 18ms
	DHT11_DQ_SET(); 	// 
	delay_us(30);		//Pull up 20~40us
}

/**
 * @brief  check for DHT11
 * @param  None
 * @retval None
 */
static bool dht11_check(void)	   
{	
	uint8_t chRetry = 0;
	
	DHT11_DQ_IN();//SET INPUT	 
	while (DHT11_DQ_READ() && chRetry < 100) {  //DHT11 Pull down 40~80us
		chRetry ++;
		delay_us(1);
	}
	
	if(chRetry >= 100){
		return false;
	} else {
		chRetry = 0;
	}
	
	while (!(DHT11_DQ_READ()) && chRetry < 100) {  //DHT11 Pull up 40~80us
		chRetry ++;
		delay_us(1);
	}
	
	if(chRetry >= 100){
		return false; //chack error	
	}
	
	return true;
}

static uint8_t dht11_read_bit(void) 			 
{
	uint8_t chRetry = 0;
	
	while(DHT11_DQ_READ() && chRetry < 100)	{ //wait become Low level
		chRetry ++;
		delay_us(1);
	}
	chRetry = 0;
	while(!(DHT11_DQ_READ()) && chRetry < 100) { //wait become High level
		chRetry ++;
		delay_us(1);
	}
	
	delay_us(40);//wait 40us
	if(DHT11_DQ_READ()) {
		return 0x01;
	} else {
		return 0x00;
	}
}

static uint8_t dht11_read_byte(void)	
{		 
	uint8_t i;
	uint8_t chDat = 0;

	for (i = 0; i < 8; i ++)  {
		chDat <<= 1; 
		chDat |= dht11_read_bit();
	}
	return chDat;
}

bool dht11_read_data(uint8_t *pchTemper, uint8_t *pchHumidity)	
{	
	uint8_t i;
	uint8_t chBuf[5];

	if(NULL == pchTemper || NULL == pchHumidity) {
		return false;
	}
	
	dht11_reset();
	if(dht11_check()) {
		for(i = 0; i < 5; i ++) {
			chBuf[i] = dht11_read_byte();
		}
		if((chBuf[0] + chBuf[1] + chBuf[2] + chBuf[3]) == chBuf[4]) {
			*pchHumidity = chBuf[0];
			*pchTemper = chBuf[2];
		}
	} else {
		return false;
	}
	
	return true;		
}
	 
bool dht11_init(void)
{	 				 		
	dht11_reset();  
	return dht11_check();
} 

/*-------------------------------END OF FILE-------------------------------*/

