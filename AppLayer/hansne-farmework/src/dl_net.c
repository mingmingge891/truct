/**
  ******************************************************************************
  * @file    dl_net.c
  * @author  HANSEN     emil:951868692@qq.com
  * @version V1.0.0
  * @date    27-6-2019
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
#include "dl_net.h"
#include "dl_device.h"
#include "MobileNet.h"
#include "string.h"

/*Macro ----------------------------------------------------------------------*/
#define SERVER_UDP             0u
#define SERVER_TCP             1u
#define CLIENT_UDP             2u
#define CLIENT_TCP             3u
#define CONNECT             	 4u
#define DIS_CONNECT            5u

/*private function------------------------------------------------------------*/
static NetConnectType_t* socket(net_type type,net_protocol protocol);
static int net_connect(NetConnectType_t* type,SocketAddrIn_t* socket_addr_in);
static int net_send(void *d,u32 length);
static int net_disconnect(void);
/*private values--------------------------------------------------------------*/
static NetConnectType_t NetConnectType;
static int fd;

/*public values --------------------------------------------------------------*/
Net_API_t pNet_API_Implements = {
	.socket 		= socket,
	.connect 		= net_connect,
	.send    		= net_send,
	.disconnect = net_disconnect,
};

/**
  * @brief static int net_send(void *d,u32 length)
  * @param none
  * @retval none
**/
static int net_send(void *d,u32 length)
{
	/* 写函数 */
	write_moudle_device(fd,d,length);
	
	return 1;
}
/**
  * @brief SocketAddrIn_t* (*socket)(net_type type,net_protocol protocol)
  * @param none
  * @retval none
**/
static NetConnectType_t* socket(net_type type,net_protocol protocol)
{

	/* 设置socket的网络连接类型 */
	
	NetConnectType.type = type;
	
	NetConnectType.protocol = protocol;
	
	return &NetConnectType;
}

/**
  * @brief static int net_disconnect(void)
  * @param none
  * @retval none
**/
static int net_disconnect(void)
{
	return ioctl_moudle_device(fd,DIS_CONNECT,0x00);
	
}
/**
  * @brief int net_connect(SocketAddrIn_t* socket_addr_in)
  * @param none
  * @retval none
**/
static int net_connect(NetConnectType_t* nc_type,SocketAddrIn_t* socket_addr_in)
{
	static struct connectPar{
		char ip[16];
		u16  port;
	}connect;
	
	switch(nc_type->type){
		case SERIVER:
			if(nc_type->protocol == UDP){
				ioctl_moudle_device(fd,SERVER_UDP,0x00);
			}
			if(nc_type->protocol == TCP){
				ioctl_moudle_device(fd,SERVER_TCP,0x00);
			}
			break;
		case CLIENT:
			if(nc_type->protocol == UDP){
				ioctl_moudle_device(fd,CLIENT_UDP,0x00);
			}
			if(nc_type->protocol == TCP){
				ioctl_moudle_device(fd,CLIENT_TCP,0x00);
			}
			break;
	}
	
	/* 参数获取 */
	memcpy(connect.ip,socket_addr_in->ip,sizeof(connect.ip));
	connect.port = socket_addr_in->port;
	
	if(socket_addr_in->setDataCallback != NULL)
	{
		/* 注册事件 */
		event_set_moudle_device(fd,socket_addr_in->setDataCallback);
	}
	
	/*配置*/
	ioctl_moudle_device(fd,CONNECT,(u8*)&connect);
	
	return 1;
}

/**
  * @brief void ail_os_port_initialize(void)
  * @param none
  * @retval none
**/
void dl_Net_initialize(void)
{
	/*首先移动网络*/
	fd = open_moudle_device("MobileNet");
	
	if(fd < 0 )
		while(1);
}
/**
  * @brief Net_API_t*	Ail_Net_Get(void)
  * @param none
  * @retval none
**/
Net_API_t* Ail_Net_Get(void)
{	
	return &pNet_API_Implements;
}

