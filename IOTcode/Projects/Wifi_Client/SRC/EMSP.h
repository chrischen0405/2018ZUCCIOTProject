#ifndef __EMSP_H
#define __EMSP_H

#include "stm32f4xx.h"


#define EMSP_MIN_PACKET_SIZE		(sizeof(struct emsp_header) + sizeof(struct emsp_footer))
#define EMSP_MAX_PACKET_SIZE		256
#define EMSP_MAX_PACKET_DATA_SIZE	(EMSP_MAX_PACKET_SIZE - sizeof(struct emsp_header) - sizeof(struct emsp_footer))

// Command code
#define EMSP_CMD_SET_CONFIG				0x0003	// set wifi module configuration, ip address, tcp/udp, port etc.
#define EMSP_CMD_GET_VER				0x006F	// get module hardware & firmware version
#define EMSP_CMD_RESET					0x0001	// reset module which is composed of STM32 and AW-GH320
#define EMSP_CMD_START					0x0005	// start module

#define EMSP_CMD_GET_CONFIG				0x0002	// get wifi module configuration, ip address, tcp/udp, port etc.
#define EMSP_CMD_SCAN_AP				0x0004	// scan ap
#define EMSP_CMD_SEND_DATA				0x0006	// send data to module, and the data will be send out use the WIFI
#define EMSP_CMD_RECV_DATA				0x0007	// receive data from module
#define EMSP_CMD_GET_STATUS				0x0008	// get module status
#define	EMSP_CMD_GET_RF_POWER	        0x000A    //Get the value of the RF power
#define	EMSP_CMD_SET_RF_POWER	        0x000B    //Set the value of the RF power


vs8 recv_cmd(void);
int send_cmd(unsigned short cmdcode, unsigned short result, void *data, unsigned short datalen);
 
#endif	

