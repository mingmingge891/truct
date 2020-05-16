/**
  ******************************************************************************
  * @file    
  * @author  
  * @version 
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  * <h2><center>&copy; COPYRIGHT 2018 </center></h2>
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "net_module.h"
#include "adp_mcu_delay.h"
#include "dl_serial.h"
#include "dl_time.h"
#include "string.h"
#include "stdio.h"


#define RETURN_FLAG    						"OK"//不同的4g模块命令返回正确的标志不一样
#define RE_SENT_CNT    						3    //从发的次数，在没有收到模块的返回时需要重发
#define RE_SENT_INTERVAL_TIMER  	1000   //每次重新发送命令的间隔时间
#define SOCKET_CONNECT_WAIT_TIME  5000 //socket连接服务器等待时间
#define PACKAGE_TE                20   //接收打包时间



net_module_inside_fun_t *p_4gHandle = NULL;

/* 命令格式化 */
static int8* usr_4g_socket_cmd_fmt(int8* cmd,SOCKET_PROTOCOL_U protocol,CS_MODE_U cs,int8* ip,u16 port)
{
	cmd[0] = 0x00;
	
	if(protocol == UDP)
	{
		
	}
	else if(protocol == TCP)
	{
		if(cs == CLIENT){
			sprintf(cmd,"AT+SOCKA=%s,%s,%d\r\n","TCP",ip,port);
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
		if(inside_send_cmd_check("AT+VER\r\n","+VER"))
		{
			return 1;
		}
		
		/*在发送+++之前200ms内不能有数据*/
		ADP_delay_ms(220);
		
		if(inside_send_cmd_check("+++","a"))
		{
			return inside_send_cmd_check("a","+ok");
		}
		return 0;
	}
	else if(mode == TRANSMISSION_MODE)
	{
		return inside_send_cmd_check("AT+ENTM\r\n",RETURN_FLAG);
	}
	return 0;
}
/* 进入命令模式 */
static uint8_t usr_4g_enter_cmd_mode(void)
{
	return usr_set_tranmission_mode(CMD_MODE);
}
/* 进入透明传输模式 */
static uint8_t usr_4g_enter_tranmission_mode(void)
{
	return usr_set_tranmission_mode(TRANSMISSION_MODE);
}
/* 得到ID */
static int8* usr_4g_get_net_modlue_mid(void)
{
	return inside_send_cmd("AT+MID\r\n");
}
/* 得到版本 */
static int8* usr_4g_get_net_modlue_version(void)
{
	return inside_send_cmd("AT+VER\r\n");
}
/* 恢复出厂设置 */
static uint8_t usr_4g_set_net_modlue_reld(void)
{
	return inside_send_cmd_check("AT+RELD\r\n",RETURN_FLAG);
}
/* 重启设备 */
static uint8_t usr_4g_net_modlue_reboot(void)
{
	while(!p_4gHandle->basis_function->enter_cmd_mode());
	return inside_send_cmd_check("AT+Z\r\n",RETURN_FLAG);
}
/* 回显示 */
static uint8_t usr_4g_set_net_modlue_echo(void)
{
	return inside_send_cmd_check("AT+E\r\n",RETURN_FLAG);
}
/* 开启一个定时器 */
static uint8_t usr_4g_start_timer(void (*ptr)(void*),uint32_t timer_period)
{
	return virtual_timer1Handler->start_timerEx(virtual_timer1Handler->this_h_id,0,(void*)0,ptr,timer_period);
}
/* 关闭一个定时器 */
static uint8_t usr_4g_start_stop(void (*ptr)(void*))
{
	return virtual_timer1Handler->stop_timerEx(virtual_timer1Handler->this_h_id,0);
}
/* 断开网络 */
static uint8_t usr_4g_disconnect_net(void)
{
	/*其实就是重启*/
	while(!p_4gHandle->basis_function->enter_cmd_mode());
	return inside_send_cmd_check("AT+Z\r\n",RETURN_FLAG);
}
/* 自动重载定时器 */
timerEx_t usr_4g_timerEx = 
{
	usr_4g_start_timer,
	usr_4g_start_stop
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
	
	/*关闭回显示*/
	inside_send_cmd_check("AT+E=OFF\r\n",RETURN_FLAG);
	
	inside_send_cmd_check("AT+Z\r\n",RETURN_FLAG);/*有人的模块所有的参数必须要重启才生效*/
	
	while(!usr_set_tranmission_mode(CMD_MODE));
	
	/*关闭SOCKTA*/
	inside_send_cmd_check("AT+SOCKAEN=OFF\r\n",RETURN_FLAG);
	
	/*关闭模块自己带的心跳功能*/
	inside_send_cmd_check("AT+HEARTEN=OFF\r\n",RETURN_FLAG);
	
	/*设置工作模式为网络透传*/
	inside_send_cmd_check("AT+WKMOD=NET\r\n",RETURN_FLAG);
	
	/*设置 Socket A 为使能状态*/
	inside_send_cmd_check("AT+SOCKAEN=ON\r\n",RETURN_FLAG);
	
	/*SOCKET长连接*/
	inside_send_cmd_check("AT+SOCKASL=LONG\r\n",RETURN_FLAG);
	
}
/**
  * @brief  模块基础功能实现
  * @param  None
  * @retval : None
**/
net_module_basis_function_t implemen_net_module_basis=
{
	.enter_cmd_mode 								= usr_4g_enter_cmd_mode,
	.enter_tranmission_mode 				= usr_4g_enter_tranmission_mode,
	.get_net_modlue_mid 						= usr_4g_get_net_modlue_mid,
	.get_net_modlue_version 				= usr_4g_get_net_modlue_version,
	.set_net_modlue_reld 						= usr_4g_set_net_modlue_reld,
	.net_modlue_reboot 							= usr_4g_net_modlue_reboot,
	.set_net_modlue_echo						= usr_4g_set_net_modlue_echo,
	.net_modlue_wait_boot 					= NULL,
	.net_modlue_disconnect_net			= usr_4g_disconnect_net,
};
/**
  * @brief  实现接口函数
  * @param  None
  * @retval : None
**/
net_module_prop_t implement_net_module_property = 
{
	.uart_send_oneByte 								= usart_send_data,																			/*!实现发送一个字节*/
	.net_module_delay_ms 							= ADP_delay_ms,																							/*!实现1ms延时函数*/
	.socket_cmd_fmt	 									= usr_4g_socket_cmd_fmt,																/*!实现socket 指令格式化*/
	.set_tranmission_mode 						= usr_set_tranmission_mode,															/*!设置4g的数据传输模式*/
	.input_data 											= NULL,																									/*!这里必须为NULL 外部调用implement_net_module_property.input_data 传入串口的一帧数据*/
	.re_sent_cnt 											= RE_SENT_CNT,                  												/*!当未收到指令时重新发送指令的次数*/
	.package_time 										= PACKAGE_TE,																						/*接受一帧数据的打包时间*/
	.return_flag 											= RETURN_FLAG,																					/*!判断指令发送成功 收到模块的标志字符串*/
	.re_sent_interval_timer 					= RE_SENT_INTERVAL_TIMER,																/*!重发指令的时间间隔*/
	.socket_wait_time                 = SOCKET_CONNECT_WAIT_TIME,
	.timerEx 													= &usr_4g_timerEx,            													/*实现一个最小单位是1ms的定时器函数*/
	.net_module_basis 								= &implemen_net_module_basis,   												/*!实现网络模块基本的功能处理*/
	.Init                             = moudleInit,
};
