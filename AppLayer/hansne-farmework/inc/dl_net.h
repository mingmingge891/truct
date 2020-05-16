/**
  ******************************************************************************
  * @file    dl_net.h
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
#ifndef __DL_NET_H_
#define __DL_NET_H_
#include "adapter.h"
/**
  * @brief  网络类型
  * @param  None
  * @retval : None
**/
typedef enum {
	SERIVER = 0,
	CLIENT = 1,
}net_type;
/**
  * @brief  协议类型
  * @param  None
  * @retval : None
**/
typedef enum {
	UDP = 0,
	TCP = 1,
}net_protocol;

/**
  * @brief  网络地址族
  * @param  None
  * @retval : None
**/
typedef struct {
	char *ip;
	u16  port;
	void (*setDataCallback)(void* d,u32 length);
}SocketAddrIn_t;
/**
  * @brief  网络连接类型
  * @param  None
  * @retval : None
**/
typedef struct {
	net_type type;
	net_protocol protocol;
}NetConnectType_t;

/**
  * @brief  网络API
  * @param  None
  * @retval : None
**/
typedef struct{
	NetConnectType_t* (*socket)(net_type type,net_protocol protocol);
	
	int (*connect)(NetConnectType_t*,SocketAddrIn_t*);
	
	int (*send)(void* d,u32 length);
	
	int (*disconnect)(void);
	
}Net_API_t;

Net_API_t* 	Ail_Net_Get(void);
void dl_Net_initialize(void);

#endif
