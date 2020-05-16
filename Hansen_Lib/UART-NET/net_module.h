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
//�����Ƿ�̬����
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
/*����SOCKET������ģʽ*/
typedef enum
{
	SERVER = 0,/*server */
	CLIENT /*client*/
}CS_MODE_U;
/*ģ���յ����ڵ�������Ϣ*/
typedef struct
{
	uint8_t* data;
	
	uint16_t length;
	
}receive_data_t;

/*ģ���ϵͳ�¼��ص�*/
typedef struct 
{
	EVENT_U event;
	
	receive_data_t receive_data;
	
}net_module_callback_t;

/*�������Ͷ�����Ҫ���ڻص���������*/
typedef void (*callback_Def)
(
	net_module_callback_t*
);


/*interface Def------------------------------------------------------------*/
typedef struct
{
	uint8_t (*enter_cmd_mode)(void); 					 							/*!ʹģ���������ģʽ*/
	
	uint8_t (*enter_tranmission_mode)(void); 								/*!ʹģ����봫��ģʽ*/
	
	int8* (*get_net_modlue_mid)(void);											/*!�õ�ģ���id��*/
	
	int8* (*get_net_modlue_version)(void);									/*!�õ�ģ��İ汾��*/
	
	uint8_t (*set_net_modlue_reld)(void);										/*!�ָ���������*/
	
	uint8_t (*net_modlue_reboot)(void);											/*!������ģ��*/
	
	uint8_t (*set_net_modlue_echo)(void);										/*!����*/
	
	uint8_t (*net_modlue_wait_boot)(void);									/*!�ȴ�ģ������*/
	
	uint8_t (*net_modlue_disconnect_net)(void);									/*�Ͽ�����*/
	
}net_module_basis_function_t;

/*����ṹ����Ҫ�������ṩ������ ��ҪĿ�������һ����׼����*/
typedef struct
{
	void (*hex_data_tranmission)(uint8_t*,uint16_t ); 				/*16�������ݷ��ͺ���*/
	
	uint8_t (*socket)(SOCKET_PROTOCOL_U,CS_MODE_U,int8* ip,uint16_t port);/*socket���Ӻ���*/
	
	net_module_basis_function_t  *basis_function;		/*�������紦����*/
	
	uint8_t (*send_cmd_check)(int8*,int8*);/*����ָ�� ���ж��Ƿ�������*/

	int8* (*send_cmd)(int8*);/*����ָ��жϽ��յ�������*/
	
	int8* (*get_at_cmd_buffer)(void);/*�õ�ָ��BUF*/
	
	uint8_t (*net_module_setCb)(void (*ptr)(net_module_callback_t*));//�������ݻص�����
	
	uint8_t (*net_module_setbuffer)(uint16_t at_buf_size,uint16_t receive_buf_size);
	
	uint8_t (*net_module_Init)(void);

}net_module_inside_fun_t;

/*��ʱ������ṹ��,��Ҫ�����ڲ��Ķ�ʱ��ʹ�� ����:�Զ����� �������ص�*/
typedef struct 
{
	uint8_t (*start)(void (*)(void*),uint32_t);           							 			/*!����һ����ʱ��*/
	
	uint8_t (*stop)(void (*)(void*));																				/*!�ر�һ����ʱ��*/
	
}timerEx_t;

/*interface Def------------------------------------------------------------*/
typedef struct
{
	void (*uart_send_oneByte)(uint8_t);																				/*!����һ���ֽ�*/

	void (*net_module_delay_ms)(uint32_t);																		/*!1ms����ʱ����*/
		
	int8* (*socket_cmd_fmt)(int8*,SOCKET_PROTOCOL_U,CS_MODE_U,int8* ip,uint16_t port);		/*!socket ָ���ʽ��*/
	
	uint8_t (*set_tranmission_mode)(uint8_t);																	/*!����ģ��Ĵ���ģʽ*/
	
	void (*input_data)(uint8_t);																							/*!��������,���뵥���ֽ�*/
	
	uint8_t re_sent_cnt;																											/*!ÿ�����ݷ���ʶ��,ָ���ط��Ĵ���*/
	
	uint16_t package_time;																										/*!����һ֡���ݵĴ��ʱ��*/
	
	int8* return_flag;																												/*!ģ�鴦����ȷ��ָ��ص�ȷ�ϱ�־λ*/
	
	uint32_t re_sent_interval_timer;																					/*!�ط����ݵļ��ʱ��*/
	
	uint16_t socket_wait_time;																					      /*!socket�ȴ�ʱ��*/
		
	timerEx_t* timerEx; 																											/*!��ʱ���ṹ��*/
		
	net_module_basis_function_t* net_module_basis;														/*!ģ��Ļ������ܺ���ʵ��*/
	
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


/*!ʵ�ֵĽṹ������*/
extern net_module_prop_t implement_net_module_property;


#endif
