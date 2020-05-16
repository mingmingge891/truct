/**
  ******************************************************************************
  * @file    bluetooth.c
  * @author  HANSEN     emil:951868692@qq.com
  * @version V1.2.0
  * @date    23-05-2019
  * @brief   
  ******************************************************************************
  * @attention
  * http://www.wisdudu.com
	* This code developed by hardware science and technology division 
	* of chongqing fanghuitechnology co., LTD 
	* if the developer want to use the code, and the consequences shall 
	* be borne by himself. 
	* @note
  * it is belongs to the open source code.
  * <h2><center>&copy; COPYRIGHT 2016 WIS-DUDU</center></h2>
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "MobileNet.h"
#include "net_module.h"	
#include "dl_device.h"
#include "string.h"

/* Macro ---------------------------------------------------------------------*/
#define SERVER_UDP             0u
#define SERVER_TCP             1u
#define CLIENT_UDP             2u
#define CLIENT_TCP             3u
#define CONNECT             	 4u
#define DIS_CONNECT            5u

/* Values --------------------------------------------------------------------*/
net_module_inside_fun_t* net_moduleHandler = NULL;

//回调函数
typedef struct{
	void (*setCallback)(void* d,u32 len);
}eventCallback_t;
static eventCallback_t eventCallback;

//数据回调
static void net_deviceHandler(net_module_callback_t *arg)
{	
	if(eventCallback.setCallback != NULL)
	{
		eventCallback.setCallback(arg->receive_data.data,arg->receive_data.length);
	}
}
/**
  * @brief net_device_open
  * @param  None
  * @retval : None
**/
static int net_device_open(void)
{
	/*实例化*/
	net_moduleHandler = new_net_module();
	net_moduleHandler->net_module_setbuffer(64,256);
	net_moduleHandler->net_module_Init();
	
	return 0;
}
/**
  * @brief net_device_close
  * @param  None
  * @retval : None
**/
static void net_device_close(void)
{
	
}
/**
  * @brief net_device_read
  * @param  None
  * @retval : None
**/
static int net_device_read(void* d,u32 count)
{	
	return 0;
}
/**
  * @brief net_device_write
  * @param  None
  * @retval : None
**/
static int net_device_write(void* d,u32 count)
{
	net_moduleHandler->hex_data_tranmission(d,count);
	return 1;
}

/**
  * @brief net_device_ioctl
  * @param  None
  * @retval : None
**/
static int net_device_ioctl(u8 CMD,void* arg)
{
	static struct connectPar{
		char ip[16];
		u16  port;
	}connect;
	static struct connectMode{
		SOCKET_PROTOCOL_U PROTOCOL;
		CS_MODE_U CS;
	}mode;
	
	/* 控制命令 */
	switch(CMD)
	{
		case CLIENT_TCP:
				mode.PROTOCOL = TCP;
				mode.CS = CLIENT;
			break;
		case CLIENT_UDP:
				mode.PROTOCOL = UDP;
				mode.CS = CLIENT;
			break;
		case CONNECT:
				connect = *(struct connectPar*)arg;
				net_moduleHandler->socket(mode.PROTOCOL,mode.CS,connect.ip,connect.port);
			break;
		case DIS_CONNECT:
				net_moduleHandler->basis_function->net_modlue_disconnect_net();
			break;
	}
	return 0;
}
/**
  * @brief net_device_event
  * @param  None
  * @retval : None
**/
static int net_device_events(void(*ptr)(void* d,u32 count))
{
	/* 设置回调函数事件 */
	eventCallback.setCallback = ptr;
	if(eventCallback.setCallback != NULL)
	{
		net_moduleHandler->net_module_setCb(net_deviceHandler);
	}
	return 0;
}
/**
  * @brief net_deviceImplements
  * @param  None
  * @retval : None
**/
FunctionalModuleInterface_t net_deviceImplements = {
	.open_moudle      = net_device_open,
	.close_moudle     = net_device_close,
	.read             = net_device_read,
	.write            = net_device_write,
	.ioctl            = net_device_ioctl,
	.events           = net_device_events,
};

/**
  * @brief net_devicedevice
  * @param  None
  * @retval : None
**/
FunctionalModuleDevice_t MobileNet_Device = {
	.DeviceName = "MobileNet",
	.Implements = &net_deviceImplements,
};


