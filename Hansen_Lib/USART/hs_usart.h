#ifndef HS_USART_H_
#define HS_USART_H_

#define STM32
//������һЩ���Ͷ���������stm32�ġ���

#ifdef 	STM32

#include "hal_init.h"

#else
typedef unsigned char  uint8_t;              /* Unsigned  8 bit quantity                           */
typedef signed	 char  int8;                 /* Signed    8 bit quantity                           */
typedef unsigned short uint16_t;             /* Unsigned 16 bit quantity                           */
typedef          short int16;                /* Signed   16 bit quantity                           */
typedef unsigned int   uint32_t;             /* Unsigned 32 bit quantity                           */
typedef 	       int   int32;          			 /* Signed   32 bit quantity                           */
typedef float          fp32;                 /* Single precision floating point                    */
typedef double         fp64;                 /* Double precision floating point                    */
#endif

/* macro ---------------------------------------------------------------------*/
#define USART_NUM          							4

/*����4������ö������*/
typedef enum
{
	HS_UART1 = 0,
	HS_UART2 = 1,
	HS_UART3 = 2,
	HS_UART4 = 3
}USART_DEF_U;

/*�������ݻص��������Ͷ���*/
typedef void (*usart_inside_callback_Def)
(
	uint8_t*,
	uint16_t
);

/*�����֡���ݵĶ�ʱ���ص��������Ͷ���*/
typedef void (*usart_timerEx_callback_Def)();

/*���ڽṹ��
	p_buf ָ��̬�����ڴ�ĵ�ַ
	��ǰ���ڽ��յ����ݵ��ܳ���
*/
typedef struct
{
	uint8_t *p_buf[USART_NUM];
	uint16_t d_length[USART_NUM];
	uint8_t 	oneBytemode[USART_NUM];
}USART_T;

/*��֡��ʱ���ص������������
t_callBack ��ǰ�ص�����
time_out:���ʱʱ��ֵ
time_count: ��ʱ���ۼ�ֵ
*/
typedef struct
{
	usart_timerEx_callback_Def t_callBack;
	uint32_t time_out;
	uint32_t time_count;
}usart_timerEx_callback_t;

/*����4�����ڻص�����*/
typedef struct
{
	void (*HS_Uart1_DataCallback)(uint8_t);
	
	void (*HS_Uart2_DataCallback)(uint8_t);
	
	void (*HS_Uart3_DataCallback)(uint8_t);
	
	void (*HS_Uart4_DataCallback)(uint8_t);
	
}USART_OUTSIDE_DATA_CALLBACK_T;

/*���崮�ڲ�����Ҫ�ĳ��������߲�����
ʵ����Ҫ�û����û���ʵ��*/
typedef struct
{
	void (*Init)(USART_DEF_U ,uint32_t);       							 /*! Initialize the usart */
	void (*usart_send_oneByte)(USART_DEF_U,uint8_t);				 /*! Send one data of uart*/
	void (*satrt_ms_timerEx)(void(*)(void*),uint32_t);				 /*! start the timer of one ms*/
	uint8_t broken_frame_timeout;                            /*! set the timer of the broken frame*/
}UART_PROP_T;

/*extern value-----------------------------------------------*/

/*�ⲿʹ���������ṹ���еĺ�����ʵ���ڲ��Ѿ�д�á�*/
extern USART_OUTSIDE_DATA_CALLBACK_T* pData_callback_usartIrq;

/*�ⲿ��Ҫʵ�ֵĽṹ���еĳ������Լ�һЩ����ʵ��*/
extern UART_PROP_T Implement_Uart_Property;

/*API---------------------------------------------------------*/

extern void hs_register_usart_callback(USART_DEF_U usart_x,void (*ptr)(uint8_t*,uint16_t),uint32_t baud_rate,uint16_t BuffSize,uint8_t reMode);

extern void usart_Init(void);

extern void hs_usart_printf(USART_DEF_U usart_x, char *Data,...);

extern void hs_usart_sendHex(USART_DEF_U usart_x,uint8_t *data,uint16_t len);

#endif
