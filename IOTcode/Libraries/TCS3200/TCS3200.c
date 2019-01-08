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
#include "TCS3200.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief Initializes TCS3200
 * @param None
 * @retval None
 */

void tcs3200_init(void)
{
	TCS3200_EN_R();
}

/**
* @brief Calibrates TCS3200
* @param pdwFreq: pointer to a variable that contains the value of frequency of R,G,B color from TCS3200 output. 
         pfBuffer: pointer to a buffer that saved calibrated value.
* @retval true: calibration is successful.
          false: failed in calibration.
*/
bool tcs3200_calibration(uint32_t *pdwFreq, float *pfBuffer)
{
	static uint8_t s_chState = TCS3200_GET_R;
	float fFreqVal;

	if(NULL == pdwFreq || NULL == pfBuffer) {  //are the pointers NULL?
		return false;
	}
	
	fFreqVal = 255 / (float)(*pdwFreq); 
	switch(s_chState) {
		case TCS3200_GET_R:
			*pfBuffer = fFreqVal;
			TCS3200_EN_G();
			s_chState = TCS3200_GET_G;
			break;
			
		case TCS3200_GET_G:
			*(pfBuffer + 1) = fFreqVal;
			TCS3200_EN_B();
			s_chState = TCS3200_GET_B;
			break;
			
		case TCS3200_GET_B:
			*(pfBuffer + 2) = fFreqVal;
			TCS3200_EN_R();
			return true;
	}

	return false;
}


/**
 * @brief outputs the values of R,G,B color respectively.
 * @param pchBuffer pointer to a buffer that contains the values of R,G,B color.
 * @retval true output is successful.
           false output is faileed.
 */
bool tcs3200_output_color(uint8_t *pchBuffer)
{
	static bool s_bFlag = true;
	static uint8_t s_chState = TCS3200_GET_R;
	static float s_fBuffer[3];
	uint32_t dwFrequency = 0;

	if(NULL == pchBuffer) {  //is pointer NULL ?
		return true;
	}
	
	dwFrequency = TCS3200_CAP_VAL();   //captures the value of frequency of TCS3200.
	
	if(0 == dwFrequency) {  //is there have no frequency come in.
		return true;
	}
	
	dwFrequency = TCS3200_GET_FREQ(dwFrequency);

	if(s_bFlag) {
		if(tcs3200_calibration(&dwFrequency, &s_fBuffer[0])) {
			s_bFlag = false;
		}
	} else if(!s_bFlag) {
		switch(s_chState) {
			case TCS3200_GET_R:
				*pchBuffer = (float)dwFrequency * s_fBuffer[0];
				TCS3200_EN_G();
				s_chState = TCS3200_GET_G;
				break;
				
			case TCS3200_GET_G:
				*(pchBuffer + 1) = (float)dwFrequency * s_fBuffer[1];
				TCS3200_EN_B();
				s_chState = TCS3200_GET_B;
				break;
				
			case TCS3200_GET_B:
				*(pchBuffer + 2) = (float)dwFrequency * s_fBuffer[2];
				TCS3200_EN_R();
				s_chState = TCS3200_GET_R;
				break;
		}
	}

	return s_bFlag;
}

/*-------------------------------END OF FILE-------------------------------*/

