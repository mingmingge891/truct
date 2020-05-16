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
#include "general_module.h"
#include "adp_mcu_delay.h"
#include "bluetooth.h"
#include "dl_serial.h"
#include "dl_time.h"
#include "string.h"


/*
	 Bluetooth Specification V4.2 EDR and BLE，协议向下兼容，EDR 与 BLE 两种
	 模式可同时工作
 主从一体，快速切换
 串口 AT 指令配置
 支持 1 多对连接，主模式下最多可同时连接 7 个从设备
 支持 1 对多广播模式
 支持互通模式，实现 EDR 与 BLE 设备兼容
 蓝牙转 UART 数据传输，支持高速串口，最高支持 3Mbps
 内置 2.4G PCB 板载天线，用户无需外接天线
 支持外接天线，增大传输距离
 3.0V~3.6V 单电源供电
*/

/*蓝颜配对的密码必须是0*/

#define RETURN_FLAG    						"OK"//不同的模块命令返回正确的标志不一样
#define RE_SENT_CNT    						3    //从发的次数，在没有收到模块的返回时需要重发
#define RE_SENT_INTERVAL_TIMER  	100   //每次重新发送命令的间隔时间
#define PACKAGE_TE                20   //接收打包时间

BluetoothDeviceType BllueToohDevice;//蓝牙设备

static void Serial1Callback(u8* d,u16 l){general_module_data_input(d[0]);}
static void Serial2Callback(u8* d,u16 l){general_module_data_input(d[0]);}
static void WH_BT200_uart_send_oneByte(uint8_t d)
{
	if(BllueToohDevice == DEVICE1){
		serial.sendHex(serial.uart2,&d,1);
	}else if(BllueToohDevice == DEVICE2){
		serial.sendHex(serial.uart1,&d,1);
	}
}
static uint8_t usr_wifi_start_timer(void (*ptr)(void*),uint32_t timer_period){return virtual_timer1Handler->start_timerEx(virtual_timer1Handler->this_h_id,0,(void*)0,ptr,timer_period);}
static uint8_t usr_wifi_start_stop(void (*ptr)(void*)){return virtual_timer1Handler->stop_timerEx(virtual_timer1Handler->this_h_id,0);}
static timerEx_t WH_BT200_timerEx = 
{
	usr_wifi_start_timer,
	usr_wifi_start_stop
};
static uint8_t WH_BT200_set_tranmission_mode(uint8_t mode)
{	
	if(mode == CMD_MODE)
	{
		/*先判断是否在AT模式 任意发送一个指令测试一下*/
		if(send_cmd_check(NULL,"AT+CIVER\r\n")){
			return 1;
		}
		/*在发送+++之前200ms内不能有数据*/
		ADP_delay_ms(300);
		if(send_cmd_check("a","+++")){
			return send_cmd_check("+ok","a");
		}
		return 0;
	}
	else if(mode == TRANSMISSION_MODE){
		return send_cmd_check(RETURN_FLAG,"AT+ENTM\r\n");
	}
	return 0;
}

/*实现通用接口模版*/
general_module_interface_t implement_general_module_interface =
{
	.re_sent_cnt                        = RE_SENT_CNT,
	.package_time                       = PACKAGE_TE,
	.return_flag                        = RETURN_FLAG,
	.re_sent_interval_timer             = RE_SENT_INTERVAL_TIMER,
	.uart_send_oneByte                  = WH_BT200_uart_send_oneByte,
	.delay_ms                           = ADP_delay_ms,
	.set_tranmission_mode               = WH_BT200_set_tranmission_mode,
	.timerEx                            = &WH_BT200_timerEx,
};

static void WH_BT200_REboot_Handler(void* ptr)
{
	//virtual_timer1Handler->stop_timerEx(//virtual_timer1Handler->this_h_id,1);
	send_cmd_check(RETURN_FLAG,"AT+Z\r\n");//重启模块	
}

static void WH_BT200_Reboot(void)
{
	while(!general_module_enter_cmd_mode());
	send_cmd_check(RETURN_FLAG,"AT+Z\r\n");//重启模块	

}
static uint8_t WH_BT200_SetName(const char* str)
{
	/*启动一个定时器用来重启模块模块需要重启才能使配置的参数生效 3s后开始执行(自动重载,调用更新)*/
	//virtual_timer1Handler->start_timerEx(//virtual_timer1Handler->this_h_id,1,NULL,WH_BT200_REboot_Handler,4*1000);
	if(strlen(str) >10 )
		return 0;
	while(!general_module_enter_cmd_mode());
	send_cmd_check(RETURN_FLAG,"AT+SNAME=%s\r\n",str);//设置SPP模式下的名字
	send_cmd_check(RETURN_FLAG,"AT+BNAME=%s\r\n",str);//设置BLE模式下的名字
	
	return 1;
}
static char* WH_BT200_SearchSlaveDevice(void)
{
	return send_cmd("");
}
static uint8_t WH_BT200_Connect(int8_t* mac)
{
	/*启动一个定时器用来重启模块模块需要重启才能使配置的参数生效 3s后开始执行(自动重载,调用更新)*/
	//virtual_timer1Handler->start_timerEx(//virtual_timer1Handler->this_h_id,1,NULL,WH_BT200_REboot_Handler,3*1000);
	while(!general_module_enter_cmd_mode());
	if(!send_cmd_check("+CONNADDS","AT+CONNADDS=%s,%d\r\n",mac,1))//通过MAC连接设备
		return 0;
	if(!send_cmd_check("+AUTOSCAN","AT+AUTOSCAN=ON\r\n"))//通过MAC连接设备//开启自动上电扫描功能
		return 0;
	return 1;
}
static void WH_BT200_Disconnect(void)
{
	/*启动一个定时器用来重启模块模块需要重启才能使配置的参数生效 3s后开始执行(自动重载,调用更新)*/
	//virtual_timer1Handler->start_timerEx(//virtual_timer1Handler->this_h_id,1,NULL,WH_BT200_REboot_Handler,3*1000);
	while(!general_module_enter_cmd_mode());
	send_cmd_check(RETURN_FLAG,"AT+DISCONN\r\n");//断开所有的连接
}
static uint8_t WH_BT200_DB_Set(int8_t TPL)
{
	/*启动一个定时器用来重启模块模块需要重启才能使配置的参数生效 3s后开始执行(自动重载,调用更新)*/
	//virtual_timer1Handler->start_timerEx(//virtual_timer1Handler->this_h_id,1,NULL,WH_BT200_REboot_Handler,3*1000);
	if(TPL <= 10 && TPL >= -25)
	{
		while(!general_module_enter_cmd_mode());
		send_cmd_check(RETURN_FLAG,"AT+TPL=%d\r\n",TPL);
		return 1;
	}
	return 0;
}
static void WH_BT200_WorkModeSet(WorkModeType Mode)
{
	/*启动一个定时器用来重启模块模块需要重启才能使配置的参数生效 3s后开始执行(自动重载,调用更新)*/
	//virtual_timer1Handler->start_timerEx(//virtual_timer1Handler->this_h_id,1,NULL,WH_BT200_REboot_Handler,3*1000);
	while(!general_module_enter_cmd_mode());
	if(Mode == BLE_MASTER)
		send_cmd_check(RETURN_FLAG,"AT+MODE=B\r\n");
	else if(Mode == SPP_MASTER)
		send_cmd_check(RETURN_FLAG,"AT+MODE=T\r\n");
	else if(Mode == SLAVER)
		send_cmd_check(RETURN_FLAG,"AT+MODE=S\r\n");
	else if(Mode == IBEACON)
		send_cmd_check(RETURN_FLAG,"AT+MODE=I\r\n");
}
static char* WH_BT200_GetMac(void)
{
	/*启动一个定时器用来重启模块模块需要重启才能使配置的参数生效 3s后开始执行(自动重载,调用更新)*/
	//virtual_timer1Handler->start_timerEx(//virtual_timer1Handler->this_h_id,1,NULL,WH_BT200_REboot_Handler,3*1000);
	while(!general_module_enter_cmd_mode());
	return send_cmd("AT+MAC?\r\n");
}
static void WH_BT200_SetBroadcastSpeed(uint8_t Speed){	Speed = Speed;}
static void NoneFunctionForCallBcak(u8* d,u16 l){}
static void WH_Bt200_OpenDevice(BluetoothDeviceType Device)
{	
	BllueToohDevice = Device;
	
	/*初始化串口模块*/
	genseral_module_initialize();
	general_module_setbuffer(128,128);
	general_module_setCb(NoneFunctionForCallBcak);//首先设置一个空函数
	
	if(Device == DEVICE1){
		/*设置打开串口2*/
		serial.config(serial.uart2,115200);
		serial.register_callback(serial.uart2,Serial2Callback,8,1);
	}
	else if(Device == DEVICE2){
		/*设置打开串口1*/
		serial.config(serial.uart1,115200);
		serial.register_callback(serial.uart1,Serial1Callback,8,1);
	}
}
//电源控制
static void WH_Bt200_CloseDevice(BluetoothDeviceType Device)
{
	if(Device == DEVICE1){
		//电源控制
	}
	else if(Device == DEVICE2){
		//电源控制
	}	
}

/*实现蓝牙接口接口*/
BluetoothInterface_t BluetoothImplements = 
{
	.SetName                  = WH_BT200_SetName,
	.SearchSlaveDevice        = WH_BT200_SearchSlaveDevice,
	.Connect                  = WH_BT200_Connect,
	.Disconnect               = WH_BT200_Disconnect,
	.DB_Set                   = WH_BT200_DB_Set,
	.WorkModeSet              = WH_BT200_WorkModeSet,
	.GetMac                   = WH_BT200_GetMac,
	.SetBroadcastSpeed        = WH_BT200_SetBroadcastSpeed,
	.OpenDevice               = WH_Bt200_OpenDevice,
	.CloseDevice              = WH_Bt200_CloseDevice,
	.SetCallback              = general_module_setCb,
	.HexTranmission           = hex_data_tranmission,
	.Reboot                   = WH_BT200_Reboot,
};

