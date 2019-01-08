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
#include "Rotary.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifndef ROTARY_COUNT_MAX
	#define ROTARY_COUNT_MAX 100
#endif
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
rotary_t g_tRotary;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief 
 * @param 
 * @retval : 
 */
void rotary_encoder_init(rotary_t *ptRotary)
{
	int8_t cNew;

	if (NULL == ptRotary) {
		return;
	}
	
	if (ROTARY_SIA_READ()) {
		cNew = 3;
	}
	if (ROTARY_SIB_READ()) {
		cNew ^= 1;
	}
	ptRotary->cLast = cNew;
	ptRotary->cEncodeDelta = 0;
}
 
void rotary_detect(rotary_t *ptRotary)
{
	int8_t cNew, cDiff;

	if (NULL == ptRotary) {
		return;
	}
	
	cNew = 0;
	if (ROTARY_SIA_READ()) {
		cNew = 3;
	}
	if (ROTARY_SIB_READ()) {
		cNew ^= 1;
	}
	cDiff = ptRotary->cLast - cNew;
	if (cDiff & 1) {
		ptRotary->cLast = cNew;
		ptRotary->cEncodeDelta += (cDiff & 2) - 1;
	}
}

int8_t rotary_read1(int8_t *pcEncodeDelta)
{
	int8_t cVal = 0;

	if (NULL == pcEncodeDelta) {
		return cVal;
	}
	
	SAFE_ATOM_CODE(
		cVal = *pcEncodeDelta;
		*pcEncodeDelta = 0;
	)

	return cVal;
}


int8_t rotary_read2(int8_t *pcEncodeDelta)
{
	int8_t cVal = 0;

	if (NULL == pcEncodeDelta) {
		return cVal;
	}
	
	SAFE_ATOM_CODE(
		cVal = *pcEncodeDelta;
		*pcEncodeDelta &= 1;
	)

	return cVal >> 1;
}

int8_t rotary_read4(int8_t *pcEncodeDelta)
{
	int8_t cVal = 0;

	if (NULL == pcEncodeDelta) {
		return cVal;
	}
	
	SAFE_ATOM_CODE(
		cVal = *pcEncodeDelta;
		*pcEncodeDelta = cVal & 3;
	)
	return cVal >> 2;
}


uint8_t rotary_counter(void)
{
	static uint8_t s_chCounter = 0;
	
	if (ROTARY_SIA_READ() == ROTARY_SIB_READ()) {
	//if(ROTARY_SIB_READ()) {
		if (++ s_chCounter >= ROTARY_COUNT_MAX) {
			s_chCounter = ROTARY_COUNT_MAX;
		}
	} else {
		if (s_chCounter > 0) {
			-- s_chCounter;
		}
	}

	return s_chCounter;
}

void rotary_detect_sia(uint8_t *pchCounter)
{
	if (NULL == pchCounter) {
		return;
	}

	if (ROTARY_SIA_READ() && ROTARY_SIB_READ()) {
		(*pchCounter) ++;
	} else {
		(*pchCounter) --;
	}
}

void rotary_detect_sib(uint8_t *pchCounter)
{
	if (NULL == pchCounter) {
		return;
	}

	if (ROTARY_SIA_READ() && ROTARY_SIB_READ()) {
		(*pchCounter) --;
	} else {
		(*pchCounter) ++;
	}
}

uint8_t rotary_sw_scan(void)
{
	static rotary_sw_t s_tState = SW_DOWN;
	static uint8_t s_chTime = 0; 
	uint8_t chScanVal = SW_NULL, chReturnVal = SW_NULL;

	chScanVal = ROTARY_SW_READ();//

	switch (s_tState) {
		case SW_DOWN:
			if(!chScanVal) {
				s_tState = SW_PRESSED;
			}
			break;

		case SW_PRESSED:
			if(!chScanVal) {
				s_tState = SW_UP;
			} else {
				s_tState = SW_DOWN;
			}
			break;

		case SW_UP:
			if(!chScanVal) {
				if(++ s_chTime >= 50) {	
					s_chTime = 0;
					s_tState = SW_LONG;
				}
			} else {
				s_chTime = 0;
				chReturnVal = SW_SHORT_USER;
				s_tState = SW_DOWN;
			}
			
			break;

		case SW_LONG:
			if(!chScanVal) {
				chReturnVal = SW_LONG_USER;
				s_tState = SW_REPEAT;
			} else {
				s_tState = SW_DOWN;
			}
			
			break;			

		case SW_REPEAT:
			if(!chScanVal) {
				if(++ s_chTime >= 10) {	
					s_chTime = 0;
					chReturnVal = SW_REPEAT_USER;
				}
			} else {
				s_chTime = 0;
				s_tState = SW_DOWN;
			}
			break;

		default:
			break;
	}
	return chReturnVal;
}

uint8_t rotary_sw_read(void)
{
	static rotary_sw_t s_tState = SW_S_CLICK;
	static uint8_t s_chTime = 0;
	uint8_t chScanVal = SW_NULL, chReturnVal = SW_NULL;

	chScanVal = rotary_sw_scan();
	switch (s_tState) {
		case SW_S_CLICK:
			if (SW_SHORT_USER == chScanVal) {
				s_chTime = 0;
				s_tState = SW_D_CLICK;
			} else {
				chReturnVal = chScanVal;
			}
			break;

		case SW_D_CLICK:
			if (SW_SHORT_USER == chScanVal) {
				chReturnVal = SW_D_CLICK_USER;
				s_tState = SW_S_CLICK;
			} else {
				if (++ s_chTime >= 25) {
					chReturnVal = SW_S_CLICK_USER;
					s_tState = SW_S_CLICK;
				}
			}
			break;
	}

	return chReturnVal;
}


/*-------------------------------END OF FILE-------------------------------*/

