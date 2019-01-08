/**
  ******************************************************************************
  * @file    xxx.c 
  * @author  
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
#include "Config.h"
#include "LIB_Config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
//#define PLL_SOURCE_HSI          // HSI (~8 MHz) used to clock the PLL, and the PLL is used as system clock source
                                //When the HSI is used as a PLL clock input, the maximum system clock
                                //frequency that can be achieved is 48 MHz.
#define PLL_SOURCE_HSE            // HSE (8MHz) used to clock the PLL, and the PLL is used as system clock source

//#define PLL_SOURCE_HSE_BYPASS   // HSE bypassed with an external clock (8MHz, coming from ) used to clock


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

static void device_init(void);
static void driver_init(void);
static void system_clock_set(void);
static void system_tick_init(void);
static void usart1_Init(uint32_t wBaudrate);
static void usart2_Init(uint32_t wBaudrate);
static void port_init(void);


/* Private functions ---------------------------------------------------------*/


/**
  * @brief  System initialization.
  * @param  None
  * @retval  None
  */
void system_init(void)
{
	disable_irq();
	
    device_init();
    driver_init();
	
	enable_irq();
}


/**
 * @brief 
 * @param 
 * @retval 
 */
static void device_init(void)
{
	system_clock_set();
	system_tick_init();
	usart1_Init(115200);
	usart2_Init(115200);
	port_init();
	delay_init(84000000);
}

/**
  * @brief  driver initialization.
  * @param  None
  * @retval None
  */
static void driver_init(void)
{

}


static void port_init(void) 
{
//	/*----------------------------------------------------------------------------------*/
    GPIO_InitTypeDef tGPIO;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE); 
#ifdef 	USE_UART1
	//USART
	tGPIO.GPIO_Pin = GPIO_Pin_6;	 	
  	tGPIO.GPIO_Mode = GPIO_Mode_AF;
  	tGPIO.GPIO_OType = GPIO_OType_PP;
  	tGPIO.GPIO_PuPd = GPIO_PuPd_UP;
  	tGPIO.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &tGPIO);

	tGPIO.GPIO_Pin = GPIO_Pin_7;		
  	tGPIO.GPIO_Mode = GPIO_Mode_AF;
  	tGPIO.GPIO_OType = GPIO_OType_PP;
  	tGPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	tGPIO.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &tGPIO);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART2);
#endif
	
#ifdef 	USE_UART2
	//USART
	tGPIO.GPIO_Pin = GPIO_Pin_2;	 	
  	tGPIO.GPIO_Mode = GPIO_Mode_AF;
  	tGPIO.GPIO_OType = GPIO_OType_PP;
  	tGPIO.GPIO_PuPd = GPIO_PuPd_UP;
  	tGPIO.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &tGPIO);

	tGPIO.GPIO_Pin = GPIO_Pin_3;		
  	tGPIO.GPIO_Mode = GPIO_Mode_AF;
  	tGPIO.GPIO_OType = GPIO_OType_PP;
  	tGPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	tGPIO.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &tGPIO);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
#endif
//	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);
//	RNG_Cmd(ENABLE);

}


static void system_clock_set(void)
{
	//F401 available
	__IO uint32_t StartUpCounter = 0, HSEStatus = 0;
/* FPU settings ------------------------------------------------------------*/
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
	SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
#endif
	/* Reset the RCC clock configuration to the default reset state ------------*/
	/* Set HSION bit */
	RCC->CR |= (uint32_t)0x00000001;

	/* Reset CFGR register */
	RCC->CFGR = 0x00000000;

	/* Reset HSEON, CSSON and PLLON bits */
	RCC->CR &= (uint32_t)0xFEF6FFFF;

	/* Reset PLLCFGR register */
	RCC->PLLCFGR = 0x24003010;

	/* Reset HSEBYP bit */
	RCC->CR &= (uint32_t)0xFFFBFFFF;

	/* Disable all interrupts */
	RCC->CIR = 0x00000000;

#if defined (DATA_IN_ExtSRAM) || defined (DATA_IN_ExtSDRAM)
	SystemInit_ExtMemCtl(); 
#endif /* DATA_IN_ExtSRAM || DATA_IN_ExtSDRAM */


#ifdef PLL_SOURCE_HSI
	/* At this stage the HSI is already enabled */
	/* Enable HSI */    
	RCC->CR |= ((uint32_t)RCC_CR_HSION);
	RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_PLLCFGR_PLLSRC));
    RCC->CFGR |= RCC_PLLCFGR_PLLSRC_HSI;
	
#else /* PLL_SOURCE_HSE_BYPASS or PLL_SOURCE_HSE */  
	 
	/* Enable HSE */    
	RCC->CR |= ((uint32_t)RCC_CR_HSEON);
#ifdef PLL_SOURCE_HSE_BYPASS
	RCC->CR |= ((uint32_t)RCC_CR_HSEBYP);
#endif   /* PLL_SOURCE_HSE_BYPASS */ 

	/* Wait till HSE is ready and if Time out is reached exit */
	do
	{
		HSEStatus = RCC->CR & RCC_CR_HSERDY;
		StartUpCounter++;
	} while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

	if ((RCC->CR & RCC_CR_HSERDY) != RESET)
	{
		HSEStatus = (uint32_t)0x01;
	}
	else
	{
		HSEStatus = (uint32_t)0x00;
	}

	if (HSEStatus == (uint32_t)0x01)
	{
		/* Select regulator voltage output Scale 1 mode */
		RCC->APB1ENR |= RCC_APB1ENR_PWREN;
		PWR->CR |= PWR_CR_VOS;
  	} 
	else
	{ /* If HSE fails to start-up, the application will have wrong clock
	 configuration. User can add here some code to deal with this error */
	}
#endif /*PLL_SOURCE_HSI*/ 
	/* HCLK = SYSCLK / 1*/
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

	/* PCLK2 = HCLK / 2*/
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;
    
    /* PCLK1 = HCLK / 4*/
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;

	/* Configure the main PLL */
	//RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) |
    //               (RCC_PLLCFGR_PLLSRC_HSE) | (PLL_Q << 24);
    
#if defined(PLL_SOURCE_HSI)
	RCC->PLLCFGR = 16 | (288 << 6) | (((4 >> 1) -1) << 16) |  //SYSCLK = HCLK = 72M, PCLK1 = 36M, PCLK1 = 72M
                   (RCC_PLLCFGR_PLLSRC_HSI) | (6 << 24);
#elif defined(PLL_SOURCE_HSE) || defined(PLL_SOURCE_HSE_BYPASS)
	RCC->PLLCFGR = 8 | (288 << 6) | (((4 >> 1) -1) << 16) |  //SYSCLK = HCLK = 72M, PCLK1 = 36M, PCLK1 = 72M
                   (RCC_PLLCFGR_PLLSRC_HSE) | (6 << 24);
#endif
	
    /* Enable the main PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till the main PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }

    /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
	FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_2WS;

	/* Select the main PLL as system clock source */
	RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
	RCC->CFGR |= RCC_CFGR_SW_PLL;

	/* Wait till the main PLL is used as system clock source */
	while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL)
	{
	}
	
	SystemCoreClockUpdate();
}

/**
  * @brief  Initialize and start the system stick timer and its interrupt.
  * @param  None
  * @retval None
  */
static void system_tick_init(void)
{
	RCC_ClocksTypeDef tRCC;
	
	RCC_GetClocksFreq(&tRCC);  
	delay_init(tRCC.HCLK_Frequency);
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	;
}

/**
  * @brief  Initializes the USARTx.
  * @param  None
  * @retval None
  */
static void usart1_Init(uint32_t wBaudrate)
{
#ifdef	USE_UART1
	/* USARTx configured as follow:
        - BaudRate =  Baudrate
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
        */

	USART_InitTypeDef tUSART;

	/* config USART2 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* USART2 mode config */
	tUSART.USART_BaudRate = wBaudrate;
	tUSART.USART_WordLength = USART_WordLength_8b;
	tUSART.USART_StopBits = USART_StopBits_1;
	tUSART.USART_Parity = USART_Parity_No;
	tUSART.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		
	tUSART.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &tUSART);
	
	USART_Cmd(USART1, ENABLE);
#endif	
}

/**
  * @brief  Initializes the USARTx.
  * @param  None
  * @retval None
  */
static void usart2_Init(uint32_t wBaudrate)
{
#ifdef	USE_UART2
   /* USARTx configured as follow:
        - BaudRate =  Baudrate
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
        */

	USART_InitTypeDef tUSART;

	/* config USART2 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* USART2 mode config */
	tUSART.USART_BaudRate = wBaudrate;
	tUSART.USART_WordLength = USART_WordLength_8b;
	tUSART.USART_StopBits = USART_StopBits_1;
	tUSART.USART_Parity = USART_Parity_No;
	tUSART.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		
	tUSART.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART2, &tUSART);
	
	USART_Cmd(USART2, ENABLE);
#endif	
}


/*-------------------------------END OF FILE-------------------------------*/
