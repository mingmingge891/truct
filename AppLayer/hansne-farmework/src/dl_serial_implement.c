/**
  ******************************************************************************
  * @file    .c
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
#include "adapter.h"
#include "dl_system.h"
#include "dl_serial.h"
#include "dl_time.h"
#include "string.h"

//定时器
timer_t *usartTime = NULL;
#define  SERIAL_TIME_EVENT     0

//usrat1 回调函数
void usart1_callback(uint8_t d)
{
	Data_Interface.HS_Uart1_DataCallback(d);
}
//usrat2 回调函数
void usart2_callback(uint8_t d)
{
	Data_Interface.HS_Uart2_DataCallback(d);
}
//usrat3 回调函数
void usart3_callback(uint8_t d)
{
	Data_Interface.HS_Uart3_DataCallback(d);
}
//usrat4 回调函数
void usart4_callback(uint8_t d)
{
	Data_Interface.HS_Uart4_DataCallback(d);
}

/**
  * @brief  串口初始化
  * @param  None
  * @retval : None
  */
static void Uart_Property_Init(uint8_t Usartx,uint32_t Baud_Rate)
{
	usartTime = timerNew(1);
	if(Usartx == serial.uart1)
	{
		ADP_USART_Config(1,Baud_Rate,8,0,1);
	}
	if(Usartx == serial.uart2)
	{
		ADP_USART_Config(2,Baud_Rate,8,0,1);
	}
	if(Usartx == serial.uart3)
	{
		ADP_USART_Init(3);
		ADP_USART_Config(3,Baud_Rate,8,0,1);
	}
	if(Usartx == serial.uart4)
	{
		ADP_USART_Init(4);
		ADP_USART_Config(4,Baud_Rate,8,0,1);
	}
	
	/*串口中断设置*/
	ADP_USART_SetIrqCb(1,usart1_callback);
	ADP_USART_SetIrqCb(2,usart2_callback);
	ADP_USART_SetIrqCb(3,usart3_callback);
	ADP_USART_SetIrqCb(4,usart4_callback);
}
/**
  * @brief  串口发送1字节数据STM32单片机
  * @param  None
  * @retval : None
  */
static void Uart_Send_Byte(uint8_t Usartx,uint8_t Data)
{
	if(Usartx == serial.uart1){
		ADP_USART_SendChar(1,Data);
	}
	else if(Usartx == serial.uart2){
		ADP_USART_SendChar(2,Data);
	}
	else if(Usartx == serial.uart3){
		ADP_USART_SendChar(3,Data);
	}
}
/**
  * @brief  启动一个1ms的定时器
  * @param  ptr 函数地址
						time_out 定时时间值单位是1ms
  * @retval : None
  */
static void start_1ms_timer(void (*ptr)(void*),uint32_t time_out)
{
	usartTime->start(usartTime->instance,(void*)0,ptr,time_out,SERIAL_TIME_EVENT);
}

/*串口参数设置以及函数接口实现*/
UART_PROP_T Implement_Uart_Property =
{
	Uart_Property_Init,//串口的基本参数设置
	Uart_Send_Byte,//实现一个发送一个字节的函数
	start_1ms_timer,//实现一个1ms的支持回调的定时器函数
	10//10ms断帧处理
};

