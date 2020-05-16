/**
  ******************************************************************************
  * @file    net_module.c
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

#include "net_module.h"
#include "string.h"
#include "stdlib.h"
#include "stdarg.h"
#include <stdio.h>

/*Function --------------------------------------------------------------------*/
static void 	 inside_hex_data_tranmission(uint8_t *data,uint16_t length);
static uint8_t inside_socket(SOCKET_PROTOCOL_U protocol,CS_MODE_U mode,int8* ip,uint16_t port);
static uint8_t inside_net_module_setCb(void (*ptr)(net_module_callback_t*));
static uint8_t inside_net_module_setbuffer(uint16_t at_buf_size,uint16_t receive_buf_size);

/*Value -----------------------------------------------------------------------*/
callback_Def  callback;

/*at cmd snd receive buffer*/
#if BUF_MALLOC_EN > 0
	int8* p_at_buf;
	int8* p_receive_buf;
#else
	int8* p_at_buf;
	int8* p_receive_buf;
	int8 	at_buf[AT_BUF_SIZE];
	int8 	receive_buf[RECEIVE_BUF_SIZE];
#endif
uint16_t p_receive_buf_length;

/*network module abstract struct*/
net_module_prop_t* p_property;

/*synchronous receive flag*/
uint8_t synchronous_locked = 0;

/*data receive max length*/
uint16_t receive_data_buffer_size;

/*socket execution cnt*/
uint16_t socket_execution_cnt = 0;

/*inside function */
net_module_inside_fun_t inside_fun;

/*receive complete flag*/
uint8_t receive_comolete_flag = 0;

static int8* out_put_at_cmd_buffer(void)
{
	return p_at_buf;
}
uint8_t net_module_Init(void)
{
	p_property->Init();
	return 1;
}
/**
  * @brief new_net_module_inside_fun
  * @param  net_module_inside_fun_t *p
  * @retval : void
**/
static void new_net_module_inside_fun(net_module_inside_fun_t *p)
{
	p->basis_function = p_property->net_module_basis;
	
	p->get_at_cmd_buffer = out_put_at_cmd_buffer;
	p->send_cmd = inside_send_cmd;
	p->send_cmd_check = inside_send_cmd_check;
	p->socket = inside_socket;
	p->hex_data_tranmission = inside_hex_data_tranmission;
	p->net_module_setCb = inside_net_module_setCb;
	p->net_module_setbuffer = inside_net_module_setbuffer;
	p->net_module_Init = net_module_Init;
}

/**
  * @brief None callback of event
  * @param  EVENT_U e 
  * @retval : void
**/
static void cb_set_event(EVENT_U e)
{
	net_module_callback_t l;
	l.event = e;
	l.receive_data.data = NULL;
	if(callback != NULL)
	{
		callback(&l);
		l.event = CLEAR;
	}
}
/**
  * @brief None
  * @param  None
  * @retval : None
**/
static void cb_set_data(uint8_t* data,uint16_t length )
{
	
	net_module_callback_t l;
	
	l.event = CLEAR;
	l.receive_data.data = data;
	
	l.receive_data.length = length;
	
	if(callback != NULL)
	{
		callback(&l);
	}
	
}

/**
  * @brief timer out handler for usart receive data
  * @param  None
  * @retval : None
**/
static void timer_out_handler(void* p_arg)
{
	p_property->timerEx->stop(timer_out_handler);
	cb_set_data((uint8_t*)p_receive_buf,p_receive_buf_length);
	p_receive_buf_length = 0;
	receive_comolete_flag = 1;
}

/**
  * @brief module_data_input 
  * @param  data :A frame of data
  * @retval : None
**/
static void module_data_input(uint8_t c_data)
{	
	/*synchronous receive data*/
	if(synchronous_locked == 0x00){
		
		p_property->timerEx->start(timer_out_handler,p_property->package_time);
		
		p_receive_buf[p_receive_buf_length++] = c_data;
		if(p_receive_buf_length == receive_data_buffer_size){
			cb_set_data((uint8_t*)p_receive_buf,p_receive_buf_length);
			p_receive_buf_length = 0;
		}
	}
}
/**
  * @brief    check input str is contained in the return data
  * @param  	str Comparison parameter
  * @retval : 1 success ,0 failure
**/
static uint8_t check(char* str)
{
	uint8_t tmp_result;
	
	/*synchronous handler data*/
	synchronous_locked = 1;
		if(strstr(p_receive_buf,str)!=NULL)
		{
			tmp_result = 1;
		}
		else
		{
			tmp_result = 0;
		}
	synchronous_locked = 0;
	
	return tmp_result;
}
/**
  * @brief    tranmission the hex data
  * @param  	data hex data ,send data length 
  * @retval :   None
**/
static void inside_hex_data_tranmission(uint8_t *data,uint16_t length)
{
	while(length--)
	{
		p_property->uart_send_oneByte(*data++);
	}
}
/**
  * @brief    
  * @param  	
  * @retval
**/
static void inside_send_str(int8* str)
{
	inside_hex_data_tranmission((uint8_t*)str,strlen(str));
}
uint8_t inside_send_cmd_check_for_time(int8* cmd,int8* return_str,u16 interval_timer)
{

	/* receive the RETURN_FLAG flag ? if not,it will be send RE_SENT_CNT times.
	Each interval RE_SENT_INTERVAL_TIMER ms resend*/
	
	/*clear memory for p_receive_buf*/
	memset(p_receive_buf,0,receive_data_buffer_size);
	
	/*interval sent*/
	inside_send_str(cmd);
	receive_comolete_flag = 0;
	uint16_t m_cnt = interval_timer;
	while(m_cnt--)
	{
		if(receive_comolete_flag == 0x01){
			break;
		}
		p_property->net_module_delay_ms(1);
	}
	
	/* if (@param) return_str indicates that there is no need to
	determine the validity of the returned data*/
	if(return_str == NULL)
	{			
		if(p_receive_buf[0] != 0){
			return 1;
		}
	}
	/*if the return flag is correct*/
	else if(check(return_str)){
		return 1;
	}
	
	return 0;
}
/**
  * @brief    sned the at cmd to module
  * @param  	cmd at cmd return_str @ compare string 
  * @retval : 1 success ,0 failure
**/
uint8_t inside_send_cmd_check(int8* cmd,int8* return_str)
{	
	uint8_t re_sent_cnt = p_property->re_sent_cnt - 1;
	
	/* receive the RETURN_FLAG flag ? if not,it will be send RE_SENT_CNT times.
	Each interval RE_SENT_INTERVAL_TIMER ms resend*/
	do
	{
		/*clear memory for p_receive_buf*/
		memset(p_receive_buf,0,receive_data_buffer_size);
		
		/*interval sent*/
		inside_send_str(cmd);
		receive_comolete_flag = 0;
		uint16_t m_cnt = p_property->re_sent_interval_timer;
		while(m_cnt--)
		{
			if(receive_comolete_flag == 0x01){
				break;
			}
			p_property->net_module_delay_ms(1);
		}
		
		/* if (@param) return_str indicates that there is no need to
		determine the validity of the returned data*/
		if(return_str == NULL)
		{			
			if(p_receive_buf[0] != 0){
				return 1;
			}
		}
		/*if the return flag is correct*/
		else if(check(return_str)){
			return 1;
		}
	}
	while(re_sent_cnt--);
//	memcpy(test_buffer,p_receive_buf,28);
	return 0;
}

/**
  * @brief    sned the at cmd to module and return result
  * @param  	cmd at cmd
  * @retval : the handler result
**/
int8* inside_send_cmd(int8* cmd)
{
	if(inside_send_cmd_check(cmd,NULL))
	{
		return p_receive_buf;
	}
	return NULL;
}

/**
  * @brief 		create socket connect
  * @param  	protocol  @ UDP   TCP
							modemode @ clent server
							ip        0.0.0.0-255.255.255.255
							port      the device internet port
  * @retval : 1         success
							0         failure
**/
static uint8_t inside_socket(SOCKET_PROTOCOL_U protocol,CS_MODE_U mode,int8* ip,uint16_t port)
{		
	
	setCmdMode:
	/*socket execution cnt up*/
	if(++socket_execution_cnt >= 20){
		socket_execution_cnt = 0;
		cb_set_event(SOCKET_CONNECT_TIMEUP);
		return 0;
	}
	
	/*first set Mode*/
	if(!p_property->set_tranmission_mode(CMD_MODE))
	{
		goto setCmdMode;
	}
	
	socket_execution_cnt = 0;
	reconnect:
	
	/*socket execution cnt up*/
	if(++socket_execution_cnt >= (20*1000/p_property->socket_wait_time)){
		socket_execution_cnt = 0;
		cb_set_event(SOCKET_CONNECT_TIMEUP);
		return 0;
	}
	
	/*FMT */
	p_at_buf = p_property->socket_cmd_fmt(p_at_buf,protocol,mode,ip,port);
	
	if(inside_send_cmd_check_for_time(p_at_buf,p_property->return_flag,p_property->socket_wait_time))
	{
		/*set transmission mode*/
		if(!p_property->set_tranmission_mode(TRANSMISSION_MODE)){
			goto reconnect;
		}
		socket_execution_cnt = 0;
		return 1;
	}
	goto reconnect;
}

/**
  * @brief    
  * @param  	buf_size @ memory szie
							ptr @ event callback register
  * @retval :   None
**/
static uint8_t inside_net_module_setbuffer(uint16_t at_buf_size,uint16_t receive_buf_size)
{
	/*Memory allocation*/
	if(p_at_buf == NULL)
	{		
		#if BUF_MALLOC_EN > 0
			p_at_buf = (int8*)my_malloc(at_buf_size);
		#else
			p_at_buf = at_buf;
		#endif
	}
	if(p_receive_buf == NULL)
	{
		#if BUF_MALLOC_EN > 0
			p_receive_buf = (int8*)my_malloc(receive_buf_size);
		#else
			p_receive_buf = receive_buf;
		#endif
		/*get max receive data size*/
		receive_data_buffer_size = receive_buf_size;
	}
	/*if malloc memory failure*/
	if(p_at_buf == NULL || p_receive_buf == NULL){
		return 0x00;
	}
	
	synchronous_locked = 0;
	
	return 1;
	
}


/**
  * @brief    initialize the socket
  * @param  	buf_size @ memory szie
							ptr @ event callback register
  * @retval :   None
**/
static uint8_t inside_net_module_setCb(void (*ptr)(net_module_callback_t*))
{

	/*Add callback funtion*/
	if(ptr != NULL)
	{
		callback = ptr;
	}
	
	synchronous_locked = 0;
	
	return 1;
	
}
/**
  * @brief  new_net_module
  * @param  	None
  * @retval :   net_module_inside_fun_t 
**/
net_module_inside_fun_t* new_net_module(void)
{
	if(p_property == NULL)
	{
		p_property = &implement_net_module_property;
		
	}
	
	/* expose the function address, for the uart data input */
	if(p_property->input_data == NULL)
	{
		p_property->input_data = module_data_input;
	}
	
	new_net_module_inside_fun(&inside_fun);
	
	return &inside_fun;
}
