/**
  ******************************************************************************
  * @file    xxx.h
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _KEY_H_
#define _KEY_H_

/* Includes ------------------------------------------------------------------*/
#include "MacroAndConst.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
enum {
	KEY_NULL = 0,
	KEY_SHORT_USER,
	KEY_LONG_USER,
	KEY_REPEAT_USER,
	KEY_D_CLICK_USER,
	KEY_S_CLICK_USER = KEY_SHORT_USER
};

typedef enum {
	KEY_DOWN = 0,
	KEY_PRESSED,
	KEY_UP,
	KEY_LONG,
	KEY_REPEAT,
	KEY_S_CLICK,
	KEY_D_CLICK
}ukey_t;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern uint8_t ukey_read(void);

#endif
/*-------------------------------END OF FILE-------------------------------*/

