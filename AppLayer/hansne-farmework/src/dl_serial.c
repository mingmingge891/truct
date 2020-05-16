/**
  ******************************************************************************
  * @file    al_serial.c
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
#include "dl_serial.h"
#include "malloc.h"
#include "string.h"
#include "stdlib.h"
#include "stdarg.h"
#include <stdio.h>

/* macro ---------------------------------------------------------------------*/

#define UART1_REV_TIMEOUT_EVENT 				0
#define UART2_REV_TIMEOUT_EVENT 				1
#define UART3_REV_TIMEOUT_EVENT 				2
#define UART4_REV_TIMEOUT_EVENT 				3

/*定时器支持的事件个数*/
#define TIME_EVENT_NUM									4

/*设置定时器时间*/
#define SET_TIME_MS                     1


/*funtion define --------------------------------------------------------------*/

static void hs_usart1_interface(uint8_t c_data);
static void hs_usart2_interface(uint8_t c_data);
static void hs_usart3_interface(uint8_t c_data);
static void hs_usart4_interface(uint8_t c_data);

/*Value -----------------------------------------------------------------------*/

/*定义一串口结构体变量*/
USART_T usart;

/*回掉给外部的串口回调变量数组*/
usart_inside_callback_Def usart_callback[USART_NUM];

/*串口抽象函数结构体变量*/
UART_PROP_T* pSerialProperty;

/*内部定时器定义*/
usart_timerEx_callback_t usartTimerEx_call[TIME_EVENT_NUM];

/*Implement -------------------------------------------------------------------*/
USART_OUTSIDE_DATA_CALLBACK_T Data_Interface = 
{
	hs_usart1_interface,
	hs_usart2_interface,
	hs_usart3_interface,
	hs_usart4_interface
};

/*API 函数初始化*/
serial_t serial = 
{
	.uart1 = 0,
	.uart2 = 1,
	.uart3 = 2,
	.uart4 = 3,
	.config = usart_config,
	.register_callback = hs_register_usart_callback,
	.print = hs_usart_printf,
	.sendHex = hs_usart_sendHex,
};

/**
  * @brief  	启动定时器
  * @param  	None
  * @retval :   None
**/
static void timerEx_start(uint8_t timerEx_id,void (*ptr)(void),uint32_t timer_period)
{
	if(timerEx_id > (TIME_EVENT_NUM-1))
	{
		return;
	}
	usartTimerEx_call[timerEx_id].t_callBack = ptr;
	usartTimerEx_call[timerEx_id].time_out = timer_period;
	usartTimerEx_call[timerEx_id].time_count = 0;
}

/**
  * @brief  	关闭定时器
  * @param  	None
  * @retval :   None
**/
static void timerEx_stop(uint8_t timerEx_id,void (*ptr)(void))
{
	if(timerEx_id > (TIME_EVENT_NUM-1))
	{
		return;
	}
	usartTimerEx_call[timerEx_id].t_callBack = ptr;
	usartTimerEx_call[timerEx_id].time_out = 0;
	usartTimerEx_call[timerEx_id].time_count = 0;
}

/**
  * @brief  	1ms定时器
  * @param  	None
  * @retval :   None
**/
static void onems_timerEx_handler(void* arg)
{
	uint8_t i = 0;
	for(i = 0; i < TIME_EVENT_NUM;i++)
	{
		if(++usartTimerEx_call[i].time_count == usartTimerEx_call[i].time_out)
		{
			usartTimerEx_call[i].time_count = 0;
			usartTimerEx_call[i].t_callBack();
		}
	}
}

/**
  * @brief  	初始化串口
  * @param  	None
  * @retval :   None
**/
void usart_config(uint8_t usart_x,uint32_t baud_rate)
{
	/*串口初始化*/
	ADP_USART_Init(usart_x+1);
	
	/*初始化串口基础设置*/
	 pSerialProperty->Init(usart_x,baud_rate);
}
	
/**
  * @brief  	初始化串口
  * @param  	None
  * @retval :   None
**/
void serial_Init(void)
{
	pSerialProperty = &Implement_Uart_Property;
}

/**
  * @brief  	注册串口回调函数
  * @param  	None
  * @retval :   None
**/
void hs_register_usart_callback(uint8_t usart_x,void (*ptr)(uint8_t*,uint16_t),uint16_t BuffSize,uint8_t reMode)
{
	uint8_t index = 0;
	
	index = ((uint8_t)usart_x);
	
	if(index > USART_NUM )
	{
		return;
	}
	
	usart.oneBytemode[index] = reMode;
	
	usart_callback[index] = ptr;
	
	/*申请内存分配空间*/
	if(usart.p_buf[index] == NULL)
	{
		usart.p_buf[index] = (uint8_t*)my_malloc(BuffSize);
	}
	
	/*启动上层定时器 用来判断断帧处理*/
	pSerialProperty->satrt_ms_timerEx(onems_timerEx_handler,SET_TIME_MS);
	
	/*打开中断*/
	ADP_USART_IT_Config(index+1);
	
}
/**
  * @brief    接收串口数据
  * @param    None
  * @retval : None
**/
static void usart_data_handler(uint8_t usart_x)
{
	/*传入的参数uasrt_x是个枚举类型0-3
		回调哪个函数根据传入的参数决定。
	*/
	uint8_t index = 0;
	
	index = ((uint8_t)usart_x);
	
	if(index > USART_NUM )
	{
		return;
	}
	
	/*根据具体的串口数据回调*/
	if(usart_callback[index] != NULL){
		usart_callback[index](usart.p_buf[index],usart.d_length[index]);
	}
	memset(usart.p_buf[index],0,usart.d_length[index]);
	
	usart.d_length[index] = 0;
}
/**
  * @brief  断帧处理
  * @param  None
  * @retval : None
**/
static void timerout_handler1(void)
{
	timerEx_stop(UART1_REV_TIMEOUT_EVENT,timerout_handler1);
	
	/*处理数据*/
	usart_data_handler(serial.uart1);
}
/**
  * @brief  断帧处理
  * @param  None
  * @retval : None
**/
static void timerout_handler2(void)
{
	timerEx_stop(UART2_REV_TIMEOUT_EVENT,timerout_handler2);
	
	/*处理数据*/
	usart_data_handler(serial.uart2);
	
}
/**
  * @brief  断帧处理
  * @param  None
  * @retval : None
**/
static void timerout_handler3(void)
{
	timerEx_stop(UART3_REV_TIMEOUT_EVENT,timerout_handler3);
	
	/*处理数据*/
	usart_data_handler(serial.uart3);
}
/**
  * @brief  断帧处理
  * @param  None
  * @retval : None
**/
static void timerout_handler4(void)
{
	timerEx_stop(UART4_REV_TIMEOUT_EVENT,timerout_handler4);
	
	/*处理数据*/
	usart_data_handler(serial.uart4);
	
}
/**
  * @brief  hal串口1中断处理
  * @param  None
  * @retval : None
**/
static void hs_usart1_interface(uint8_t c_data)
{
	
	if(usart.oneBytemode[serial.uart1])
	{
		if(usart_callback[serial.uart1] != NULL){
			usart_callback[serial.uart1](&c_data,1);
		}
		return ;
	}
	
	/*第一个串口的数据*/
	usart.p_buf[serial.uart1][usart.d_length[serial.uart1]++] = c_data;
	
	/*超时处理pSerialProperty->broken_frame_timeout断帧处理*/
	timerEx_start(UART1_REV_TIMEOUT_EVENT,timerout_handler1,pSerialProperty->broken_frame_timeout);
}

/**
  * @brief  hal串口2中断处理
  * @param  None
  * @retval : None
**/
static void hs_usart2_interface(uint8_t c_data)
{
	if(usart.oneBytemode[serial.uart2])
	{
		if(usart_callback[serial.uart2] != NULL){
			usart_callback[serial.uart2](&c_data,1);
		}
		return ;
	}
	/*第二个串口的数据*/
	usart.p_buf[serial.uart2][usart.d_length[serial.uart2]++] = c_data;
	
	/*超时处理pSerialProperty->broken_frame_timeout断帧处理*/
	timerEx_start(UART2_REV_TIMEOUT_EVENT,timerout_handler2,pSerialProperty->broken_frame_timeout);
	
}
/**
  * @brief  hal串口3中断处理
  * @param  None
  * @retval : None
**/
static void hs_usart3_interface(uint8_t c_data)
{
	if(usart.oneBytemode[serial.uart3])
	{
		if(usart_callback[serial.uart3] != NULL){
			usart_callback[serial.uart3](&c_data,1);
		}
		return ;
	}
	/*第三个串口的数据*/
	usart.p_buf[serial.uart3][usart.d_length[serial.uart3]++] = c_data;
	
	/*超时处理pSerialProperty->broken_frame_timeout断帧处理*/
	timerEx_start(UART3_REV_TIMEOUT_EVENT,timerout_handler3,pSerialProperty->broken_frame_timeout);
	
}
/**
  * @brief  hal串口4中断处理
  * @param  None
  * @retval : None
**/
static void hs_usart4_interface(uint8_t c_data)
{
	if(usart.oneBytemode[serial.uart4])
	{
		if(usart_callback[serial.uart4] != NULL){
			usart_callback[serial.uart4](&c_data,1);
		}
		return ;
	}
	/*第四个串口的数据*/
	usart.p_buf[serial.uart4][usart.d_length[serial.uart4]++] = c_data;
	
	/*超时处理pSerialProperty->broken_frame_timeout断帧处理*/
	timerEx_start(UART4_REV_TIMEOUT_EVENT,timerout_handler4,pSerialProperty->broken_frame_timeout);
	
}

/**
  * @brief  发送16进制
  * @param  None
  * @retval : None
  */
void hs_usart_sendHex(uint8_t usart_x,uint8_t *data,uint16_t len)
{ 
	while(len--)
	{
		pSerialProperty->usart_send_oneByte(usart_x,*data++);
	}
}

/**
  * @brief  格式化输出数据不占用缓存
  * @param  None
  * @retval : None
  */
void hs_usart_printf(uint8_t usart_x, const char *format,...)
{ 
	serial.channel = (uint8_t)usart_x;
	va_list ap;
	va_start(ap, format);
	vprintf(format,ap);
	va_end(ap);
}

/**
  * @brief  格式化输出数据占用缓存
  * @param  None
  * @retval : None
  */
#if 0
void hs_usart_printf(uint8_t usart_x, const char *format, ...)
{
	#define IFS_PRINTF_BUF_LEN 100
	static u8 prt_buf[IFS_PRINTF_BUF_LEN];
	u8* pBuf = prt_buf;
	va_list ap;
	uint32_t size = 0;
	
	va_start(ap, format);
	size = vsnprintf((char *)pBuf, IFS_PRINTF_BUF_LEN, format, ap);
	va_end(ap);

	pBuf[size] = 0;
	while(*pBuf !=0)
	pSerialProperty->usart_send_oneByte(usart_x,*pBuf++);
}

#endif
/**
  * @brief    fputc
  * @param    None
  * @retval : None
  */
#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000

struct __FILE { int handle; /* Add whatever needed */ };
FILE __stdout;
FILE __stdin;
int fputc(int ch, FILE *f)
{
	pSerialProperty->usart_send_oneByte(serial.channel,ch);
//	if (DEMCR & TRCENA) {
//    while (ITM_Port32(0) == 0);
//    ITM_Port8(0) = ch;
//  }
 return(ch);
}
///ÖØ¶¨Ïòc¿âº¯Êýscanfµ½´®¿Ú£¬ÖØÐ´Ïòºó¿ÉÊ¹ÓÃscanf¡¢getcharµÈº¯Êý
int fgetc(FILE *f)
{
//		/* µÈ´ý´®¿ÚÊäÈëÊý¾Ý */
//		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

//		return (int)USART_ReceiveData(USART1);
	return 1;
}

