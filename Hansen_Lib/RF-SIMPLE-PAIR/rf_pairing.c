/**
  ******************************************************************************
  * @file    rf_pairing.c
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
	
#include "rf_pairing.h"
#include "string.h"
#include "stdlib.h"
#include "stdarg.h"
#include <stdio.h>
#include "timerEX.h"

/*Value -----------------------------------------------------------------------*/

/*rf pairing abstract struct*/
rf_pair_prop_t* p_property;

/*配对请求信息*/
pair_process_info_t pair_process_info;

setint_info_t *p_seting_property;

static int mac_id_compare(uint8_t* mac_id)
{
	for(uint8_t i=0;i<ID_LENGTH;i++)
	{
		if(mac_id[i] != Rx_Buf[i])
		{
			return 0;
		}
	}
	return 1;
}

#ifdef SLAVE

uint8_t slave_start_pair_flag = 1;

void success_handler(void)
{
	/*绑定成功返回 设备信息*/
	
	
	p_property->pair_status(1,p_seting_property->data);
	
	osal_stop_timerEx(0,success_handler);
	
	slave_start_pair_flag = 1;
}

/**
  * @brief 从机
  * @param 
  * @retval
**/
void rf_start_pair_process_slave(void)
{
	pair_process_info_t *p = (pair_process_info_t*)Rx_Buf;
	
	static uint8_t  step = 0;
	static uint32_t receive_cnt = 0;
	
	/*开始配对*/
	if(slave_start_pair_flag)
	{
		slave_start_pair_flag = 0;
		p_property->pair_status(2,NULL);
		
		receive_cnt = 0;
		step = 0;
	}
	
	if(step == 0)
	{
		/*得到主机发送的请求信息*/
		if((p->special_encoding[0]==0xC1) &&(p->special_encoding[1]==0xC2))
		{
			memcpy(pair_process_info.mac,p_property->get_device_id(),ID_LENGTH);
			pair_process_info.special_encoding[0] = 0xC1;
			pair_process_info.special_encoding[1] = 0xC2;
			p_property->send_data((uint8_t*)&pair_process_info);
		}
	}
	/*对比主机发送来的目标id是否和自身的id相同*/
	if(mac_id_compare(p_property->get_device_id()))
	{
		step = 1;
		osal_start_timerEx(0,success_handler,1000);//自动重新载入定时器
		memcpy(pair_process_info.mac,p_property->get_device_id(),ID_LENGTH);
		
		/*同时记录设置信息 偏移量是ID长度+编码长度*/
		memcpy(p_seting_property->data,&Rx_Buf[ID_LENGTH+CODING_LENGTH],p_seting_property->length);
		
		/*返回第二步特殊数据 c3 c4*/
		pair_process_info.special_encoding[0] = 0xC3;
		pair_process_info.special_encoding[1] = 0xC4;
						
		p_property->send_data((uint8_t*)&pair_process_info);
			
		/*确保收到主机发送来的不是超时数据  而是确认的有效数据  数据是双向的*/
		if(p->special_encoding[0] == 0xAA)
		{
			/*成功的次数小于25次 绑定失败 总共50次*/
			if(++receive_cnt < 5)
			{
				osal_stop_timerEx(0,success_handler);
			}
		}
	}
	memset(Rx_Buf,0xFF,RX_BUF_LEN);
}

#endif

#ifdef MASTER

uint8_t tmp_mac[ID_LENGTH];

static uint8_t event_interrupt(void)
{
	static uint16_t time_out = 0;
	if(++ time_out >PAIR_TIME_OUT)
	{
		time_out = 0;
		return 1;
	}
	return 0;
}

//等待ACK
static uint8_t rf_wait_ack(uint8_t status)
{
	pair_process_info_t *p = (pair_process_info_t*)(&Rx_Buf[0]);
	
	if(status == 1)
	{
		if(p->special_encoding[0] == 0xE1 && p->special_encoding[0] == 0xE2)
		{
			/*从机设备已经被绑定了 记录标记*/
			pair_process_info.bind_flag = 0x55;
			return 1;
		}
		if((p->special_encoding[0] == 0xC1) && (p->special_encoding[1] == 0xC2))
		{
			/*临时记录记录从机的ID*/
			memcpy(tmp_mac,p->mac,ID_LENGTH);
			
			memset(Rx_Buf,0xFF,RX_BUF_LEN);
			return 1;
		}
	}
	if(status == 2)//第二步
	{
		if(mac_id_compare(pair_process_info.mac))//对比从机的id是否是之前的id
		{
			if((p->special_encoding[0] == 0xC3) && (p->special_encoding[1] == 0xC4))
			{
				memset(Rx_Buf,0xFF,RX_BUF_LEN);
				return 1;
			}
		}
	}
	memset(Rx_Buf,0xFF,RX_BUF_LEN);
	return 0;
}

void rf_pair_state_clear(void)
{
		p_property->pair_status(3,NULL);
}

void* rf_pair_ack(void)
{
	/*配对请求的标识符*/
	uint8_t identifier[2] = {0xC1,0xC2};
	
	memcpy(pair_process_info.special_encoding,identifier,2);
	
	memset(pair_process_info.mac,0xFF,ID_LENGTH);
	/*开始配对指示*/
	p_property->pair_status(2,NULL);
	/*@0 */
	do
	{
		/*发送配对请求到从机*/
		p_property->send_data((uint8_t*)&pair_process_info);
		p_property->delay_ms(1);
		
		/*如果外部事件打断就结束本次配对 （超时或者按键退出）*/
		if(event_interrupt())
		{
			goto pair_fail;
		}
	}
	while(!rf_wait_ack(1));
	memcpy(pair_process_info.mac,tmp_mac,ID_LENGTH);//得到从机的ID
	memset(tmp_mac,0xFF,ID_LENGTH);
	return (uint8_t*)(&pair_process_info);//返回状态
	
	pair_fail:
	/*配对失败指示*/
	p_property->pair_status(0,NULL);
	return NULL;
}

/**
  * @brief 主机
  * @param 
  * @retval
**/
void rf_pair_algorithm(void)
{	
	
	uint8_t valid_data = 0;
	uint8_t receive_time_out_cnt = 0;
	uint32_t receive_time_out = 0;
	uint8_t re_request_cnt = 0;
	
	uint8_t* send_temp_data = (uint8_t*)malloc(sizeof(pair_process_info_t) + sizeof(p_seting_property->length));
	
	/*@1第二步发送设置信息 需要发送第一步获取到的从机的地址,避免其他从机开机时的干扰问题
	以及设置的rf信道号码*/	
	/*第二步设置配对超时时间*/
	re_request_cnt = PAIR_TIME_OUT/1000;
	while(re_request_cnt--)
	{
		receive_time_out_cnt = 0;
		for(uint16_t i = 0;i < 50;i++)
		{
			/*第二步由于发送数据可能会出现单向传输的情况，所以必须加标志位来判断
			通信是否是双向的*/
			valid_data = 0xAA;
			
			/*ID -C3 -C4 -DATA*/
			memcpy(send_temp_data,(uint8_t*)&pair_process_info,sizeof(pair_process_info_t));
			memcpy(send_temp_data+sizeof(pair_process_info_t),(uint8_t*)p_seting_property->data,p_seting_property->length);

			p_property->send_data(send_temp_data);
			while(!rf_wait_ack(2))
			{
				/*设置20ms的接收超时时间*/
				if(++receive_time_out > 20*1000){
					receive_time_out = 0;
					receive_time_out_cnt++;
					valid_data = ~valid_data;
					break;
				}
				p_property->delay_us(1);
			}
			pair_process_info.special_encoding[0] = valid_data;
		}
		free(send_temp_data);
		
		/*@2只有通信成功20次以上才说明配对成功*/
		if(receive_time_out_cnt < 30)
		{			
			/*配对成功指示*/
			p_property->pair_status(1,NULL);
			return;
		}
	}
	/*配对失败指示*/
	p_property->pair_status(0,NULL);
}

#endif

//初始化
void init_rf_pairing(void)
{
	p_property = &implement_rf_pair_property;
	
	p_seting_property = &implement_seting_info_property;
	
}
