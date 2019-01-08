#include "LIB_Config.h"
#include "Hal_led.h"
#include "Hal_key.h"


void Hal_Init(void)
{
	LED_GPIO_Init();
	printf("LED Init OK\r\n");
//	TIM3_Int_Init(7199,9);   //1ms SystemTimeCount + 1
	printf("SystemTime Init OK\r\n");
	KEY_GPIO_Init();
//	printf("KEY Init OK\r\n");
//	RGB_LED_Init();
//	printf("RGB LED Init OK\r\n");
//	Motor_Init();
//	printf("Motor Init OK\r\n");
//	Delay_Init(72);
//	DHT11_Init();
//	printf("DHT11 Init OK\r\n");
//	IR_Init();
//	printf("IR Init OK\r\n");
//	OLED_Init();
//	OLED_ShowString(35, 0, (uint8_t *)"IOTKIT");
//	OLED_ShowString(0, 32, (uint8_t *)"http://www.zucc.edu.cn");	
}

int main(void) 
{
	system_init();
	Hal_Init();
	
	while(1)
	{
//		if (ReadKeyValue() != 0)
		if (Get_Key() != 0)
			delay_ms(1000);
		
		LED_ON(LED2);
		delay_ms(50);	
		LED_OFF(LED2);
		delay_ms(100);	
	}
}

/*-------------------------------END OF FILE-------------------------------*/

