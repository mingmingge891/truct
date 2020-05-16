/**
  ******************************************************************************
  * @file    dl_syatem.c
  * @author  HANSEN     emil:951868692@qq.com
  * @version V1.2.0
  * @date    24-1-2019
  * @brief   
  ******************************************************************************
  * @attention
  * http://www.twc.com
	* This code developed by hardware science and technology division 
	* of chongqing fanghuitechnology co., LTD 
	* if the developer want to use the code, and the consequences shall 
	* be borne by himself. 
	* @note
  * it is belongs to the open source code.
  * <h2><center>&copy; COPYRIGHT 2019 HansenTech</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "dl_system.h"
#include "adp_mcu_system.h"
#if USB_SUPPORT	> 0
	#include "adp_mcu_usb_hid.h"
#endif
#include "dl_delay.h"
#if IR_REMOTE_SUPPORT > 0
	#include "dl_irfrared.h"
#endif
#include "dl_button.h"
	#if NRF24L01_SUPPORT > 0
#include "dl_nrf24l01.h"
#endif
#include "dl_led.h"
#include "dl_time.h"
#include "dl_serial.h"
#include "dl_net.h"
#include "malloc.h"	 

//系统任务
void Sys_Task(void *ptr)
{
#if USB_SUPPORT	> 0
	/*USB数据读取*/
	ADP_USB_HID_ReceiveHandler();
#endif
	
}

/**
  * @brief  系统初始化
  * @param  None
  * @retval None
**/
void SystemInitialize(void)
{
	/*CPU初始化*/
	ADP_MCU_Init();
	
	/*内存管理函数*/
	malloc_init();

#if USB_SUPPORT	> 0
	/*USB初始化*/
	ADP_USB_HID_Initialize();
#endif
#if TIME_SUPPORT	> 0
	
	/*定时器初始化*/
	init_timerEX();
#endif
	
#if SERIAL_SUPPORT	> 0	
	/*串口初始化*/
	serial_Init();
#endif	
	
	/*延时函数初始化*/
	delay_initialize();
	
#if IR_REMOTE_SUPPORT > 0	
	//红外发射接收模块
	IrRemote_Init();
#endif

#if SYS_BUTTON_SUPPORT > 0	
	/*系统按键初始化*/
	init_syskey();
#endif

#if SYS_LED_SUPPORT > 0
	/*系统LEDc初始化*/
	init_sysled();
#endif
	
#if NRF24L01_SUPPORT > 0	
	/*初始化NRF24L01*/
	init_nrf24l01();
#endif
	
#if NET_SUPPORT  > 0
	/*网络模块初始化*/
	dl_Net_initialize();
#endif
	
}

