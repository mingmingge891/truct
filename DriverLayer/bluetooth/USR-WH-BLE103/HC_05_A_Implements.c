/**
  ******************************************************************************
  * @file    WH-BLE103_Implements.c
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
#include "general_module2.h"
#include "adp_mcu_delay.h"
#include "bluetooth.h"
#include "dl_serial.h"
#include "dl_time.h"
#include "string.h"

#define RETURN_FLAG    						"OK"//不同的模块命令返回正确的标志不一样
#define RE_SENT_CNT    						3   //从发的次数，在没有收到模块的返回时需要重发
#define RE_SENT_INTERVAL_TIMER  	100 //每次重新发送命令的间隔时间
#define PACKAGE_TE                30  //接收打包时间

static void Serial1Callback(u8* d,u16 l){general_module2_data_input(d[0]);}

//发送一个字节
static void WH_BT200_uart_send_oneByte(uint8_t d)
{
	serial.sendHex(serial.uart1,&d,1);
}
static uint8_t usr_wifi_start_timer(void (*ptr)(void*),uint32_t timer_period){return virtual_timer1Handler->start_timerEx(virtual_timer1Handler->this_h_id,1,(void*)0,ptr,timer_period);}
static uint8_t usr_wifi_start_stop(void (*ptr)(void*)){return virtual_timer1Handler->stop_timerEx(virtual_timer1Handler->this_h_id,1);}
//定时器
static timer2Ex_t WH_BLE103_timerEx = 
{
	usr_wifi_start_timer,
	usr_wifi_start_stop
};
//模式选择
static uint8_t WH_BT200_set_tranmission_mode(uint8_t mode)
{	
//	if(mode == CMD_MODE)
//	{
//		/*先判断是否在AT模式 任意发送一个指令测试一下*/
//		if(send_cmd2_check(NULL,"AT+CIVER\r\n")){
//			return 1;
//		}
//		/*在发送+++之前200ms内不能有数据*/
//		ADP_delay_ms(300);
//		if(send_cmd2_check("a+ok","+++a")){
//			return 1;//send_cmd2_check("+ok","a");
//		}
//		return 0;
//	}
//	else if(mode == TRANSMISSION_MODE){
//		return send_cmd2_check(RETURN_FLAG,"AT+ENTM\r\n");
//	}
//	return 0;
	return 1;
}

/*实现通用接口模版*/
general_module2_interface_t implement_general_module2_interface =
{
	.re_sent_cnt                        = RE_SENT_CNT,
	.package_time                       = PACKAGE_TE,
	.return_flag                        = RETURN_FLAG,
	.re_sent_interval_timer             = RE_SENT_INTERVAL_TIMER,
	.uart_send_oneByte                  = WH_BT200_uart_send_oneByte,
	.delay_ms                           = ADP_delay_ms,
	.set_tranmission_mode               = WH_BT200_set_tranmission_mode,
	.timerEx                            = &WH_BLE103_timerEx,
};

//UUID获取
char* GetUUID(void)
{
//	while(!general_module2_enter_cmd_mode());
//	return send_cmd2("AT+UUID?\r\n");
	return "NULL";
}
//UUID获取
char* GetMac(void)
{
	//while(!general_module2_enter_cmd_mode());
	//return send_cmd2("AT+UUID?\r\n");
	return "NULL";
}
void Reboot()
{
	//while(!general_module2_enter_cmd_mode());
	//send_cmd2("AT+Z\r\n");
}
static void NoneFunctionForCallBcak(u8* d,u16 l){}
/*初始化蓝牙2*/
void initBluetooth2(void)
{
	/*初始化串口模块*/
	general_module2_initialize();
	general_module2_setbuffer(64,128);
	general_module2_setCb(NoneFunctionForCallBcak);//首先设置一个空函数
	serial.config(serial.uart1,9600);
	serial.register_callback(serial.uart1,Serial1Callback,8,1);
	/*有个原因 参数设置不清楚*/
}

