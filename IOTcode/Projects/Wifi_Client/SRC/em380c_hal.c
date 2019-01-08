#include "em380c_hal.h"

vu32 MS_TIMER;
vu32 TICK_TIMER;
vu32 SEC_TIMER;
unsigned long startsec = 0;
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

int buff_pos;
u8 EM380RxBuffer[512] = {0};
//bool use_hardware_flow_control;
vu32 pulse;

uint32_t UART_BaudRate[]={9600,19200,38400,57600,115200};
uint16_t UART_WordLength[]={USART_WordLength_8b,USART_WordLength_9b};
uint16_t UART_StopBits[]={USART_StopBits_1,USART_StopBits_0_5,USART_StopBits_2,USART_StopBits_1_5};
uint16_t UART_Parity[]={USART_Parity_No,USART_Parity_Even,USART_Parity_Odd};
uint16_t UART_CTSRTS[]={USART_HardwareFlowControl_None,USART_HardwareFlowControl_RTS_CTS};


//static __IO uint32_t Timingdelay_ms;
//void delay_ms(__IO uint32_t nTime);

void SET_STATUS_PIN(void)
{
  GPIO_SetBits(GPIOE, GPIO_Pin_6);
}
void RESET_STATUS_PIN(void)
{
  GPIO_ResetBits(GPIOE, GPIO_Pin_6);
}		   

void SET_WAKEUP_PIN(void)
{
  GPIO_SetBits(GPIOE, GPIO_Pin_8);
}
void RESET_WAKEUP_PIN(void)
{
  GPIO_ResetBits(GPIOE, GPIO_Pin_8);
}

vs8 READ_INT_PIN(void)
{
   return(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5));
}

vs8 EM380C_HAL_Init(uint32_t BaudRate,uint16_t WordLength,uint16_t StopBits,uint16_t Parity,uint16_t CTSRTS)
{  	
   vs8 ret = -1;
   buff_pos = 0;
//   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1 , ENABLE);

//  /* Configure USART */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);		  /* Configure USART RX PIN*/
//  
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);		 /* Configure USART TX PIN*/


//  if(CTSRTS)
//  {
//	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	  GPIO_Init(GPIOA, &GPIO_InitStructure);		  /* Configure USART CTS PIN*/
//	  
//	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
//	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	  GPIO_Init(GPIOA, &GPIO_InitStructure);		 /* Configure USART RTS PIN*/
//  }

//  /* Configure the NVIC Preemption Priority Bits */  
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
//  
//  /* Enable the USARTy Interrupt */
//  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);

//  USART_InitStructure.USART_BaudRate = UART_BaudRate[BaudRate];
//  USART_InitStructure.USART_WordLength = UART_WordLength[WordLength];
//  USART_InitStructure.USART_StopBits = UART_StopBits[StopBits];
//  USART_InitStructure.USART_Parity = UART_Parity[Parity];
//  USART_InitStructure.USART_HardwareFlowControl = UART_CTSRTS[CTSRTS];
//  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		 /* Configure USART parameters*/
//  USART_Init(USART1, &USART_InitStructure);

//  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

//  USART_Cmd(USART1, ENABLE);											 /* ENABLE USART*/

//  /* Configure status /RESET /INT and Wakeup PIN */
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);


//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_8 |GPIO_Pin_9;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  GPIO_Init(GPIOE, &GPIO_InitStructure);		 /* Configure Status, /RESET(PE9) and Wakeup PIN*/

//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_Init(GPIOE, &GPIO_InitStructure);		 /* Configure /INT PIN*/

//  RESET_STATUS_PIN();		    /* EM380C start in configuration mode*/
//  SET_WAKEUP_PIN();				/* WAKEUP pin should be high*/
// 
//  GPIO_ResetBits(GPIOA, GPIO_Pin_12);	/* CTX=0*/
//   

//  GPIO_ResetBits(GPIOE, GPIO_Pin_9);
//  delay_ms(1);
//  GPIO_SetBits(GPIOE, GPIO_Pin_9);
//  delay_ms(750);
//  if(!READ_INT_PIN())
	 ret = 0;
  return ret;
}

void UART_send_buf(u8 *buf, int len)
{
	int i; 
	buff_pos=0;   
	for (i = 0; i < len; i++)
	  {
       USART_SendData(USART2,buf[i]);
	   while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE));
	  }	
}

int UART_receive_buf(u8 *buf)
{   memcpy(buf, EM380RxBuffer, buff_pos);
	return buff_pos;
}


void USART2_IRQHandler(void)
//{ if(EM380C_mode == config_mode);
  {
	buff_pos &= 0xff;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
	/* Read one byte from the receive data register */
	EM380RxBuffer[buff_pos++] = USART_ReceiveData(USART2);
	}
//  }
  /*your application */
//  else
//  {
//  }
}

