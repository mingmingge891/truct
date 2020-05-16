/**
  ******************************************************************************
  * @file    WH-BT200_Implements.c
  * @author  HANSEN     emil:951868692@qq.com
  * @version V0.0.0
  * @date    05-23-2019
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
/* Includes ------------------------------------------------------------------*/	
#include "net_module.h"
#include "adp_mcu_delay.h"
#include "dl_serial.h"
#include "dl_time.h"
#include "string.h"
#include "stdio.h"


#define RETURN_FLAG    						"OK"//不同的2g模块命令返回正确的标志不一样
#define RE_SENT_CNT    						3    //从发的次数，在没有收到模块的返回时需要重发
#define RE_SENT_INTERVAL_TIMER  	1000 //每次重新发送命令的间隔时间
#define SOCKET_CONNECT_WAIT_TIME  5000 //socket连接服务器等待时间
#define PACKAGE_TE                20   //接收打包时间


net_module_inside_fun_t *p_2gHandle = NULL;


/* 得到ID */
static int8* usr_2g_get_net_modlue_mid(void){return " ";}
/* 得到版本 */
static int8* usr_2g_get_net_modlue_version(void){return " ";}
/* 恢复出厂设置 */
static uint8_t usr_2g_set_net_modlue_reld(void){return 1;}
/* 重启设备 */
static uint8_t usr_2g_net_modlue_reboot(void){return 1;}
/* 回显示 */
static uint8_t usr_2g_set_net_modlue_echo(void){return 1;}

/* 命令格式化 */
static int8* usr_2g_socket_cmd_fmt(int8* cmd,SOCKET_PROTOCOL_U protocol,CS_MODE_U cs,int8* ip,u16 port)
{
	cmd[0] = 0x00;
	
	if(protocol == UDP){
		
	}
	else if(protocol == TCP)
	{
		if(cs == CLIENT){
			sprintf(cmd,"AT+CIPSTART=%s,%s,%d\r\n","TCP",ip,port);
		}
	}
	return cmd;
}
/* 设置传输模式 */
static uint8_t usr_set_tranmission_mode(uint8_t mode)
{
	if(mode == CMD_MODE)
	{
		/*先判断是否在AT模式 任意发送一个指令测试一下*/
		if(inside_send_cmd_check("AT\r\n","OK"))
		{
			return 1;
		}
		
		/*在发送+++之前200ms内不能有数据*/
		ADP_delay_ms(220);
		return inside_send_cmd_check("+++",RETURN_FLAG);
	}
	else if(mode == TRANSMISSION_MODE)
	{
		return inside_send_cmd_check("AT+CIPTMODE\r\n",RETURN_FLAG);
	}
	return 0;
}
/* 进入命令模式 */
static uint8_t usr_2g_enter_cmd_mode(void)
{
	return usr_set_tranmission_mode(CMD_MODE);
}
/* 进入透明传输模式 */
static uint8_t usr_2g_enter_tranmission_mode(void)
{
	return usr_set_tranmission_mode(TRANSMISSION_MODE);
}
/* 开启一个定时器 */
static uint8_t usr_2g_start_timer(void (*ptr)(void*),uint32_t timer_period)
{
	return virtual_timer1Handler->start_timerEx(virtual_timer1Handler->this_h_id,0,(void*)0,ptr,timer_period);
}
/* 关闭一个定时器 */
static uint8_t usr_2g_start_stop(void (*ptr)(void*))
{
	return virtual_timer1Handler->stop_timerEx(virtual_timer1Handler->this_h_id,0);
}
/* 断开网络 */
static uint8_t usr_2g_disconnect_net(void)
{
	return inside_send_cmd_check("AT+CIPCLOSE\r\n",RETURN_FLAG);
}
/* 自动重载定时器 */
timerEx_t usr_2g_timerEx = 
{
	usr_2g_start_timer,
	usr_2g_start_stop
};
//发送一个字节
static void usart_send_data(u8 pData)
{
	serial.sendHex(serial.uart2,&pData,1);
}
//串口处理函数
void uart2Callback(u8* data,u16 len)
{
	implement_net_module_property.input_data(data[0]);
}
static void moudleInit(void)
{
	/*串口注册 128缓存 单字节模式*/
	serial.config(serial.uart2,115200);
	serial.register_callback(serial.uart2,uart2Callback,128,1);
	
	while(!usr_set_tranmission_mode(CMD_MODE));

	inside_send_cmd_check("ATE0\r\n",RETURN_FLAG);/*关闭回显示*/
	inside_send_cmd_check("AT+CIPCLOSE\r\n",RETURN_FLAG);//关闭SOCKET连接
	inside_send_cmd_check("AT+CIPHMODE=0\r\n","OK");//关闭心跳包发送
	
	/*SOCKET 配置*/
	while(!inside_send_cmd_check("AT+CIPTCFG=0,5\r\n",RETURN_FLAG));//5次重传
	while(!inside_send_cmd_check("AT+CIPTCFG=1,0\r\n",RETURN_FLAG));//0延时
	while(!inside_send_cmd_check("AT+CIPTCFG=2,1024\r\n",RETURN_FLAG));//1kbuffer
	while(!inside_send_cmd_check("AT+CIPTCFG=3,1000\r\n",RETURN_FLAG));//1s打包
	while(!inside_send_cmd_check("AT+GPS=0\r\n","OK"));//关闭GPS
	while(!inside_send_cmd_check("AT+CGATT=1\r\n","OK"));//注册上网
	while(!inside_send_cmd_check("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n","OK"));//设置PDP参数
	while(!inside_send_cmd_check("AT+CGACT=1,1\r\n","OK"));//激活PDP
	while(!inside_send_cmd_check("AT+CIPMUX=0\r\n","OK"));//单链接模式

}
/**
  * @brief  模块基础功能实现
  * @param  None
  * @retval : None
**/
net_module_basis_function_t implemen_net_module_basis=
{
	.enter_cmd_mode 								= usr_2g_enter_cmd_mode,
	.enter_tranmission_mode 				= usr_2g_enter_tranmission_mode,
	.get_net_modlue_mid 						= usr_2g_get_net_modlue_mid,
	.get_net_modlue_version 				= usr_2g_get_net_modlue_version,
	.set_net_modlue_reld 						= usr_2g_set_net_modlue_reld,
	.net_modlue_reboot 							= usr_2g_net_modlue_reboot,
	.set_net_modlue_echo						= usr_2g_set_net_modlue_echo,
	.net_modlue_wait_boot 					= NULL,
	.net_modlue_disconnect_net			= usr_2g_disconnect_net,
};
/**
  * @brief  实现接口函数
  * @param  None
  * @retval : None
**/
net_module_prop_t implement_net_module_property = 
{
	.uart_send_oneByte 								= usart_send_data,																			/*!实现发送一个字节*/
	.net_module_delay_ms 							= ADP_delay_ms,																					/*!实现1ms延时函数*/
	.socket_cmd_fmt	 									= usr_2g_socket_cmd_fmt,																/*!实现socket 指令格式化*/
	.set_tranmission_mode 						= usr_set_tranmission_mode,															/*!设置2g的数据传输模式*/
	.input_data 											= NULL,																									/*!这里必须为NULL 外部调用implement_net_module_property.input_data 传入串口的一帧数据*/
	.re_sent_cnt 											= RE_SENT_CNT,                  												/*!当未收到指令时重新发送指令的次数*/
	.package_time 										= PACKAGE_TE,																						/*接受一帧数据的打包时间*/
	.return_flag 											= RETURN_FLAG,																					/*!判断指令发送成功 收到模块的标志字符串*/
	.re_sent_interval_timer 					= RE_SENT_INTERVAL_TIMER,																/*!重发指令的时间间隔*/
	.socket_wait_time                 = SOCKET_CONNECT_WAIT_TIME,
	.timerEx 													= &usr_2g_timerEx,            													/*实现一个最小单位是1ms的定时器函数*/
	.net_module_basis 								= &implemen_net_module_basis,   												/*!实现网络模块基本的功能处理*/
	.Init                             = moudleInit,
};

