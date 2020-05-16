/**
  ******************************************************************************
  * @file    hs_usart.c
  * @author  HANSEN     emil:951868692@qq.com
  * @version V0.0.0
  * @date    15-november-2017
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
  * <h2><center>&copy; COPYRIGHT 2017 TFC</center></h2>
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "hs_usart.h"
#include "string.h"
#include "stdlib.h"
#include "stdarg.h"
#include <stdio.h>

/* macro ---------------------------------------------------------------------*/

#define UART1_REV_TIMEOUT_EVENT 				0
#define UART2_REV_TIMEOUT_EVENT 				1
#define UART3_REV_TIMEOUT_EVENT 				2
#define UART4_REV_TIMEOUT_EVENT 				3

/*��ʱ��֧�ֵ��¼�����*/
#define TIME_EVENT_NUM									4

/*���ö�ʱ��ʱ��*/
#define SET_TIME_MS                     1


/*funtion define --------------------------------------------------------------*/

static void hs_usart1_interface(uint8_t c_data);
static void hs_usart2_interface(uint8_t c_data);
static void hs_usart3_interface(uint8_t c_data);
static void hs_usart4_interface(uint8_t c_data);

/*Value -----------------------------------------------------------------------*/

/*����һ���ڽṹ�����*/
USART_T usart;

/*�ص����ⲿ�Ĵ��ڻص���������*/
usart_inside_callback_Def usart_callback[USART_NUM];

/*���ڳ������ṹ�����*/
UART_PROP_T* pProperty;

/*�����ⲿ���ݻص�*/
USART_OUTSIDE_DATA_CALLBACK_T* pData_callback_usartIrq;

/*�ڲ���ʱ������*/
usart_timerEx_callback_t usartTimerEx_call[TIME_EVENT_NUM];

/*Implement -------------------------------------------------------------------*/
USART_OUTSIDE_DATA_CALLBACK_T Data_Interface = 
{
	hs_usart1_interface,
	hs_usart2_interface,
	hs_usart3_interface,
	hs_usart4_interface
};

/**
  * @brief  	������ʱ��
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
  * @brief  	�رն�ʱ��
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
  * @brief  	1ms��ʱ��
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
  * @brief  	��ʼ������
  * @param  	None
  * @retval :   None
**/
void usart_Init(void)
{
	pProperty = &Implement_Uart_Property;
	
	/*�����ж����ݻص��ӿ�*/
	pData_callback_usartIrq = &Data_Interface;
	
	/*�����ϲ㶨ʱ��*/
	pProperty->satrt_ms_timerEx(onems_timerEx_handler,SET_TIME_MS);
	
}

/**
  * @brief  	ע�ᴮ�ڻص�����
  * @param  	None
  * @retval :   None
**/
void hs_register_usart_callback(USART_DEF_U usart_x,void (*ptr)(uint8_t*,uint16_t),uint32_t baud_rate,uint16_t BuffSize,
	uint8_t reMode)
{
	uint8_t index = 0;
	
	index = ((uint8_t)usart_x);
	
	if(index > USART_NUM )
	{
		return;
	}
	
	usart.oneBytemode[index] = reMode;
	
	usart_callback[index] = ptr;
	
	/*�����ڴ����ռ�*/
	if(usart.p_buf[index] == NULL)
	{
		usart.p_buf[index] = (uint8_t*)my_malloc(BuffSize);
	}
	
	/*��ʼ�����ڻ�������*/
	pProperty->Init(usart_x,baud_rate);
	
}
/**
  * @brief    ���մ�������
  * @param    None
  * @retval : None
**/
static void usart_data_handler(USART_DEF_U usart_x)
{
	/*����Ĳ���uasrt_x�Ǹ�ö������0-3
		�ص��ĸ��������ݴ���Ĳ���������
	*/
	uint8_t index = 0;
	
	index = ((uint8_t)usart_x);
	
	if(index > USART_NUM )
	{
		return;
	}
	
	/*���ݾ���Ĵ������ݻص�*/
	if(usart_callback[index] != NULL){
		usart_callback[index](usart.p_buf[index],usart.d_length[index]);
	}
	memset(usart.p_buf[index],0,usart.d_length[index]);
	
	usart.d_length[index] = 0;
}
/**
  * @brief  ��֡����
  * @param  None
  * @retval : None
**/
static void timerout_handler1(void)
{
	timerEx_stop(UART1_REV_TIMEOUT_EVENT,timerout_handler1);
	
	/*��������*/
	usart_data_handler(HS_UART1);
}
/**
  * @brief  ��֡����
  * @param  None
  * @retval : None
**/
static void timerout_handler2(void)
{
	timerEx_stop(UART2_REV_TIMEOUT_EVENT,timerout_handler2);
	
	/*��������*/
	usart_data_handler(HS_UART2);
	
}
/**
  * @brief  ��֡����
  * @param  None
  * @retval : None
**/
static void timerout_handler3(void)
{
	timerEx_stop(UART3_REV_TIMEOUT_EVENT,timerout_handler3);
	
	/*��������*/
	usart_data_handler(HS_UART3);
}
/**
  * @brief  ��֡����
  * @param  None
  * @retval : None
**/
static void timerout_handler4(void)
{
	timerEx_stop(UART4_REV_TIMEOUT_EVENT,timerout_handler4);
	
	/*��������*/
	usart_data_handler(HS_UART4);
	
}
/**
  * @brief  hal����1�жϴ���
  * @param  None
  * @retval : None
**/
static void hs_usart1_interface(uint8_t c_data)
{
	
	if(usart.oneBytemode[HS_UART1])
	{
		if(usart_callback[HS_UART1] != NULL){
			usart_callback[HS_UART1](&c_data,1);
		}
		return ;
	}
	
	/*��һ�����ڵ�����*/
	usart.p_buf[HS_UART1][usart.d_length[HS_UART1]++] = c_data;
	
	/*��ʱ����pProperty->broken_frame_timeout��֡����*/
	timerEx_start(UART1_REV_TIMEOUT_EVENT,timerout_handler1,pProperty->broken_frame_timeout);
}

/**
  * @brief  hal����2�жϴ���
  * @param  None
  * @retval : None
**/
static void hs_usart2_interface(uint8_t c_data)
{
	if(usart.oneBytemode[HS_UART2])
	{
		if(usart_callback[HS_UART2] != NULL){
			usart_callback[HS_UART2](&c_data,1);
		}
		return ;
	}
	/*�ڶ������ڵ�����*/
	usart.p_buf[HS_UART2][usart.d_length[HS_UART2]++] = c_data;
	
	/*��ʱ����pProperty->broken_frame_timeout��֡����*/
	timerEx_start(UART2_REV_TIMEOUT_EVENT,timerout_handler2,pProperty->broken_frame_timeout);
	
}
/**
  * @brief  hal����3�жϴ���
  * @param  None
  * @retval : None
**/
static void hs_usart3_interface(uint8_t c_data)
{
	if(usart.oneBytemode[HS_UART3])
	{
		if(usart_callback[HS_UART3] != NULL){
			usart_callback[HS_UART3](&c_data,1);
		}
		return ;
	}
	/*���������ڵ�����*/
	usart.p_buf[HS_UART3][usart.d_length[HS_UART3]++] = c_data;
	
	/*��ʱ����pProperty->broken_frame_timeout��֡����*/
	timerEx_start(UART3_REV_TIMEOUT_EVENT,timerout_handler3,pProperty->broken_frame_timeout);
	
}
/**
  * @brief  hal����4�жϴ���
  * @param  None
  * @retval : None
**/
static void hs_usart4_interface(uint8_t c_data)
{
	if(usart.oneBytemode[HS_UART4])
	{
		if(usart_callback[HS_UART4] != NULL){
			usart_callback[HS_UART4](&c_data,1);
		}
		return ;
	}
	/*���ĸ����ڵ�����*/
	usart.p_buf[HS_UART4][usart.d_length[HS_UART4]++] = c_data;
	
	/*��ʱ����pProperty->broken_frame_timeout��֡����*/
	timerEx_start(UART4_REV_TIMEOUT_EVENT,timerout_handler4,pProperty->broken_frame_timeout);
	
}
/**
  * @brief  	stdlibû���������Ҫ�Լ�ʵ��
  * @param  	None
  * @retval :   None
**/
static char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }
		
    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';
        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;
    return string;
}

/**
  * @brief  ʵ��printf��������
  * @param  None
  * @retval : None
  */
static void hs_usart_sprintf(USART_DEF_U usart_x, char *Data,va_list ap)
{
	const char *s;
	int d;
	double f;
	char buf[16];
	while(*Data!=0){				           		//�ж��Ƿ񵽴��ַ���������
		if(*Data==0x5c){										//'\'
			switch (*++Data){
				case 'r':							          //�س���
					pProperty->usart_send_oneByte(usart_x,0x0d);
					Data++;
					break;
				case 'n':												//���з�
					pProperty->usart_send_oneByte(usart_x, 0x0a);	
					Data++;
					break;
				default:
					Data++;
				    break;
			}
		}
		else if(*Data=='%'){
			switch (*++Data){				
				case 's':									//�ַ���
                	s = va_arg(ap, const char *);
                	for ( ; *s; s++) {
                    	pProperty->usart_send_oneByte(usart_x,*s);
                	}
					Data++;
                	break;
            	case 'd':									//ʮ����
                	d = va_arg(ap, int);
                	itoa(d, buf, 10);
                	for (s = buf; *s; s++){
                    	pProperty->usart_send_oneByte(usart_x,*s);
                	}
					Data++;
                	break;
					case 'f': 								/* double*/         
								f = va_arg(ap,double);
								sprintf(buf,"%.2f",f);
									for (s = buf; *s; s++) {
                    	pProperty->usart_send_oneByte(usart_x,*s);
                	}
					Data++;
								break;
				default:
					Data++;
				    break;
			}		 
		}
		else 
		{
			pProperty->usart_send_oneByte(usart_x,*Data++);
		}
	}
}
/**
  * @brief  ����16����
  * @param  None
  * @retval : None
  */
void hs_usart_sendHex(USART_DEF_U usart_x,uint8_t *data,uint16_t len)
{ 
	while(len--)
	{
		pProperty->usart_send_oneByte(usart_x,*data++);
	}
}

/**
  * @brief  ��ʽ���������
  * @param  None
  * @retval : None
  */
void hs_usart_printf(USART_DEF_U usart_x, char *Data,...)
{ 
	va_list ap;
	va_start(ap, Data);
	hs_usart_sprintf(usart_x,Data, ap);
	va_end(ap);
}
