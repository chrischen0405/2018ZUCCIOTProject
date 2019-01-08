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
#ifndef __ROTARY_H
#define __ROTARY_H  

/* Includes ------------------------------------------------------------------*/
#include "MacroAndConst.h"

/* Exported types ------------------------------------------------------------*/
enum {
	SW_NULL = 0,
	SW_SHORT_USER,
	SW_LONG_USER,
	SW_REPEAT_USER,
	SW_D_CLICK_USER,
	SW_S_CLICK_USER = SW_SHORT_USER
};

typedef enum {
	SW_DOWN = 0,
	SW_PRESSED,
	SW_UP,
	SW_LONG,
	SW_REPEAT,
	SW_S_CLICK,
	SW_D_CLICK
}rotary_sw_t;

typedef struct {
	volatile int8_t cEncodeDelta;
	int8_t cLast; 
}rotary_t;

extern rotary_t g_tRotary;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

extern void rotary_encoder_init(rotary_t *ptRotary);
extern void rotary_detect(rotary_t *ptRotary);
extern int8_t rotary_read1(int8_t *pcEncodeDelta);
extern int8_t rotary_read2(int8_t *pcEncodeDelta);
extern int8_t rotary_read4(int8_t *pcEncodeDelta);
extern uint8_t rotary_sw_scan(void);
extern uint8_t rotary_sw_read(void);

extern uint8_t rotary_counter(void);
extern void rotary_detect_sia(uint8_t *pchCounter);
extern void rotary_detect_sib(uint8_t *pchCounter);


#endif
/*-------------------------------END OF FILE-------------------------------*/
