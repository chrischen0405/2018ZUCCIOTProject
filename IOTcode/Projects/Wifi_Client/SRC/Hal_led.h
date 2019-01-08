#ifndef _HAL_LED_H
#define _HAL_LED_H

#include <stdio.h>
#include <stm32f4xx.h>

//#define GPIO_LED1_CLK    RCC_AHB1Periph_GPIOB
//#define GPIO_LED1_PORT   GPIOB
//#define GPIO_LED1_PIN    GPIO_Pin_15

#define GPIO_LED2_PIN    GPIO_Pin_5

//#define LED1             0X01
#define LED2             0X02

void LED_GPIO_Init(void);
void LED_ON(uint8_t LEDNUM);
void LED_OFF(uint8_t LEDNUM);

#endif /*_HAL_LED_H*/

