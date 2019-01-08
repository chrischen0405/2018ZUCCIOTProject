#ifndef __EMSP_API_H
#define __EMSP_API_H

#include "stm32f4xx.h"
#include "delay.h"

#define STATION                            0x1
#define SOFTAP                             0x2
#define STA_AP                             0x3

#define	TCP_Server                         0x0
#define TCP_Client                         0x1

#define DHCP_Disable                       0x0
#define DHCP_Enable	                       0x1

#define TCP_mode                           0x0
#define UDP_mode	                       0x1

#define BaudRate_9600                      0x0
#define BaudRate_19200                     0x1
#define BaudRate_38400                     0x2 
#define BaudRate_57600                     0x3 
#define BaudRate_115200                    0x4

#define buffer_0bytes                      0x0
#define buffer_8bytes                     0x1
#define buffer_16bytes	                   0x2
#define buffer_32bytes	                   0x3
#define buffer_64bytes	                   0x4
#define buffer_128bytes                    0x5
#define buffer_256bytes                    0x6 
 
#define WordLength_8b                      0x0

#define StopBits_1                         0x0                         
#define StopBits_0_5                       0x1                         
#define StopBits_2                         0x1                         
#define StopBits_1_5                       0x1                         

#define Parity_No                          0x0                         
#define Parity_Even                        0x1                         
#define Parity_Odd                         0x2  

#define HardwareFlowControl_None           0x0                         
#define HardwareFlowControl_RTS_CTS        0x1

#define 	WEP			0x0
#define 	WPA_WPA2 	0x1
#define 	None		0x2
#define 	WEP_Hex		0x3
#define 	Auto		0x4

                         
typedef enum
{	TCP_stop      = 0x0,
    TCP_listening = 0x1,
	TCP_connected = 0x2,
	TCP_unknown   = 0xFF,
} EM380C_TCPstatus_TypeDef;

typedef enum
{
  wifi_disconnected = 0x0,
  wifi_connected	= 0x5,
  wifi_unknown      = 0xFF,
} EM380C_WiFistatus_TypeDef;

typedef enum
{
  config_mode = 0x0,
  DTU_mode = 0x1,
} EM380C_mode_TypeDef;

typedef struct
{
	// WIFI
	u8 wifi_mode;		// Wlan802_11IBSS(0), Wlan802_11Infrastructure(1)
	u8 wifi_ssid[32];	// 
	u8 wifi_wepkey[16];	// 40bit and 104 bit
	u8 wifi_wepkeylen;	// 5, 13

	// TCP/IP
	u8 local_ip_addr[16];		// if em380 is server, it is server's IP;	if em380 is client, it is em380's default IP(when DHCP is disable)
	u8 remote_ip_addr[16];		// if em380 is server, it is NOT used;		if em380 is client, it is server's IP
	u8 net_mask[16];	// 255.255.255.0
	u8 gateway[16];		// 192.168.1.1
	u8 portH;			// High Byte of 16 bit
	u8 portL;			// Low Byte of 16 bit
	u8 connect_mode;	// 0:server  1:client
	u8 use_dhcp;		// 0:disale, 1:enable
	u8 use_udp;			// 0:use TCP,1:use UDP

	// COM
	u8 UART_buadrate;	// 0:9600, 1:19200, 2:38400, 3:57600, 4:115200
	u8 DMA_buffersize;	// 0:No, 1:8, 2:16, 3:32, 4:64, 5:128
	u8 use_CTS_RTS;		// 0:disale, 1:enable
	u8 parity;			// 0:none, 1:even parity, 2:odd parity
	u8 data_length;		// 0:8
	u8 stop_bits;		// 0:1, 1:0.5, 1:2, 1:1.5

	// DEVICE
	u8	io1;
	u8	security_mode;	//0: WEP 	1: WPA/WPA2 PSK 	2: None	3: WEP Hex	4: Auto
	u8 	wpa_key[32];		// 0 - 255
} EM380C_parm_TypeDef;

typedef struct
{
  EM380C_TCPstatus_TypeDef TCPstatus;
  EM380C_WiFistatus_TypeDef  WiFistatus;
} EM380C_status_TypeDef;

typedef struct 
{
  u8 Min_Power;
  u8 Max_Power;
  u8 Cur_Power;
} EM380C_RF_POWER_TypeDef;

typedef struct 
{
  char   AP_NAME[20];
  float     AP_signal;
} EM380C_APLst_TypeDef;

#define EM380ERROR -1

vs8 EM380C_Init(uint32_t BaudRate,uint16_t WordLength,uint16_t StopBits,uint16_t Parity,uint16_t CTSRTS,uint16_t buffer);
vs8 EM380C_Get_ver(u32* version);
vs8 EMSP_Get_status(EM380C_status_TypeDef* EM380C_status);
vs8 EM380C_Get_APList(EM380C_APLst_TypeDef* EM380C_APLst);
vs8 EM380C_Get_Config(EM380C_parm_TypeDef* EM380C_Parm);
vs8 EM380C_Set_Config(EM380C_parm_TypeDef* EM380C_Parm);
vs8 EM380C_Get_RF_POWER(EM380C_RF_POWER_TypeDef* RF_POWER);
vs8 EM380C_Set_RF_POWER(EM380C_RF_POWER_TypeDef* RF_POWER);
u32 EM380C_Send_Data(u8* Data,u32 len);
u32 EM380C_Recv_Data(u8* Data);
vs8 EM380C_Set_Mode(EM380C_mode_TypeDef mode);
vs8 EM380C_Startup(void);
vs8 EM380C_Reset(void);
vs8 EM380C_Sleep(void);
vs8 EM380C_Wakeup(void);


#endif
