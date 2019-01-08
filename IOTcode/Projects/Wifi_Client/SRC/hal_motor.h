#ifndef _HAL_MOTOR_H
#define _HAL_MOTOR_H

#include <stdio.h>


#define Motor_stop        0
#define Motor_Forward     1
#define Motor_Reverse     2

#define MOTOR_ARR 899 //8kHZ
#define MOTOR_MAX 100
#define MOTOR_MIN 0

#define TIM3_COUNTER_CLK 10000000   //?????(Hz)
#define TIM3_OUTPUT_CLK   50000      //PWM??(Hz)
#define TIM3_CLOCK_DIV   0
#define DUTYCYCLE        50         //?????
 
#define PRESCALER        ((SystemCoreClock / TIM3_COUNTER_CLK) - 1)
#define ARR              ((TIM3_COUNTER_CLK / TIM3_OUTPUT_CLK) - 1)
#define DUTYCYCLE_PRAM   (((DUTYCYCLE+1) * ARR) / 100)


void Motor_Init(void);
void Motor_status(unsigned int status);

#endif /*_HAL_MOTOR_H*/


