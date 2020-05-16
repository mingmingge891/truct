#ifndef HS_USART_H_
#define HS_USART_H_
#include "adapter.h"
/* macro ---------------------------------------------------------------------*/
#define USART_NUM          							4

/*串口数据回调函数类型定义*/
typedef void (*usart_inside_callback_Def)
(
	uint8_t*,
	uint16_t
);

/*处理断帧数据的定时器回掉函数类型定义*/
typedef void (*usart_timerEx_callback_Def)();

/*串口结构体
	p_buf 指向动态分配内存的地址
	当前串口接收到数据的总长度
*/
typedef struct
{
	uint8_t *p_buf[USART_NUM];
	uint16_t d_length[USART_NUM];
	uint8_t 	oneBytemode[USART_NUM];
}USART_T;

/*断帧定时器回掉函数相关数据
t_callBack 当前回调函数
time_out:最大定时时间值
time_count: 定时器累加值
*/
typedef struct
{
	usart_timerEx_callback_Def t_callBack;
	uint32_t time_out;
	uint32_t time_count;
}usart_timerEx_callback_t;

/*定义4个串口回调函数*/
typedef struct
{
	void (*HS_Uart1_DataCallback)(uint8_t);
	
	void (*HS_Uart2_DataCallback)(uint8_t);
	
	void (*HS_Uart3_DataCallback)(uint8_t);
	
	void (*HS_Uart4_DataCallback)(uint8_t);
	
}USART_OUTSIDE_DATA_CALLBACK_T;

/*定义串口操作必要的抽象函数或者参数，
实现需要用户在用户层实现*/
typedef struct
{
	void (*Init)(uint8_t ,uint32_t);       							 /*! Initialize the usart */
	void (*usart_send_oneByte)(uint8_t,uint8_t);				 /*! Send one data of uart*/
	void (*satrt_ms_timerEx)(void(*)(void*),uint32_t);				 /*! start the timer of one ms*/
	uint8_t broken_frame_timeout;                            /*! set the timer of the broken frame*/
}UART_PROP_T;

/*extern value-----------------------------------------------*/

/*外部需要实现的结构体中的抽象函数以及一些参数实现*/
extern UART_PROP_T Implement_Uart_Property;
extern USART_OUTSIDE_DATA_CALLBACK_T Data_Interface;
/*API---------------------------------------------------------*/

/*API 操作结构体*/
typedef struct
{
	uint8_t uart1;
	uint8_t uart2;
	uint8_t uart3;
	uint8_t uart4;
	
	uint8_t channel;
	
	void (*config)(uint8_t usart_x,uint32_t baud_rate);
	//Mode 1 为单字节传输模式
	void (*register_callback)(uint8_t usart_x,void (*ptr)(uint8_t*,uint16_t),uint16_t BuffSize,uint8_t reMode);
	
	void (*print)(uint8_t usart_x, const char *Data,...);

	void (*sendHex)(uint8_t usart_x,uint8_t *data,uint16_t len);
	
}serial_t;

extern void hs_register_usart_callback(uint8_t usart_x,void (*ptr)(uint8_t*,uint16_t),uint16_t BuffSize,uint8_t reMode);

extern void usart_config(uint8_t usart_x,uint32_t baud_rate);

extern void serial_Init(void);

extern void hs_usart_printf(uint8_t usart_x, const char *Data,...);

extern void hs_usart_sendHex(uint8_t usart_x,uint8_t *data,uint16_t len);

extern serial_t serial;

#endif
