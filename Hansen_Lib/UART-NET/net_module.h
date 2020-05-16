/**
  ******************************************************************************
  * @file    net_module.h
  * @author  HANSEN     emil:951868692@qq.com
  * @version V0.0.0
  * @date    29-may-2018
  * @brief   
  ******************************************************************************
  * @attention
  * http://www.tfc.com
	* This code developed by hardware science and technology division 
	* of tfc technology co., LTD 
	* if the developer want to use the code, and the consequences shall 
	* be borne by himself. 
	* @note
  * it is belongs to the open source code.
  * <h2><center>&copy; COPYRIGHT 2018 TFC</center></h2>
  ******************************************************************************
  */
#ifndef _NET_MODULE_H
#define _NET_MODULE_H

#ifdef 	STM32

#else
#include "adapter.h"

typedef unsigned char  uint8_t;              /* Unsigned  8 bit quantity                           */
typedef 	 				char int8;                 /* Signed    8 bit quantity                           */
typedef unsigned short uint16_t;             /* Unsigned 16 bit quantity                           */
typedef          short int16;                /* Signed   16 bit quantity                           */
typedef unsigned int   uint32_t;             /* Unsigned 32 bit quantity                           */
typedef 	       int   int32;          			 /* Signed   32 bit quantity                           */
typedef float          fp32;                 /* Single precision floating point                    */
typedef double         fp64;                 /* Double precision floating point                    */
#endif


#define CMD_MODE 														1
#define TRANSMISSION_MODE 									2
//缓存是否动态设置
#define BUF_MALLOC_EN                       0

#define    AT_BUF_SIZE											128
#define    RECEIVE_BUF_SIZE               	512

/*event enum----------------------------------------------------------*/
typedef enum
{
	CLEAR = 0,										/*NULL of the flags*/
	SOCKET_CONNECT_TIMEUP,        /*socket connect timeup*/
}EVENT_U;
typedef enum
{
	UDP = 0,											/*udp mode*/
	TCP/*tcp	mode*/
}SOCKET_PROTOCOL_U;
/*关于SOCKET工作的模式*/
typedef enum
{
	SERVER = 0,/*server */
	CLIENT /*client*/
}CS_MODE_U;
/*模块收到串口的数据信息*/
typedef struct
{
	uint8_t* data;
	
	uint16_t length;
	
}receive_data_t;

/*模块的系统事件回调*/
typedef struct 
{
	EVENT_U event;
	
	receive_data_t receive_data;
	
}net_module_callback_t;

/*函数类型定义主要用在回调函数上面*/
typedef void (*callback_Def)
(
	net_module_callback_t*
);


/*interface Def------------------------------------------------------------*/
typedef struct
{
	uint8_t (*enter_cmd_mode)(void); 					 							/*!使模块进入命令模式*/
	
	uint8_t (*enter_tranmission_mode)(void); 								/*!使模块进入传输模式*/
	
	int8* (*get_net_modlue_mid)(void);											/*!得到模块的id号*/
	
	int8* (*get_net_modlue_version)(void);									/*!得到模块的版本号*/
	
	uint8_t (*set_net_modlue_reld)(void);										/*!恢复出厂设置*/
	
	uint8_t (*net_modlue_reboot)(void);											/*!重启动模块*/
	
	uint8_t (*set_net_modlue_echo)(void);										/*!回显*/
	
	uint8_t (*net_modlue_wait_boot)(void);									/*!等待模块重启*/
	
	uint8_t (*net_modlue_disconnect_net)(void);									/*断开网络*/
	
}net_module_basis_function_t;

/*这个结构体主要是向外提供抽象函数 主要目地是提高一个标准处理*/
typedef struct
{
	void (*hex_data_tranmission)(uint8_t*,uint16_t ); 				/*16进制数据发送函数*/
	
	uint8_t (*socket)(SOCKET_PROTOCOL_U,CS_MODE_U,int8* ip,uint16_t port);/*socket连接函数*/
	
	net_module_basis_function_t  *basis_function;		/*基础网络处理函数*/
	
	uint8_t (*send_cmd_check)(int8*,int8*);/*发送指令 带判断是否有数据*/

	int8* (*send_cmd)(int8*);/*发送指令不判断接收到的数据*/
	
	int8* (*get_at_cmd_buffer)(void);/*得到指令BUF*/
	
	uint8_t (*net_module_setCb)(void (*ptr)(net_module_callback_t*));//设置数据回调函数
	
	uint8_t (*net_module_setbuffer)(uint16_t at_buf_size,uint16_t receive_buf_size);
	
	uint8_t (*net_module_Init)(void);

}net_module_inside_fun_t;

/*定时器抽象结构体,主要用来内部的定时器使用 需求:自动重载 带函数回调*/
typedef struct 
{
	uint8_t (*start)(void (*)(void*),uint32_t);           							 			/*!启动一个定时器*/
	
	uint8_t (*stop)(void (*)(void*));																				/*!关闭一个定时器*/
	
}timerEx_t;

/*interface Def------------------------------------------------------------*/
typedef struct
{
	void (*uart_send_oneByte)(uint8_t);																				/*!发送一个字节*/

	void (*net_module_delay_ms)(uint32_t);																		/*!1ms的延时函数*/
		
	int8* (*socket_cmd_fmt)(int8*,SOCKET_PROTOCOL_U,CS_MODE_U,int8* ip,uint16_t port);		/*!socket 指令格式化*/
	
	uint8_t (*set_tranmission_mode)(uint8_t);																	/*!设置模块的传输模式*/
	
	void (*input_data)(uint8_t);																							/*!串口数据,输入单个字节*/
	
	uint8_t re_sent_cnt;																											/*!每次数据发送识别,指令重发的次数*/
	
	uint16_t package_time;																										/*!串口一帧数据的打包时间*/
	
	int8* return_flag;																												/*!模块处理正确的指令返回的确认标志位*/
	
	uint32_t re_sent_interval_timer;																					/*!重发数据的间隔时间*/
	
	uint16_t socket_wait_time;																					      /*!socket等待时间*/
		
	timerEx_t* timerEx; 																											/*!定时器结构体*/
		
	net_module_basis_function_t* net_module_basis;														/*!模块的基础功能函适实现*/
	
	void (*Init)(void);
	
}net_module_prop_t;


/**
  * @brief    sned the at cmd to module
  * @param  	cmd at cmd return_str @ compare string 
  * @retval : 1 success ,0 failure
**/
uint8_t inside_send_cmd_check(int8* cmd,int8* return_str);

/**
  * @brief    sned the at cmd to module and return result
  * @param  	cmd at cmd
  * @retval : the handler result
**/
int8* inside_send_cmd(int8* cmd);

/**
  * @brief  new_net_module
  * @param  	None
  * @retval :   net_module_inside_fun_t 
**/
net_module_inside_fun_t* new_net_module(void);


/*!实现的结构体命名*/
extern net_module_prop_t implement_net_module_property;


#endif
