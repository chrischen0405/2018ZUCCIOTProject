#ifndef __EM380C_HAL_H
#define __EM380C_HAL_H

#include "stm32f4xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>	


vs8 EM380C_HAL_Init(uint32_t BaudRate,uint16_t WordLength,uint16_t StopBits,uint16_t Parity,uint16_t CTSRTS);
void Delay(__IO uint32_t nTime);
void SET_STATUS_PIN(void);
void RESET_STATUS_PIN(void);
void SET_WAKEUP_PIN(void);
void RESET_WAKEUP_PIN(void);
vs8 READ_INT_PIN(void);
void UART_send_buf(u8 *buf, int len);
int UART_receive_buf(u8 *buf);


#endif

