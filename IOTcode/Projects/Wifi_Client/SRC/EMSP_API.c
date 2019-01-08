#include "EMSP_API.h"
#include "EMSP.h"
#include "em380c_hal.h"
#include <string.h>	
#include "delay.h"


extern u8 *recvdata;
extern u8 recvlen;
extern u8 buffer_bytes;
//extern EM380C_APLst_TypeDef EM380C_APLst_now[10];


vs8 EMSP_Get_status(EM380C_status_TypeDef* EM380C_status)
{  
   u32 timeout;
   vs8 ret = -1;
   for(timeout=5;timeout>0&&ret==-1;timeout--)
   {
     ret = send_cmd(EMSP_CMD_GET_STATUS, 0, NULL, 0);
	}

   switch(*(u16 *)(recvdata+2*sizeof(u8)))
 	 {
        case TCP_stop:
		   EM380C_status->TCPstatus = TCP_stop;
		   break;
        case TCP_listening:
		   EM380C_status->TCPstatus = TCP_listening;
		   break;
		case TCP_connected:
		   EM380C_status->TCPstatus = TCP_connected;
		   break;
		default:
		   EM380C_status->TCPstatus = TCP_unknown;
			break;
	 }

   switch(*(u16 *)(recvdata))
 	 {
        case wifi_connected:
		   EM380C_status->WiFistatus = wifi_connected;
		   break;
        case wifi_disconnected:
		   EM380C_status->WiFistatus = wifi_disconnected;
		   break;
		default:
		   EM380C_status->WiFistatus = wifi_unknown;
			break;
	 }

  return ret;
}

vs8 EM380C_Get_RF_POWER(EM380C_RF_POWER_TypeDef* RF_POWER)
{
   u32 timeout;
   vs8 ret = -1;
   for(timeout=5;timeout>0&&ret==-1;timeout--)
   {
     ret = send_cmd(EMSP_CMD_GET_RF_POWER, 0, NULL, 0);
	}

   RF_POWER->Min_Power=	recvdata[0];
   RF_POWER->Max_Power=	recvdata[1];
   RF_POWER->Cur_Power=	recvdata[2];

  return ret;
}

vs8 EM380C_Get_Config(EM380C_parm_TypeDef* EM380C_Parm)
{
   u32 timeout;
   vs8 ret = -1;
   for(timeout=5;timeout>0&&ret==-1;timeout--)
   {
     ret = send_cmd(EMSP_CMD_GET_CONFIG, 0, NULL, 0) ;
	}

   memcpy(EM380C_Parm,recvdata,sizeof(EM380C_parm_TypeDef));
   return ret;
}

vs8 EM380C_Set_Config(EM380C_parm_TypeDef* EM380C_Parm)
{
   u32 timeout;
   vs8 ret = -1;
   for(timeout=5;timeout>0&&ret==-1;timeout--)
   {
     ret = send_cmd(EMSP_CMD_SET_CONFIG, 0, EM380C_Parm, sizeof(EM380C_parm_TypeDef));
	}
   return ret;
}

vs8 EM380C_Set_RF_POWER(EM380C_RF_POWER_TypeDef* RF_POWER)
{
   u32 timeout;
   vs8 ret = -1;
   for(timeout=5;timeout>0&&ret==-1;timeout--)
   {
     ret = send_cmd(EMSP_CMD_SET_RF_POWER, 0, &(RF_POWER->Cur_Power), sizeof(u8));
	}

  return ret;
}

vs8 EM380C_Get_ver(u32* version)
{	 
   u32 timeout;
   vs8 ret = -1;
   for(timeout=5;timeout>0&&ret==-1;timeout--)
     ret = send_cmd(EMSP_CMD_GET_VER, 0, NULL, 0);
   *version = *(u32 *)(recvdata);

   return ret;

}

vs8 EM380C_Get_APList(EM380C_APLst_TypeDef* EM380C_APLst)
{	 
   u32 timeout,AP_number;
   u8 scan_index=0,AP_name_length;
   vs8 ret = -1;
   u8 tmpRssi;
   EM380C_APLst_TypeDef* EM380C_APLst_scan = EM380C_APLst;


   for(timeout=5;timeout>0&&ret==-1;timeout--)
   {
     ret = send_cmd(EMSP_CMD_SCAN_AP, 0, NULL, 0);
	}
   if(ret==0)
	goto done;

     AP_name_length=strcspn((const char *)recvdata, "\0")+1; 
	  
     for(AP_number = 0;AP_number < ret;AP_number++)
   {  
      
	  memcpy(EM380C_APLst_scan, recvdata+scan_index, AP_name_length);							 //copy AP name
	  tmpRssi = *(recvdata+scan_index+AP_name_length);											 //get RSSI
	  if(tmpRssi < 40)
         tmpRssi = 100;
	  else if(tmpRssi > 95)
	     tmpRssi = 0;
	  else
	    {
	     tmpRssi = (float)(95 - tmpRssi) / (95-40) * 100;
	    }
	  
	  EM380C_APLst_scan->AP_signal = tmpRssi;				 
	  scan_index += AP_name_length+2;
	  AP_name_length =  strcspn((const char *)(recvdata+scan_index), "\0")+1;  
      EM380C_APLst_scan += sizeof(EM380C_APLst_TypeDef);
	}
   scan_index=0;
done:
	return ret;

}

u32 EM380C_Send_Data(u8* Data,u32 len)
{
   vs8 ret = -1;
   u32 timeout;

   for(timeout=5;timeout>0&&ret==-1;timeout--)
   {
     ret = send_cmd(EMSP_CMD_SEND_DATA, 0, Data, len);
	}

	return ret;

}

vs8 EM380C_Set_Mode(EM380C_mode_TypeDef mode)
{
  vs8 ret = -1;
  switch(mode)
 {
    case DTU_mode:{
       SET_STATUS_PIN();
	   ret = 0;
	   break;
	   }
    case config_mode:{
       RESET_STATUS_PIN();
	   ret = 0;
	   break;
	   }
	default:
	   break;
 }
  return ret;

}

vs8 EM380C_Sleep(void)
{
  RESET_WAKEUP_PIN();
  delay_ms(100);
  return 0;
}

vs8 EM380C_Wakeup(void)
{
  vs8 ret = -1;
  SET_WAKEUP_PIN();
  if(!READ_INT_PIN())
	 ret = 0;
  return ret;

}
vs8 EM380C_Startup(void)
 {	 
   u32 timeout;
   s8 ret = -1;

   for(timeout=5;timeout>0&&ret==-1;timeout--)
   {
     ret = send_cmd(EMSP_CMD_START, 0, NULL, 0);
	}
   return ret;
}



vs8 EM380C_Init(uint32_t BaudRate,uint16_t WordLength,uint16_t StopBits,uint16_t Parity,uint16_t CTSRTS,uint16_t buffer)
{  	
   u32 timeout,timeout2;
   s8 ret = -1;
   buffer_bytes = buffer;

   recvdata = malloc(200);
   for(timeout=10;timeout>0&&ret==-1;timeout--)
   {
      if(EM380C_HAL_Init(BaudRate,WordLength,StopBits,Parity,CTSRTS) == EM380ERROR)
	     break;
	  for(timeout2=3;timeout2>0&&ret==-1;timeout2--)
        {
		 ret = send_cmd(EMSP_CMD_GET_VER, 0, NULL, 0);
		}
   }
	return ret;
}



 vs8 EM380C_Reset(void)
 {	 
   u32 timeout;
   s8 ret = -1;

   for(timeout=5;timeout>0&&ret==-1;timeout--)
   {
     ret = send_cmd(EMSP_CMD_RESET, 0, NULL, 0);
	}
   return ret;
}
