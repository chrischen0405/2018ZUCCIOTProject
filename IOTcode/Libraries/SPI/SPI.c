/**
  ******************************************************************************
  * @file    SPI.c 
  * @author  Waveshare Team
  * @version 
  * @date    13-October-2014
  * @brief   This file provides a set of functions to manage the communication between SPI peripherals.
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
#include "SPI.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
uint8_t spi_read_write_byte(SPI_TypeDef* tSPIx, uint8_t chByte);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief Read or write an byte from or to SPI bus.
  * @param  tSPIx: where x can be 1 , 2 or 3 to select the SPI pripheral.
  * @retval chByte: Receive an byte from SPI bus
  */
uint8_t spi_read_write_byte(SPI_TypeDef* tSPIx, uint8_t chByte)
{		
	uint8_t chRetry = 0;

#ifdef STM32F10X_MD
	while (SPI_I2S_GetFlagStatus(tSPIx, SPI_I2S_FLAG_TXE) == RESET) {
		if (++ chRetry > 200) {
			return 0;
		}
	}
	
	SPI_I2S_SendData(tSPIx, chByte); 
	chRetry=0;
	
	while (SPI_I2S_GetFlagStatus(tSPIx, SPI_I2S_FLAG_RXNE) == RESET) {
		if (++ chRetry > 200) {
			return 0;
		}
	}
	
	return SPI_I2S_ReceiveData(tSPIx);
	
#elif defined(STM32F302x8) || defined(STM32F030) || defined(STM32F401xx)
	uint8_t chTemp = 0;

	while (SPI_I2S_GetFlagStatus(tSPIx, SPI_I2S_FLAG_TXE) == RESET) {
		if (++ chRetry > 200) {
			return 0;
		}
	}

#if defined(STM32F401xx)
	SPI_I2S_SendData(tSPIx, chByte);
#else
	SPI_SendData8(tSPIx, chByte); 
#endif

	chRetry=0;
	while (SPI_I2S_GetFlagStatus(tSPIx, SPI_I2S_FLAG_RXNE) == RESET) {
		if (++ chRetry > 200) {
			return 0;
		}
	}

#if defined(STM32F401xx)
	chTemp = SPI_I2S_ReceiveData(tSPIx);
#else
	chTemp = SPI_ReceiveData8(tSPIx);
#endif
	
	/* Wait until the BSY flag is set */ 
	while(SPI_I2S_GetFlagStatus(tSPIx, SPI_I2S_FLAG_BSY) != RESET) {
	
	}
	
	return chTemp;

#endif
	
}

/*-------------------------------END OF FILE-------------------------------*/

