#ifndef _HAL_RGB_LED_H
#define _HAL_RGB_LED_H

#include "include.h"

#define  R_MAX  255
#define  G_MAX  255
#define  B_MAX  255

#define GPIO_SCK_PIN    GPIO_Pin_8
#define GPIO_SDA_PIN    GPIO_Pin_9

#define SCL_LOW 	GPIO_WriteBit(GPIOB, GPIO_SCK_PIN, Bit_RESET)
#define SCL_HIGH 	GPIO_WriteBit(GPIOB, GPIO_SCK_PIN, Bit_SET)

#define SDA_LOW		GPIO_WriteBit(GPIOB, GPIO_SDA_PIN, Bit_RESET)
#define SDA_HIGH	GPIO_WriteBit(GPIOB, GPIO_SDA_PIN, Bit_SET)

//#define SCL_LOW 	GPIO_ResetBits(GPIOB,GPIO_SCK_PIN)
//#define SCL_HIGH 	GPIO_SetBits(GPIOB,GPIO_SCK_PIN)

//#define SDA_LOW		GPIO_ResetBits(GPIOB,GPIO_SDA_PIN)
//#define SDA_HIGH	GPIO_SetBits(GPIOB,GPIO_SDA_PIN)

void RGB_LED_Init(void);
void LED_R_Control(uint8_t R);
void LED_G_Control(uint8_t G);
void LED_B_Control(uint8_t B);
void LED_RGB_Control(uint8_t R,uint8_t B,uint8_t G);

#endif /*_HAL_RGB_LED_H*/

