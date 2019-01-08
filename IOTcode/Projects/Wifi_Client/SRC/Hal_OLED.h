#ifndef _HAL_OLED_H
#define _HAL_OLED_H

#include "include.h"

#define OLED_CMD    0
#define OLED_DAT    1
#define	OLED_MAX_COL	128

extern const uint8_t OLED_Asc2_1206[95][12];
extern const uint8_t OLED_Asc2_1608[95][16];

/*************************************************************
*1 > VCC    -----  3.3V       --> ZUCC
*2 > GBD    -----  GND
*3 > NC     -----  NC
*4 > DIN    -----  PA7 --> D11 --> PA7
*5 > SCLK   -----  PA5 --> D13 --> PA5
*6 > CS     -----  PB6 --> D10 --> PB6
*7 > D/C    -----  PC7 --> D9  --> PC7
*8 > RES    -----  PA9 --> D8  --> PA9
***************************************************************/
#define OLED_SPIx          		SPI1    
#define OLED_SPIx_CLK        	RCC_APB2Periph_SPI1
#define OLED_SPIx_CLK_Cmd		RCC_APB2PeriphClockCmd

#define OLED_SPIx_MOSI_PIN    	GPIO_Pin_7
#define OLED_SPIx_MOSI_PORT   	GPIOA
#define OLED_SPIx_MOSI_CLK     	RCC_AHB1Periph_GPIOA	//RCC_APB2Periph_GPIOA

// NOT USED FOR OLED
#define OLED_SPIx_MISO_PIN    	GPIO_Pin_6
#define OLED_SPIx_MISO_PORT    	GPIOA
#define OLED_SPIx_MISO_CLK   	RCC_AHB1Periph_GPIOA	//RCC_APB2Periph_GPIOA

#define OLED_SPIx_SCK_PIN		GPIO_Pin_5
#define OLED_SPIx_SCK_PORT 		GPIOA
#define OLED_SPIx_SCK_CLK  	 	RCC_AHB1Periph_GPIOA	//RCC_APB2Periph_GPIOA

#ifdef STUNO
#define OLED_CS_PORT		GPIOA
#define OLED_CS_CLK			RCC_APB2Periph_GPIOA
#define OLED_CS_PIN    		GPIO_Pin_15

#define OLED_DC_PORT		GPIOB
#define OLED_DC_CLK   		RCC_APB2Periph_GPIOB
#define OLED_DC_PIN   	 	GPIO_Pin_11
#endif

#ifdef Xnucleo
#define OLED_CS_PORT       	GPIOB
#define OLED_CS_CLK         RCC_APB2Periph_GPIOB
#define OLED_CS_PIN        	GPIO_Pin_6

#define OLED_DC_PORT     	GPIOC
#define OLED_DC_CLK       	RCC_APB2Periph_GPIOC
#define OLED_DC_PIN      	GPIO_Pin_7
#endif

// ZUCC_IOT
#if 1
#define OLED_CS_PORT       	GPIOB
#define OLED_CS_CLK         RCC_AHB1Periph_GPIOB	//RCC_APB2Periph_GPIOB
#define OLED_CS_PIN        	GPIO_Pin_6

#define OLED_DC_PORT     	GPIOC
#define OLED_DC_CLK       	RCC_AHB1Periph_GPIOC	//RCC_APB2Periph_GPIOC
#define OLED_DC_PIN      	GPIO_Pin_7
#endif

#define OLED_RES_PORT   	GPIOA
#define OLED_RES_CLK        RCC_AHB1Periph_GPIOA	//RCC_APB2Periph_GPIOA
#define OLED_RES_PIN       	GPIO_Pin_9

// SET AND CLEAR PIN
#define OLED_CS_SET()    	GPIO_WriteBit(OLED_CS_PORT, OLED_CS_PIN, Bit_SET)    
#define OLED_CS_RESET()		GPIO_WriteBit(OLED_CS_PORT, OLED_CS_PIN, Bit_RESET)		

#define OLED_DC_SET()    	GPIO_WriteBit(OLED_DC_PORT, OLED_DC_PIN, Bit_SET)    
#define OLED_DC_RESET()		GPIO_WriteBit(OLED_DC_PORT, OLED_DC_PIN, Bit_RESET)		

#define OLED_RES_SET()   	GPIO_WriteBit(OLED_RES_PORT, OLED_RES_PIN, Bit_SET)    
#define OLED_RES_RESET()	GPIO_WriteBit(OLED_RES_PORT, OLED_RES_PIN, Bit_RESET)		

void OLED_Init(void);
void OLED_DisplayOn(void);
void OLED_DisplayOff(void);
void OLED_Refresh_Gram(void);
void OLED_Clear(uint8_t Color);
void OLED_DrawPoint(uint8_t Xpos,uint8_t Ypos,uint8_t Fill);
void OLED_ShowChar(uint8_t X, uint8_t Y, uint8_t Chr, uint8_t Size, uint8_t Mode);
void OLED_ShowString(uint8_t X, uint8_t Y, const uint8_t *Str);

#endif /*_HAL_OLED_H*/


