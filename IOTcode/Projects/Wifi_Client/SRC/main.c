#include "LIB_Config.h"
#include "Hal_rgb_led.h"
#include "Hal_uart.h"
#include "EMSP_API.h"
#include "hal_infrared.h"
#include "hal_temp_hum.h"
#include "hal_motor.h"
#include "Hal_OLED.h"
#include "hal_led.h"
#include "em380c_hal.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>	

#define	STARTFLAG	0x31501084

typedef struct {
	unsigned char ID;
	unsigned int V1;
	unsigned int V2;
	unsigned int V3;
	unsigned short VNot;
	unsigned char none;
}__attribute__((packed)) sensor_data_frame;

typedef struct {
	unsigned char START_F0;
	unsigned char START_F1;
	unsigned char START_F2;
	unsigned char START_F3;
	unsigned char ID;
	unsigned char	sensor_num;
	unsigned char	heartbeat;
	sensor_data_frame	sensor_dev1;
	sensor_data_frame	sensor_dev2;
	sensor_data_frame	sensor_dev3;
	unsigned char chkorl;
}__attribute__((packed)) device_comm_frame;

typedef struct{
	int V1;
	int V2;
	int V3;
} __attribute__((packed)) receive_Sensor_data ;

typedef struct{
	unsigned char START_F0;
	unsigned char START_F1;
	unsigned char START_F2;
	unsigned char START_F3;
	
	unsigned char sensor_num;
	unsigned char heartbeat;
	
	receive_Sensor_data sensor_dev1;
	receive_Sensor_data sensor_dev2;
	receive_Sensor_data sensor_dev3;
	
	unsigned char chkorl;
}__attribute__((packed)) receive_IOT;


char const SERVER_IP[] = { "" }; /* 目标IP地址 */
char const GATEWAY_IP[] = { "" };
char const WIFI_SSID[] = { "" };/*wifi名称*/
char const WIFI_KEY[] = { "" };/*wifi密码*/
uint16_t SERVER_PORT = 8888;/*端口号*/

char const LOCAL_IP[] = { "" }; /* 本地IP地址 */

extern int buff_pos;
extern bool	bInfrared;

u8 rcv_buf_from_server[256];
receive_Sensor_data sensor_data[10];

int heartbeat;
char content[7];


/* Private variables ---------------------------------------------------------*/
EM380C_parm_TypeDef parm;
u32 version;

void setup_wifi(void)
{
	
	EMW3612_CMD;
	delay_ms(200);
	
	LED_RGB_Control(0, 0, 10);
	delay_ms(200);
	EM380C_Init(BaudRate_115200, WordLength_8b, StopBits_1, Parity_No,
			HardwareFlowControl_None, buffer_128bytes); //if you dont know EM380C's baudrate,you should use	EM380C_scan_Init(...)
	delay_ms(400);
	delay_ms(400);

	while (EM380C_Get_ver(&version) == EM380ERROR)
		;
//		ret = EM380C_Get_ver(&version);
	LED_RGB_Control(10, 0, 0);
	delay_ms(200);

	parm.wifi_mode = STATION;
	strcpy((char*) parm.wifi_ssid, WIFI_SSID);
	strcpy((char*) parm.wifi_wepkey, WIFI_KEY);
	parm.wifi_wepkeylen = strlen(WIFI_SSID);

	strcpy((char*) parm.local_ip_addr, LOCAL_IP);
	strcpy((char*) parm.remote_ip_addr, SERVER_IP);
	strcpy((char*) parm.gateway, GATEWAY_IP);
	strcpy((char*) parm.net_mask, "255.255.255.0");
	parm.portH = SERVER_PORT >> 8;
	parm.portL = SERVER_PORT & 0xff;
	parm.connect_mode = TCP_Client;
	parm.use_dhcp = DHCP_Enable;
	parm.use_udp = TCP_mode;
	parm.UART_buadrate = BaudRate_115200;
	parm.DMA_buffersize = buffer_64bytes;
	parm.use_CTS_RTS = HardwareFlowControl_None;
	parm.parity = Parity_No;
	parm.data_length = WordLength_8b;
	parm.stop_bits = StopBits_1;

	parm.io1 = None;
	strcpy((char*) parm.wpa_key, WIFI_KEY);
	parm.security_mode = Auto;

	while (EM380C_Set_Config(&parm) == EM380ERROR)
		;									 //EMSP API, set EM380C status
	delay_ms(400);
	LED_RGB_Control(0, 10, 0);

	while (EM380C_Startup() == EM380ERROR)
		;
	delay_ms(400);
	LED_RGB_Control(5, 5, 5);

//	EM380C_Reset();	
	delay_ms(400);	
	EMW3612_DAT;
	delay_ms(400);
}

unsigned short calcheckcode(unsigned char *dat)
{
	int n;
	unsigned short chk = 0;

	for (n = 0; n < sizeof(device_comm_frame) - 2; n++) {
		chk += *dat;
		dat++;
	}
	return chk;
}

unsigned char HexToChar(unsigned char bChar){
  if((bChar>=0x30)&&(bChar<=0x39))
    bChar -= 0x30;
  else if((bChar>=0x41)&&(bChar<=0x46))
    bChar -= 0x37;
  else if((bChar>=0x61)&&(bChar<=0x66))
    bChar -= 0x57;
  else bChar = 0xff;
    return bChar;
}

int process_rcv_data()
{
	int datalen;
	int i;
	vs8 ret = -1;
	receive_IOT *prcv_buf = (receive_IOT *)rcv_buf_from_server;

    datalen = UART_receive_buf(rcv_buf_from_server);

	if (!((rcv_buf_from_server[0] == 0x31) &&
		(rcv_buf_from_server[1] == 0x50) &&
		(rcv_buf_from_server[2] == 0x10) &&
		(rcv_buf_from_server[3] == 0x84))){
		ret = -1;
		goto done;
	}
	
	memcpy(sensor_data, (u8 *)&prcv_buf->sensor_dev1, sizeof(sensor_data_frame) * (prcv_buf->sensor_num));
	heartbeat = rcv_buf_from_server[5];
	for(i = 0;i < 7; i++){
		content[i] = 0;
		if(rcv_buf_from_server[42+i] != 0){
			content[i] = rcv_buf_from_server[42+i];
		}
	}
	ret = 0;
done:
	return ret;
}


void Hal_Init(void)
{
	UARTx_Init();
	RGB_LED_Init();
	IR_Init();
	DHT11_Init();
	LED_GPIO_Init();
	Motor_Init();
	Motor_status(0);
	LED_OFF(LED2);
}

int main(void)
{
	int cnt = 1;
	int temperature, humidity;
	int R=10, G=10, B=10;
	int Moto;
	int LED_Stat=0;
	char str_heartbeat[20], str_rgb[20], str_moto[20];
	u8 buf[128];
	device_comm_frame tcpip_data;

	system_init();
	Hal_Init();

	setup_wifi();
	IR_Handle();
	
	OLED_Init();
	OLED_Clear(0x0);
	OLED_ShowString(30,10,(const uint8_t *)"cwj");
	OLED_ShowString(30,35,(const uint8_t *)"31501084");
	delay_ms(1000);
	
	tcpip_data.START_F0 = (STARTFLAG >> 24) & 0xff;
	tcpip_data.START_F1 = (STARTFLAG >> 16) & 0xff;
	tcpip_data.START_F2 = (STARTFLAG >> 8) & 0xff;
	tcpip_data.START_F3 = (STARTFLAG >> 0) & 0xff;
	tcpip_data.ID = 32;
	tcpip_data.chkorl = 0;
	
	while (cnt++) {
		tcpip_data.heartbeat++;
		tcpip_data.sensor_num = 3;
		
		//RGB
		tcpip_data.sensor_dev1.ID = 1;
		tcpip_data.sensor_dev1.V1 = R;
		tcpip_data.sensor_dev1.V2 = G;
		tcpip_data.sensor_dev1.V3 = B;
		
		//temp&hum
		tcpip_data.sensor_dev2.ID = 2;
		DHT11_Read_Data((u8*)&temperature, (u8*)&humidity);
		tcpip_data.sensor_dev2.V1 = temperature;
		tcpip_data.sensor_dev2.V2 = humidity;
		
		//IR
		tcpip_data.sensor_dev3.ID = 3;
		IR_Handle();
		if(bInfrared)
		  tcpip_data.sensor_dev3.V1 = 1;
		else
			tcpip_data.sensor_dev3.V1 = 0;
		
		tcpip_data.chkorl = calcheckcode((unsigned char *) &tcpip_data);

		UART_send_buf((uint8_t *)&tcpip_data,sizeof(device_comm_frame));
		delay_ms(500);
		if (process_rcv_data() == 0){

			//RGB
			R =  sensor_data[0].V1;
	    G =  sensor_data[0].V2;
			B =  sensor_data[0].V3;
			LED_RGB_Control(R, G, B);
			
			
			//MOTOR
			Moto = sensor_data[1].V1;
			Motor_status(Moto);
			
			//LED
			LED_Stat = sensor_data[2].V1;
			if(LED_Stat == 1)
				LED_ON(LED2);
			else
				LED_OFF(LED2);

			
			sprintf(str_heartbeat, "Heartbeat:%d", heartbeat);
			sprintf(str_rgb, "R:%d G:%d B:%d", R, G ,B);
			sprintf(str_moto, "Motor:%d LED:%d", Moto, LED_Stat);
			OLED_Clear(0x0);
	    OLED_ShowString(2,0,(const uint8_t *)str_heartbeat);
			OLED_ShowString(2,15,(const uint8_t *)str_rgb);
			OLED_ShowString(2,30,(const uint8_t *)str_moto);
			OLED_ShowString(2,45,(const uint8_t *)"Content:");
			OLED_ShowString(70,45,(const uint8_t *)content);
	    delay_ms(1000);
		}
		else{	//ERROR
			OLED_Clear(0x0);
	    OLED_ShowString(25,10,(const uint8_t *)"No Data");
			OLED_ShowString(30,40,(const uint8_t *)"31501084");
	    delay_ms(1000);
		}
		

		
		/*switch (cnt % 3) {
		case 0:
			LED_RGB_Control(10, 0, 0);
			break;
		case 1:
			LED_RGB_Control(0, 10, 0);
			break;
		case 2:
			LED_RGB_Control(0, 0, 10);
			break;
		default:
			break;
		}*/
	}
}

/*-------------------------------END OF FILE-------------------------------*/

