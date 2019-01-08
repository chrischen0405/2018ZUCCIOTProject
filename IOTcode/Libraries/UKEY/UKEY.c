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
#include "UKEY.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief 
 * @param 
 * @retval : 
*/
uint8_t ukey_scan(void)
{
	static ukey_t s_tState = KEY_DOWN;
	static uint8_t s_chTime = 0; 
	uint8_t chScanVal = KEY_NULL, chReturnVal = KEY_NULL;

	chScanVal = UKEY_READ();//

	switch(s_tState) {
		case KEY_DOWN:
			if (!chScanVal) {
				s_tState = KEY_PRESSED;
			}
			break;

		case KEY_PRESSED:
			if (!chScanVal) {
				s_tState = KEY_UP;
			} else {
				s_tState = KEY_DOWN;
			}
			break;

		case KEY_UP:
			if (!chScanVal) {
				if (++ s_chTime >= 50) { 
					s_chTime = 0;
					s_tState = KEY_LONG;
				}
			} else {
				s_chTime = 0;
				chReturnVal = KEY_SHORT_USER;
				s_tState = KEY_DOWN;
			}
			
			break;

		case KEY_LONG:
			if (!chScanVal) {
				chReturnVal = KEY_LONG_USER;
				s_tState = KEY_REPEAT;
			} else {
				s_tState = KEY_DOWN;
			}
			
			break;			

		case KEY_REPEAT:
			if (!chScanVal) {
				if (++ s_chTime >= 10) { 
					s_chTime = 0;
					chReturnVal = KEY_REPEAT_USER;
				}
			} else {
				s_chTime = 0;
				s_tState = KEY_DOWN;
			}
			break;

		default:
			break;
	}
	return chReturnVal;
}

uint8_t ukey_read(void)
{
	static ukey_t s_tState = KEY_S_CLICK;
	static uint8_t s_chTime = 0;
	uint8_t chScanVal = KEY_NULL, chReturnVal = KEY_NULL;

	chScanVal = ukey_scan();
	switch(s_tState) {
		case KEY_S_CLICK:
			if (KEY_SHORT_USER == chScanVal) {
				s_chTime = 0;
				s_tState = KEY_D_CLICK;
			} else {
				chReturnVal = chScanVal;
			}
			break;

		case KEY_D_CLICK:
			if (KEY_SHORT_USER == chScanVal) {
				chReturnVal = KEY_D_CLICK_USER;
				s_tState = KEY_S_CLICK;
			} else {
				if(++ s_chTime >= 25) {
					chReturnVal = KEY_S_CLICK_USER;
					s_tState = KEY_S_CLICK;
				}
			}
			break;
	}

	return chReturnVal;
}

/*-------------------------------END OF FILE-------------------------------*/

