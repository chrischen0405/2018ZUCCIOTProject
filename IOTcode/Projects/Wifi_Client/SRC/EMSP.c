#include "EMSP.h"
#include "em380c_hal.h"
#include "EMSP_API.h"
#include "delay.h"

__packed struct emsp_header {
	u16 cmdcode;	// Command code
	u16 size;		// size of the packet, including the header and footer
	u16 result;		// Result code, set by module.
					// This field is used only the response packet. Set 0 in the request packet
	u16 checksum;	// check sum of the header
};

__packed struct emsp_footer {
	u16 checksum;	// check sum of the packet body( not include the header)
};


u8 sendbuf[EMSP_MAX_PACKET_SIZE];
u8 recvbuf[EMSP_MAX_PACKET_SIZE];

u8 senddata[200];
u8 *recvdata;
u8 recvlen;
u8 buffer_bytes; 

u16 calc_sum(void *data, u32 len)
{
	u32	cksum=0;
	__packed u16 *p=data;

	while (len > 1)
	{
		cksum += *p++;
		len -=2;
	}
	if (len)
	{
		cksum += *(u8 *)p;
	}
	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >>16);

	return ~cksum;
}

int check_sum(void *data, u32 len)	
{
	__packed u16 *sum;
	u8 *p = (u8 *)data;
	
	p += len - 2;

	sum = (u16 *)p;

	if (calc_sum(data, len - 2) != *sum) {	// check sum error		
 		return 0;
	}
	return 1;	// ckeck sum ok
}

vs8 recv_cmd(void) 
{
	int datalen;
	vs8 ret = -1;
	struct emsp_header *hdr = (struct emsp_header *)recvbuf;
	u8 *data = &recvbuf[sizeof(struct emsp_header)];

    recvlen = UART_receive_buf(recvbuf);


	if (!check_sum(hdr, sizeof(struct emsp_header)) || recvlen != hdr->size) {	// check sum error or length error
		ret = -1;
		goto done;
	}
	
	datalen = hdr->size - sizeof(struct emsp_header);

	if (!check_sum(data, datalen)) {	// check sum error
		ret = -1;
		goto done;
	}

	switch (hdr->cmdcode) {
		case EMSP_CMD_RESET:				// reset GH320 and stack or we can reset hardware
        	ret = 0;
			break;
		case EMSP_CMD_GET_CONFIG:			// get current system configuration and return them to HOST
			if (hdr->result == 1) {
			memcpy(recvdata, data, datalen - 2);
			recvlen = datalen - 2;
			ret = 0;
			}
			break;
		case EMSP_CMD_SET_CONFIG:			// set new configuration
			if (hdr->result == 1) {
			ret = 0;
			}
			break;
		case EMSP_CMD_SCAN_AP:				// scan ap and return scan result to HOST
			memcpy(recvdata, data, datalen - 2);
			recvlen = datalen - 2;
			ret= hdr->result;
			break;
		case EMSP_CMD_START:				// start module and handle wifi/tcpip
		    ret = 0;
			break;
		case EMSP_CMD_SEND_DATA:			// host send data to local, we should send them to GH320
		    ret = 0;
       		break;
		case EMSP_CMD_RECV_DATA:			// host receive data from local, if there is no any data, then return size = 0
		   if (hdr->result == 1) 
		   {
			memcpy(recvdata, data, datalen - 2);
			recvlen = datalen - 2;
			ret = 0;
		   }
			break;
		case EMSP_CMD_GET_STATUS:			// get module current status
		   if (hdr->result == 1) 
		   {
			memcpy(recvdata, data, datalen - 2);
			recvlen = datalen - 2;
			ret = 0;
		   }
			break;
		case EMSP_CMD_GET_VER:
		   if (hdr->result == 1) 
		   {
			memcpy(recvdata, data, datalen - 2);
			recvlen = datalen - 2;
			ret = 0;
		   }
			break;
		case EMSP_CMD_GET_RF_POWER:			// host send data to local, we should send them to GH320
		   if (hdr->result == 1) 
		   {
			memcpy(recvdata, data, datalen - 2);
			recvlen = datalen - 2;
			ret = 0;
		   }
			break;
		case EMSP_CMD_SET_RF_POWER:			// host send data to local, we should send them to GH320
			if (hdr->result == 1) {
			ret = 0;
			}
			break;
		default:
			break;
	}
done:
	return ret;

}

int send_cmd(unsigned short cmdcode, unsigned short result, void *data, unsigned short datalen)
{
	u8 *dt;
	struct emsp_header *hdr;
	struct emsp_footer *ft;
	u16 len = sizeof(struct emsp_header) + datalen + sizeof(struct emsp_footer);

	if (sendbuf == 0) {
		return -1;
	}
	hdr = (struct emsp_header *)sendbuf;
	dt = (sendbuf + sizeof(struct emsp_header));
	ft = (struct emsp_footer *)(dt + datalen);

	hdr->cmdcode = cmdcode;
	hdr->size = len;
	hdr->result = result;
	hdr->checksum = calc_sum(hdr, sizeof(struct emsp_header) - 2);
	
	memcpy(dt, data, datalen);

	ft->checksum = calc_sum(dt, datalen);

	UART_send_buf(sendbuf, len);
	switch(buffer_bytes){
      case buffer_0bytes:	
		   delay_ms(0);
		   break;							 // EM380C's response has a delay_ms
      case buffer_8bytes:	
		   delay_ms(15);
		   break;							 // EM380C's response has a delay_ms
      case buffer_16bytes:	
		   delay_ms(30);
		   break;							 // EM380C's response has a delay_ms
      case buffer_32bytes:	
		   delay_ms(60);
		   break;							 // EM380C's response has a delay_ms
      case buffer_64bytes:	
		   delay_ms(125);
		   break;							 // EM380C's response has a delay_ms
      case buffer_128bytes:	
		   delay_ms(250);
		   break;							 // EM380C's response has a delay_ms
      case buffer_256bytes:	
		   delay_ms(500);
		   break;							 // EM380C's response has a delay_ms
	  default:
		    delay_ms(500);
			break;
			}
	if(cmdcode == EMSP_CMD_SCAN_AP){
	 delay_ms(500);	 delay_ms(800);
	}
	if(cmdcode == EMSP_CMD_SET_CONFIG){
	 delay_ms(600);	delay_ms(400);	
	}
	
	return recv_cmd();
}



