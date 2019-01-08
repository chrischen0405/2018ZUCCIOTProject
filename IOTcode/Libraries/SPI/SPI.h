/**
  ******************************************************************************
  * @file    SPI.h
  * @author  Waveshare Team
  * @version 
  * @date    13-October-2014
  * @brief   Header for SPI.c file
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _USE_SPI_H_
#define _USE_SPI_H_

/* Includes ------------------------------------------------------------------*/
#include "MacroAndConst.h"
#ifdef STM32F10X_MD
	#include "stm32f10x_spi.h"
#elif defined(STM32F302x8)
	#include "stm32f30x_spi.h"
#elif defined(STM32F030)
	#include "stm32f0xx_spi.h"
#elif defined(STM32F401xx)
	#include "stm32f4xx_spi.h"
#endif

//#include "LIB_Config.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern uint8_t spi_read_write_byte(SPI_TypeDef* tSPIx, uint8_t chByte);

#endif

/*-------------------------------END OF FILE-------------------------------*/

