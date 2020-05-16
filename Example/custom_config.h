/**
  ******************************************************************************
  * @file    sustom_config.h
  * @author  HANSEN     emil:951868692@qq.com
  * @version 0.1.0
  * @date    23-3-2020
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
  * <h2><center>&copy; COPYRIGHT 2020 HansenTech</center></h2>
  ******************************************************************************
  */
#ifndef _CUSTOM_CONFIG_H_
#define _CUSTOM_CONFIG_H_

//模块支持
#define NET_SUPPORT                    0
#define USB_SUPPORT                    0  
#define NRF24L01_SUPPORT               0
#define SYS_BUTTON_SUPPORT             0
#define SYS_LED_SUPPORT                0
#define IR_REMOTE_SUPPORT              0
#define SERIAL_SUPPORT                 1
#define TIME_SUPPORT                   1
//串口支持配置
#define SERIAL1_SUPPORT                1
#define SERIAL2_SUPPORT                1
#define SERIAL3_SUPPORT                1
//定时器支持配置
#define TIME2_SUPPORT                  1
#define TIME3_SUPPORT                  1
#define TIME4_SUPPORT                  1
//USB中断优先级配置
#define USB_RX0_NVIC_IRQ_PP            1//抢占优先级
#define USB_RX0_NVIC_IRQ_SP            1//子优先级
#define USB_TX_NVIC_IRQ_PP             1//抢占优先级
#define USB_TX_NVIC_IRQ_SP             0//子优先级
//串口中断优先级配置
#define SERIAL1_NVIC_IRQ_PP            1//抢占优先级
#define SERIAL1_NVIC_IRQ_SP            0//子优先级
#define SERIAL2_NVIC_IRQ_PP            0//抢占优先级
#define SERIAL2_NVIC_IRQ_SP            0//子优先级
#define SERIAL3_NVIC_IRQ_PP            1//抢占优先级
#define SERIAL3_NVIC_IRQ_SP            2//子优先级
//定时器中断优先级配置
#define TIME2_NVIC_IRQ_PP              1//抢占优先级
#define TIME2_NVIC_IRQ_SP              0//子优先级
#define TIME3_NVIC_IRQ_PP              0//抢占优先级
#define TIME3_NVIC_IRQ_SP              2//子优先级
#define TIME4_NVIC_IRQ_PP              2//抢占优先级
#define TIME4_NVIC_IRQ_SP              1//子优先级
#endif
